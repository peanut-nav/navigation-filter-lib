/**
 * @file MathUtils.hpp
 * @brief Mathematical utilities for navigation computations
 *
 * Provides essential mathematical functions and conversions used in navigation systems,
 * including angle conversions, direction cosine matrix computation, and attitude quaternion generation.
 *
 * @author peanut-nav
 * @date Created: 2025-07-22
 * @last Modified: 2025-07-22
 * @version 0.1
 */

#pragma once
#include <cmath>
#include <Eigen/Dense>

namespace NavigationUtils {

/**
 * @brief Degrees to radians conversion constant
 */
constexpr double DEG_TO_RAD = M_PI / 180.0;

/**
 * @brief Convert degrees to radians
 * @tparam T Numeric type (float/double)
 * @param deg Angle in degrees
 * @return Angle in radians
 */
template <typename T>
T deg2rad(T deg) {
    return deg * static_cast<T>(DEG_TO_RAD);
}

/**
 * @brief Compute Direction Cosine Matrix (body to navigation frame) using Z-X-Y rotation sequence
 * 
 * Computes the rotation matrix from body frame to navigation frame using
 * Z-X-Y Euler angle sequence (yaw, roll, pitch).
 *
 * @tparam T Numeric type (float/double)
 * @param roll_deg Roll angle (degrees)
 * @param pitch_deg Pitch angle (degrees)
 * @param yaw_deg Yaw angle (degrees)
 * @return 3x3 Direction Cosine Matrix
 */
template <typename T>
Eigen::Matrix<T, 3, 3> bodyToNavigationDCM(T roll_deg, T pitch_deg, T yaw_deg) {
    // Convert input angles from degrees to radians
    const T roll_rad = deg2rad(roll_deg);
    const T pitch_rad = deg2rad(pitch_deg);
    const T yaw_rad = deg2rad(yaw_deg);
    
    // Precompute trigonometric values for efficiency
    const T sin_roll = std::sin(roll_rad);
    const T cos_roll = std::cos(roll_rad);
    const T sin_pitch = std::sin(pitch_rad);
    const T cos_pitch = std::cos(pitch_rad);
    const T sin_yaw = std::sin(yaw_rad);
    const T cos_yaw = std::cos(yaw_rad);
    
    // Initialize output matrix
    Eigen::Matrix<T, 3, 3> dcm;
    
    // First row elements
    dcm(0, 0) = cos_pitch * cos_yaw - sin_pitch * sin_roll * sin_yaw;
    dcm(0, 1) = cos_pitch * sin_yaw + sin_pitch * sin_roll * cos_yaw;
    dcm(0, 2) = -sin_pitch * cos_roll;
    
    // Second row elements
    dcm(1, 0) = -cos_roll * sin_yaw;
    dcm(1, 1) = cos_roll * cos_yaw;
    dcm(1, 2) = sin_roll;
    
    // Third row elements
    dcm(2, 0) = sin_pitch * cos_yaw + cos_pitch * sin_roll * sin_yaw;
    dcm(2, 1) = sin_pitch * sin_yaw - cos_pitch * sin_roll * cos_yaw;
    dcm(2, 2) = cos_pitch * cos_roll;
    
    return dcm;
}

/**
 * @brief Compute attitude quaternion from Euler angles
 * 
 * Constructs a normalized attitude quaternion from Euler angles.
 *
 * @tparam T Numeric type (float/double)
 * @param roll_deg Roll angle (degrees)
 * @param pitch_deg Pitch angle (degrees)
 * @param yaw_deg Yaw angle (degrees)
 * @return 4x1 attitude quaternion [w, x, y, z]
 */
template <typename T>
Eigen::Matrix<T, 4, 1> eulerToQuaternion(T roll_deg, T pitch_deg, T yaw_deg) {
    // Convert angles to radians and halve for quaternion conversion
    const T roll_half = deg2rad(roll_deg) * static_cast<T>(0.5);
    const T pitch_half = deg2rad(pitch_deg) * static_cast<T>(0.5);
    const T yaw_half = deg2rad(yaw_deg) * static_cast<T>(0.5);
    
    // Precompute trigonometric values for efficiency
    const T cos_yaw_half = std::cos(yaw_half);
    const T sin_yaw_half = std::sin(yaw_half);
    const T cos_roll_half = std::cos(roll_half);
    const T sin_roll_half = std::sin(roll_half);
    const T cos_pitch_half = std::cos(pitch_half);
    const T sin_pitch_half = std::sin(pitch_half);
    
    // Initialize quaternion vector
    Eigen::Matrix<T, 4, 1> quat;
    
    // Compute quaternion components using direct conversion formula
    quat(0) = cos_yaw_half * cos_roll_half * cos_pitch_half - sin_yaw_half * sin_roll_half * sin_pitch_half;
    quat(1) = cos_yaw_half * sin_roll_half * cos_pitch_half - sin_yaw_half * cos_roll_half * sin_pitch_half;
    quat(2) = cos_yaw_half * cos_roll_half * sin_pitch_half + sin_yaw_half * sin_roll_half * cos_pitch_half;
    quat(3) = cos_yaw_half * sin_roll_half * sin_pitch_half + sin_yaw_half * cos_roll_half * cos_pitch_half;
    
    // Return normalized quaternion
    quat.normalize();
    return quat;
}

} // namespace NavigationUtils
