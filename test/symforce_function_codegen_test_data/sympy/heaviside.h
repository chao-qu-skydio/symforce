// -----------------------------------------------------------------------------
// This file was autogenerated by symforce. Do NOT modify by hand.
// -----------------------------------------------------------------------------
#pragma once

#include <Eigen/Dense>

namespace cpp_code_printer_test {

/**
 * This function was autogenerated from a symbolic function. Do not modify by hand.
 *
 * Symbolic function: f
 * Arg type(s): Symbol
 * Return type(s): Heaviside
 *
 */
template <typename Scalar>
Scalar Heaviside(const Scalar x) {
  // Total ops: 1

  // Input arrays

  // Intermediate terms (0)

  // Output terms (1)
  Scalar _res;

  _res = 0.5 * ((((x) >= 0) - ((x) < 0)) + 1);

  return _res;
}  // NOLINT(readability/fn_size)

// NOLINTNEXTLINE(readability/fn_size)
}  // namespace cpp_code_printer_test