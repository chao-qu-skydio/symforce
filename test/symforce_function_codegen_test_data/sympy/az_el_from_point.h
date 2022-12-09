// -----------------------------------------------------------------------------
// This file was autogenerated by symforce from template:
//     function/FUNCTION.h.jinja
// Do NOT modify by hand.
// -----------------------------------------------------------------------------

#pragma once

#include <Eigen/Dense>

#include <sym/pose3.h>

namespace sym {

/**
 * Transform a nav point into azimuth / elevation angles in the
 * camera frame.
 *
 * Args:
 *     nav_T_cam (sf.Pose3): camera pose in the world
 *     nav_t_point (sf.Matrix): nav point
 *     epsilon (Scalar): small number to avoid singularities
 *
 * Returns:
 *     sf.Matrix: (azimuth, elevation)
 */
template <typename Scalar>
Eigen::Matrix<Scalar, 2, 1> AzElFromPoint(const sym::Pose3<Scalar>& nav_T_cam,
                                          const Eigen::Matrix<Scalar, 3, 1>& nav_t_point,
                                          const Scalar epsilon) {
  // Total ops: 91

  // Input arrays
  const Eigen::Matrix<Scalar, 7, 1>& _nav_T_cam = nav_T_cam.Data();

  // Intermediate terms (23)
  const Scalar _tmp0 = 2 * _nav_T_cam[3];
  const Scalar _tmp1 = _nav_T_cam[2] * _tmp0;
  const Scalar _tmp2 = 2 * _nav_T_cam[0] * _nav_T_cam[1] - _tmp1;
  const Scalar _tmp3 = _nav_T_cam[0] * _tmp0;
  const Scalar _tmp4 = 2 * _nav_T_cam[1];
  const Scalar _tmp5 = _nav_T_cam[2] * _tmp4 + _tmp3;
  const Scalar _tmp6 = 2 * std::pow(_nav_T_cam[0], Scalar(2));
  const Scalar _tmp7 = 2 * std::pow(_nav_T_cam[2], Scalar(2)) - 1;
  const Scalar _tmp8 = -_tmp6 - _tmp7;
  const Scalar _tmp9 = -_nav_T_cam[4] * _tmp2 - _nav_T_cam[5] * _tmp8 - _nav_T_cam[6] * _tmp5 +
                       _tmp2 * nav_t_point(0, 0) + _tmp5 * nav_t_point(2, 0) +
                       _tmp8 * nav_t_point(1, 0);
  const Scalar _tmp10 = _nav_T_cam[0] * _tmp4 + _tmp1;
  const Scalar _tmp11 = _nav_T_cam[1] * _tmp0;
  const Scalar _tmp12 = 2 * _nav_T_cam[0] * _nav_T_cam[2] - _tmp11;
  const Scalar _tmp13 = _nav_T_cam[5] * _tmp10;
  const Scalar _tmp14 = _nav_T_cam[6] * _tmp12;
  const Scalar _tmp15 = 2 * std::pow(_nav_T_cam[1], Scalar(2));
  const Scalar _tmp16 = -_tmp15 - _tmp7;
  const Scalar _tmp17 = _nav_T_cam[4] * _tmp16;
  const Scalar _tmp18 = _tmp10 * nav_t_point(1, 0) + _tmp12 * nav_t_point(2, 0) - _tmp13 - _tmp14 +
                        _tmp16 * nav_t_point(0, 0) - _tmp17;
  const Scalar _tmp19 = 2 * _nav_T_cam[0] * _nav_T_cam[2] + _tmp11;
  const Scalar _tmp20 = 2 * _nav_T_cam[1] * _nav_T_cam[2] - _tmp3;
  const Scalar _tmp21 = -_tmp15 - _tmp6 + 1;
  const Scalar _tmp22 = -_nav_T_cam[4] * _tmp19 - _nav_T_cam[5] * _tmp20 - _nav_T_cam[6] * _tmp21 +
                        _tmp19 * nav_t_point(0, 0) + _tmp20 * nav_t_point(1, 0) +
                        _tmp21 * nav_t_point(2, 0);

  // Output terms (1)
  Eigen::Matrix<Scalar, 2, 1> _res;

  _res(0, 0) = std::atan2(_tmp9, _tmp10 * nav_t_point(1, 0) + _tmp12 * nav_t_point(2, 0) - _tmp13 -
                                     _tmp14 + _tmp16 * nav_t_point(0, 0) - _tmp17 +
                                     epsilon * ((((_tmp18) > 0) - ((_tmp18) < 0)) + Scalar(0.5)));
  _res(1, 0) = -std::acos(_tmp22 / std::sqrt(Scalar(std::pow(_tmp18, Scalar(2)) +
                                                    std::pow(_tmp22, Scalar(2)) +
                                                    std::pow(_tmp9, Scalar(2)) + epsilon))) +
               Scalar(M_PI_2);

  return _res;
}  // NOLINT(readability/fn_size)

// NOLINTNEXTLINE(readability/fn_size)
}  // namespace sym
