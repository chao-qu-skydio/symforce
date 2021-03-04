//  -----------------------------------------------------------------------------
// This file was autogenerated by symforce. Do NOT modify by hand.
// -----------------------------------------------------------------------------
#pragma once

#include <ostream>
#include <random>
#include <vector>

#include <Eigen/Dense>

#include "./ops/group_ops.h"
#include "./ops/lie_group_ops.h"
#include "./ops/storage_ops.h"
#include "./rot2.h"

namespace geo {

/**
 * Autogenerated C++ implementation of <class 'symforce.geo.pose2.Pose2'>.
 *
 * Group of two-dimensional rigid body transformations - SE(2).
 *
 * The storage space is a complex (real, imag) for rotation followed by a position (x, y).
 *
 * The tangent space is two elements for translation followed by one angle for rotation.
 * TODO(hayk): Flip this to match Pose3 with rotation first.
 */
template <typename ScalarType>
class Pose2 {
 public:
  // Typedefs
  using Scalar = ScalarType;
  using Self = Pose2<Scalar>;
  using DataVec = Eigen::Matrix<Scalar, 4, 1>;
  using TangentVec = Eigen::Matrix<Scalar, 3, 1>;
  using SelfJacobian = Eigen::Matrix<Scalar, 3, 3>;

  // Construct from data vec
  explicit Pose2(const DataVec& data) : data_(data) {}

  // Default construct to identity
  Pose2() : Pose2(GroupOps<Self>::Identity()) {}

  // Access underlying storage as const
  inline const DataVec& Data() const {
    return data_;
  }

  // --------------------------------------------------------------------------
  // StorageOps concept
  // --------------------------------------------------------------------------

  static constexpr int32_t StorageDim() {
    return StorageOps<Self>::StorageDim();
  }

  void ToStorage(Scalar* const vec) const {
    return StorageOps<Self>::ToStorage(*this, vec);
  }

  static Pose2 FromStorage(const Scalar* const vec) {
    return StorageOps<Self>::FromStorage(vec);
  }

  // --------------------------------------------------------------------------
  // GroupOps concept
  // --------------------------------------------------------------------------

  static Self Identity() {
    return GroupOps<Self>::Identity();
  }

  Self Inverse() const {
    return GroupOps<Self>::Inverse(*this);
  }

  Self Compose(const Self& b) const {
    return GroupOps<Self>::Compose(*this, b);
  }

  Self Between(const Self& b) const {
    return GroupOps<Self>::Between(*this, b);
  }

  Self InverseWithJacobian(SelfJacobian* const res_D_a = nullptr) const {
    return GroupOps<Self>::InverseWithJacobian(*this, res_D_a);
  }

  Self ComposeWithJacobians(const Self& b, SelfJacobian* const res_D_a = nullptr,
                            SelfJacobian* const res_D_b = nullptr) const {
    return GroupOps<Self>::ComposeWithJacobians(*this, b, res_D_a, res_D_b);
  }

  Self BetweenWithJacobians(const Self& b, SelfJacobian* const res_D_a = nullptr,
                            SelfJacobian* const res_D_b = nullptr) const {
    return GroupOps<Self>::BetweenWithJacobians(*this, b, res_D_a, res_D_b);
  }

  // Compose shorthand
  template <typename Other>
  auto operator*(const Other& b) const -> decltype(Compose(b)) {
    return Compose(b);
  }

  // --------------------------------------------------------------------------
  // LieGroupOps concept
  // --------------------------------------------------------------------------

  static constexpr int32_t TangentDim() {
    return LieGroupOps<Self>::TangentDim();
  }

  static Self FromTangent(const TangentVec& vec, const Scalar epsilon = 1e-8f) {
    return LieGroupOps<Self>::FromTangent(vec, epsilon);
  }

  TangentVec ToTangent(const Scalar epsilon = 1e-8f) const {
    return LieGroupOps<Self>::ToTangent(*this, epsilon);
  }

  Self Retract(const TangentVec& vec, const Scalar epsilon = 1e-8f) const {
    return LieGroupOps<Self>::Retract(*this, vec, epsilon);
  }

  TangentVec LocalCoordinates(const Self& b, const Scalar epsilon = 1e-8f) const {
    return LieGroupOps<Self>::LocalCoordinates(*this, b, epsilon);
  }

  // --------------------------------------------------------------------------
  // General Helpers
  // --------------------------------------------------------------------------

  bool IsApprox(const Self& b, const Scalar tol) const {
    // isApprox is multiplicative so we check the norm for the exact zero case
    // https://eigen.tuxfamily.org/dox/classEigen_1_1DenseBase.html#ae8443357b808cd393be1b51974213f9c
    if (b.Data() == DataVec::Zero()) {
      return Data().norm() < tol;
    }

    return Data().isApprox(b.Data(), tol);
  }

  template <typename ToScalar>
  Pose2<ToScalar> Cast() const {
    return Pose2<ToScalar>(Data().template cast<ToScalar>());
  }

  bool operator==(const Pose2& rhs) const {
    return data_ == rhs.Data();
  }

  // Matrix type aliases
  using Vector2 = Eigen::Matrix<Scalar, 2, 1>;

  // Included from "custom_methods/pose2.h.jinja":
  // --------------------------------------------------------------------------
  // Handwritten methods for Pose2
  // --------------------------------------------------------------------------

  Pose2(const Rot2<Scalar>& rotation, const Vector2& position) {
    data_.template head<2>() = rotation.Data();
    data_.template tail<2>() = position;
  }

  Rot2<Scalar> Rotation() const {
    return Rot2<Scalar>(data_.template head<2>());
  }

  Vector2 Position() const {
    return data_.template tail<2>();
  }

  // TODO(hayk): Could codegen this.
  Vector2 Compose(const Vector2& point) const {
    return Rotation() * point + Position();
  }

  // --------------------------------------------------------------------------
  // Custom generated methods
  // --------------------------------------------------------------------------

  Vector2 InverseCompose(const Vector2& point) const;

 protected:
  DataVec data_;
};

// Shorthand for scalar types
using Pose2d = Pose2<double>;
using Pose2f = Pose2<float>;

}  // namespace geo

// Externs to reduce duplicate instantiation
extern template class geo::Pose2<double>;
extern template class geo::Pose2<float>;

// Print definitions
std::ostream& operator<<(std::ostream& os, const geo::Pose2<double>& a);
std::ostream& operator<<(std::ostream& os, const geo::Pose2<float>& a);

// Concept implementations for this class (include order matters here)
// clang-format off
#include "./ops/pose2/storage_ops.h"
#include "./ops/pose2/lie_group_ops.h"
#include "./ops/pose2/group_ops.h"
// clang-format on
