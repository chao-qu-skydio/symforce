/* ----------------------------------------------------------------------------
 * SymForce - Copyright 2022, Skydio, Inc.
 * This source code is under the Apache 2.0 license found in the LICENSE file.
 * ---------------------------------------------------------------------------- */

#pragma once

#include <ostream>

#include <Eigen/Sparse>

#include <lcmtypes/sym/index_entry_t.hpp>
#include <lcmtypes/sym/linearized_dense_factor_t.hpp>
#include <lcmtypes/sym/linearized_dense_factorf_t.hpp>

#include "./templates.h"
#include "./values.h"

namespace sym {

template <typename _S>
struct LinearizedDenseFactorTypeHelper;

template <typename _S>
struct LinearizedSparseFactorTypeHelper;

// NOTE(aaron): Unlike the dense versions of these, we don't have SparseMatrix eigen_lcm types, so
// we just defined these as structs, since we don't need to serialize them anyway
struct linearized_sparse_factor_t {
  index_t index;

  Eigen::VectorXd residual;              // b
  Eigen::SparseMatrix<double> jacobian;  // J
  Eigen::SparseMatrix<double> hessian;   // H, JtJ
  Eigen::VectorXd rhs;                   // Jtb

  static constexpr const char* getTypeName() {
    return "linearized_sparse_factor_t";
  }
};

struct linearized_sparse_factorf_t {
  index_t index;

  Eigen::VectorXf residual;             // b
  Eigen::SparseMatrix<float> jacobian;  // J
  Eigen::SparseMatrix<float> hessian;   // H, JtJ
  Eigen::VectorXf rhs;                  // Jtb

  static constexpr const char* getTypeName() {
    return "linearized_sparse_factorf_t";
  }
};

/**
 * A residual term for optimization.
 *
 * Created from a function and a set of Keys that act as inputs. Given a Values as an evaluation
 * point, generates a linear approximation to the residual function. Contains templated helper
 * constructors to simplify creation.
 */
template <typename ScalarType>
class Factor {
 public:
  using Scalar = ScalarType;

  // Helpers to expose the correct LCM type for the scalar type
  using LinearizedDenseFactor = typename LinearizedDenseFactorTypeHelper<Scalar>::Type;
  using LinearizedSparseFactor = typename LinearizedSparseFactorTypeHelper<Scalar>::Type;

  // ----------------------------------------------------------------------------------------------
  // Residual function forms
  //
  // These are the functions the Factor operates on. In fact it stores only a HessianFunc (or
  // SparseHessianFunc) as any JacobianFunc specification is used to compute a HessianFunc. However,
  // it is not common for the user to specify these directly as usually functions do not accept a
  // Values, rather the individual input arguments. There are helper constructors to automate the
  // task of extracting and forwarding the proper inputs from the Values using the Keys in the
  // Factor.
  // ----------------------------------------------------------------------------------------------

  template <typename JacobianMatrixType>
  using JacobianFunc = std::function<void(const Values<Scalar>&,              // Input storage
                                          const std::vector<index_entry_t>&,  // Keys
                                          VectorX<Scalar>*,                   // Mx1 residual
                                          JacobianMatrixType*                 // MxN jacobian
                                          )>;

  using DenseJacobianFunc = JacobianFunc<MatrixX<Scalar>>;
  using SparseJacobianFunc = JacobianFunc<Eigen::SparseMatrix<Scalar>>;

  template <typename MatrixType>
  using HessianFunc = std::function<void(const Values<Scalar>&,              // Input storage
                                         const std::vector<index_entry_t>&,  // Keys
                                         VectorX<Scalar>*,                   // Mx1 residual
                                         MatrixType*,                        // MxN jacobian
                                         MatrixType*,                        // NxN hessian
                                         VectorX<Scalar>*                    // Nx1 right-hand side
                                         )>;

  using DenseHessianFunc = HessianFunc<MatrixX<Scalar>>;
  using SparseHessianFunc = HessianFunc<Eigen::SparseMatrix<Scalar>>;

  // ----------------------------------------------------------------------------------------------
  // Constructors
  // ----------------------------------------------------------------------------------------------

  Factor() = default;

  /**
   * Create directly from a (sparse) hessian functor. This is the lowest-level constructor.
   *
   * Args:
   *   keys_to_func: The set of input arguments, in order, accepted by func.
   *   keys_to_optimize: The set of input arguments that correspond to the derivative in func. Must
   *                     be a subset of keys_to_func.
   */
  Factor(DenseHessianFunc hessian_func, const std::vector<Key>& keys_to_func,
         const std::vector<Key>& keys_to_optimize = {});
  Factor(SparseHessianFunc hessian_func, const std::vector<Key>& keys_to_func,
         const std::vector<Key>& keys_to_optimize = {});

  /**
   * Does this factor use a sparse jacobian/hessian matrix?
   */
  bool IsSparse() const {
    return is_sparse_;
  }

  /**
   * Create from a function that computes the (dense) jacobian. The hessian will be computed using
   * the Gauss Newton approximation:
   *    H   = J.T * J
   *    rhs = J.T * b
   *
   * Args:
   *   keys_to_func: The set of input arguments, in order, accepted by func.
   *   keys_to_optimize: The set of input arguments that correspond to the derivative in func. Must
   *                     be a subset of keys_to_func.
   */
  Factor(const DenseJacobianFunc& jacobian_func, const std::vector<Key>& keys_to_func,
         const std::vector<Key>& keys_to_optimize = {});

  /**
   * Create from a function that computes the (sparse) jacobian. The hessian will be computed using
   * the Gauss Newton approximation:
   *    H   = J.T * J
   *    rhs = J.T * b
   *
   * Args:
   *   keys_to_func: The set of input arguments, in order, accepted by func.
   *   keys_to_optimize: The set of input arguments that correspond to the derivative in func. Must
   *                     be a subset of keys_to_func.
   */
  Factor(const SparseJacobianFunc& jacobian_func, const std::vector<Key>& keys_to_func,
         const std::vector<Key>& keys_to_optimize = {});

  /**
   * Same as the above, but allows extra constant keys into the function which are not optimized.
   * For example, to pass through epsilon.
   *
   * Args:
   *   keys_to_func: The set of input arguments, in order, accepted by func.
   *   keys_to_optimize: The set of input arguments that correspond to the derivative in func. Must
   *                     be a subset of keys_to_func.
   */
  template <typename Functor>
  static Factor Jacobian(Functor&& func, const std::vector<Key>& keys_to_func,
                         const std::vector<Key>& keys_to_optimize = {});

  /**
   * Same as the above, but allows extra constant keys into the function which are not optimized.
   * For example, to pass through epsilon.
   *
   * Args:
   *   keys_to_func: The set of input arguments, in order, accepted by func.
   *   keys_to_optimize: The set of input arguments that correspond to the derivative in func. Must
   *                     be a subset of keys_to_func.
   */
  template <typename Functor>
  static Factor Hessian(Functor&& func, const std::vector<Key>& keys_to_func,
                        const std::vector<Key>& keys_to_optimize = {});

  // ----------------------------------------------------------------------------------------------
  // Linearization
  // ----------------------------------------------------------------------------------------------

  /**
   * Evaluate the factor at the given linearization point and output just the
   * numerical values of the residual.
   */
  void Linearize(const Values<Scalar>& values, VectorX<ScalarType>* residual) const;

  /**
   * Evaluate the factor at the given linearization point and output just the
   * numerical values of the residual and jacobian.
   *
   * This overload can only be called if IsSparse is false; otherwise, it will throw
   */
  void Linearize(const Values<Scalar>& values, VectorX<ScalarType>* residual,
                 MatrixX<ScalarType>* jacobian) const;

  /**
   * Evaluate the factor at the given linearization point and output just the
   * numerical values of the residual and jacobian.
   *
   * This overload can only be called if IsSparse is true; otherwise, it will throw
   */
  void Linearize(const Values<Scalar>& values, VectorX<ScalarType>* residual,
                 Eigen::SparseMatrix<ScalarType>* jacobian) const;

  /**
   * Evaluate the factor at the given linearization point and output a LinearizedDenseFactor that
   * contains the numerical values of the residual, jacobian, hessian, and right-hand-side.
   *
   * This overload can only be called if IsSparse is false; otherwise, it will throw
   */
  void Linearize(const Values<Scalar>& values, LinearizedDenseFactor* linearized_factor) const;

  /**
   * Evaluate the factor at the given linearization point and output a LinearizedDenseFactor that
   * contains the numerical values of the residual, jacobian, hessian, and right-hand-side.
   *
   * This overload can only be called if IsSparse is false; otherwise, it will throw
   */
  LinearizedDenseFactor Linearize(const Values<Scalar>& values) const;

  /**
   * Evaluate the factor at the given linearization point and output a LinearizedDenseFactor that
   * contains the numerical values of the residual, jacobian, hessian, and right-hand-side.
   *
   * This overload can only be called if IsSparse is true; otherwise, it will throw
   */
  void Linearize(const Values<Scalar>& values, LinearizedSparseFactor* linearized_factor) const;

  // ----------------------------------------------------------------------------------------------
  // Helpers
  // ----------------------------------------------------------------------------------------------

  /**
   * Get the optimized keys for this factor
   */
  const std::vector<Key>& OptimizedKeys() const;

  /**
   * Get all keys required to evaluate this factor
   */
  const std::vector<Key>& AllKeys() const;

 private:
  void EnsureIndexEntriesExist(const Values<Scalar>& values) const;

  template <typename LinearizedFactorT>
  void FillLinearizedFactorIndex(const Values<Scalar>& values,
                                 LinearizedFactorT& linearized_factor) const;

  DenseHessianFunc hessian_func_;
  SparseHessianFunc sparse_hessian_func_;
  bool is_sparse_;

  // Keys to be optimized in this factor, which must match the column order of the jacobian.
  std::vector<Key> keys_to_optimize_;

  // All keys required to evaluate the factor
  std::vector<Key> keys_;

  // Index entries for the above keys, cached to avoid repeated unordered_map lookups.
  // Values ID are used to detect structure changes.
  mutable std::pair<int64_t, std::vector<index_entry_t>> values_id_and_index_entries_{
      Values<Scalar>::kInvalidId, {}};
};

// Shorthand instantiations
using Factord = Factor<double>;
using Factorf = Factor<float>;

template <typename Scalar>
std::ostream& operator<<(std::ostream& os, const sym::Factor<Scalar>& factor);

// TODO(hayk): Why doesn't this work instead of splitting out the types?
// template <typename Scalar>
// std::ostream& operator<<(std::ostream& os,
//                          const typename sym::Factor<Scalar>::LinearizedDenseFactor& factor);
std::ostream& operator<<(std::ostream& os, const sym::linearized_dense_factor_t& factor);
std::ostream& operator<<(std::ostream& os, const sym::linearized_dense_factorf_t& factor);
std::ostream& operator<<(std::ostream& os, const sym::linearized_sparse_factor_t& factor);
std::ostream& operator<<(std::ostream& os, const sym::linearized_sparse_factorf_t& factor);

}  // namespace sym

// Template method implementations
#include "./factor.tcc"
