// -----------------------------------------------------------------------------
// This file was autogenerated by symforce from template:
//     measurements.h.jinja
// Do NOT modify by hand.
// -----------------------------------------------------------------------------

#include <vector>

#include <Eigen/Dense>

#include <sym/pose3.h>

namespace robot_3d_localization {

// Measurment values for matching factors.
extern const std::vector<std::vector<Eigen::Vector3d>> body_t_landmark_measurements;

// Measurement values for odometry factors.
extern const std::vector<sym::Pose3d> odometry_relative_pose_measurements;

// Landmark positions.
extern const std::vector<Eigen::Vector3d> landmark_positions;

}  // namespace robot_3d_localization
