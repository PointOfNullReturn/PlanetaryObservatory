#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

/// Orbit-style camera that orbits around a focus point with clamped yaw/pitch.
class OrbitCamera {
public:
  struct Focus {
    glm::vec3 position{0.0f};
    float preferredRadius{5.0f};
  };

  OrbitCamera();

  /// Updates internal camera state for this frame.
  void update(double deltaSeconds);

  /// Sets the target focus the camera should orbit around.
  void setFocus(const Focus &focus, bool snapInstantly = false);

  /// Smoothly set yaw/pitch in degrees.
  void setAngles(float yawDegrees, float pitchDegrees, bool snap = false);

  /// Immediately snap to yaw/pitch.
  void snapAngles(float yawDegrees, float pitchDegrees);

  /// Adjust yaw/pitch incrementally (e.g. from input).
  void orbit(float deltaYawDegrees, float deltaPitchDegrees);

  /// Adjust radius incrementally.
  void zoom(float deltaRadius);

  /// Directly set orbit radius.
  void setRadius(float radius, bool snap = false);

  /// Returns the current orbit radius.
  float radius() const { return m_currentRadius; }

  glm::mat4 viewMatrix() const;
  glm::vec3 position() const { return m_position; }
  glm::vec3 target() const { return m_focus.position; }
  glm::vec3 focusPosition() const { return m_currentFocus.position; }
  float yawDegrees() const { return m_currentYawDeg; }
  float pitchDegrees() const { return m_currentPitchDeg; }
  glm::vec3 up() const;

  /// Controls for interpolation behaviour.
  void setLerpSpeed(float positionSpeed, float angleSpeed, float radiusSpeed);
  void setPitchLimits(float minPitchDegrees, float maxPitchDegrees);

private:
  void recalcPosition();

  Focus m_focus{};

  float m_currentYawDeg = 270.0f;
  float m_currentPitchDeg = 0.0f;
  float m_currentRadius = 5.0f;

  float m_targetYawDeg = m_currentYawDeg;
  float m_targetPitchDeg = m_currentPitchDeg;
  float m_targetRadius = m_currentRadius;

  float m_minPitchDeg = -89.0f;
  float m_maxPitchDeg = 89.0f;

  float m_angleLerpSpeed = 6.0f;   // degrees per second responsiveness
  float m_radiusLerpSpeed = 6.0f;  // units per second responsiveness
  float m_focusLerpSpeed = 4.0f;   // units per second responsiveness

  glm::vec3 m_position{0.0f};
  Focus m_currentFocus{};
};
