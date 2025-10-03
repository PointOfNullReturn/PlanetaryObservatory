#include "render/OrbitCamera.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

namespace {
constexpr float kMinRadius = 2.0f;
constexpr float kMaxRadius = 38.0f;

float clampPitch(float value, float minPitch, float maxPitch) {
  return std::clamp(value, std::min(minPitch, maxPitch),
                    std::max(minPitch, maxPitch));
}

float wrapDegrees(float value) {
  while (value > 180.0f) {
    value -= 360.0f;
  }
  while (value <= -180.0f) {
    value += 360.0f;
  }
  return value;
}


float approach(float current, float target, float speed, float dt) {
  const float delta = target - current;
  const float step = speed * dt;
  if (std::fabs(delta) <= step) {
    return target;
  }
  return current + std::copysign(step, delta);
}

glm::vec3 approachVec3(const glm::vec3 &current, const glm::vec3 &target,
                       float speed, float dt) {
  const glm::vec3 delta = target - current;
  const float length = glm::length(delta);
  if (length <= speed * dt || length <= std::numeric_limits<float>::epsilon()) {
    return target;
  }
  return current + (delta / length) * (speed * dt);
}
} // namespace

OrbitCamera::OrbitCamera() {
  m_currentFocus = m_focus;
  recalcPosition();
}

void OrbitCamera::setLerpSpeed(float positionSpeed, float angleSpeed,
                               float radiusSpeed) {
  m_focusLerpSpeed = std::max(0.0f, positionSpeed);
  m_angleLerpSpeed = std::max(0.0f, angleSpeed);
  m_radiusLerpSpeed = std::max(0.0f, radiusSpeed);
}

void OrbitCamera::setPitchLimits(float minPitchDegrees,
                                 float maxPitchDegrees) {
  m_minPitchDeg = minPitchDegrees;
  m_maxPitchDeg = maxPitchDegrees;
  m_targetPitchDeg = clampPitch(m_targetPitchDeg, m_minPitchDeg, m_maxPitchDeg);
  m_currentPitchDeg =
      clampPitch(m_currentPitchDeg, m_minPitchDeg, m_maxPitchDeg);
}

void OrbitCamera::setFocus(const Focus &focus, bool snapInstantly) {
  m_focus = focus;
  if (snapInstantly) {
    m_currentFocus = focus;
    recalcPosition();
  }
}

void OrbitCamera::setAngles(float yawDegrees, float pitchDegrees, bool snap) {
  m_targetYawDeg = wrapDegrees(yawDegrees);
  m_targetPitchDeg =
      clampPitch(pitchDegrees, m_minPitchDeg, m_maxPitchDeg);
  if (snap) {
    m_currentYawDeg = m_targetYawDeg;
    m_currentPitchDeg = m_targetPitchDeg;
    recalcPosition();
  }
}

void OrbitCamera::snapAngles(float yawDegrees, float pitchDegrees) {
  setAngles(yawDegrees, pitchDegrees, true);
}

void OrbitCamera::orbit(float deltaYawDegrees, float deltaPitchDegrees) {
  setAngles(m_targetYawDeg + deltaYawDegrees,
            m_targetPitchDeg + deltaPitchDegrees, false);
}

void OrbitCamera::zoom(float deltaRadius) {
  setRadius(m_targetRadius + deltaRadius, false);
}

void OrbitCamera::setRadius(float radius, bool snap) {
  m_targetRadius = std::clamp(radius, kMinRadius, kMaxRadius);
  if (snap) {
    m_currentRadius = m_targetRadius;
    recalcPosition();
  }
}

void OrbitCamera::update(double deltaSeconds) {
  const float dt = static_cast<float>(deltaSeconds);

  m_currentYawDeg = wrapDegrees(
      approach(m_currentYawDeg, m_targetYawDeg, m_angleLerpSpeed, dt));
  m_currentPitchDeg = approach(m_currentPitchDeg, m_targetPitchDeg,
                               m_angleLerpSpeed, dt);
  m_currentPitchDeg =
      clampPitch(m_currentPitchDeg, m_minPitchDeg, m_maxPitchDeg);
  m_currentRadius =
      approach(m_currentRadius, m_targetRadius, m_radiusLerpSpeed, dt);

  m_currentFocus.position =
      approachVec3(m_currentFocus.position, m_focus.position,
                   m_focusLerpSpeed, dt);
  if (m_focus.preferredRadius > 0.0f) {
    m_currentRadius =
        approach(m_currentRadius, m_focus.preferredRadius, m_radiusLerpSpeed, dt);
    m_targetRadius = m_focus.preferredRadius;
  }

  recalcPosition();
}

void OrbitCamera::recalcPosition() {
  const float yawRad = glm::radians(m_currentYawDeg);
  const float pitchRad = glm::radians(m_currentPitchDeg);
  const float cosPitch = std::cos(pitchRad);
  const float sinPitch = std::sin(pitchRad);
  const float cosYaw = std::cos(yawRad);
  const float sinYaw = std::sin(yawRad);

  glm::vec3 offset;
  offset.x = m_currentRadius * cosPitch * cosYaw;
  offset.y = m_currentRadius * sinPitch;
  offset.z = m_currentRadius * cosPitch * sinYaw;

  m_position = m_currentFocus.position + offset;
}

glm::mat4 OrbitCamera::viewMatrix() const {
  return glm::lookAt(m_position, m_currentFocus.position, up());
}

glm::vec3 OrbitCamera::up() const {
  const float pitchRad = glm::radians(m_currentPitchDeg);
  if (std::fabs(pitchRad) >= glm::half_pi<float>()) {
    return glm::vec3(0.0f, 0.0f, (pitchRad > 0.0f) ? -1.0f : 1.0f);
  }
  return glm::vec3(0.0f, 1.0f, 0.0f);
}

