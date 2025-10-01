#include "scenegraph/components/CameraComponent.h"

#include "render/OrbitCamera.h"
#include "scenegraph/SceneNode.h"

CameraComponent::CameraComponent(std::shared_ptr<OrbitCamera> camera)
    : m_camera(std::move(camera)) {}

void CameraComponent::onAttach(SceneNode &) {}

void CameraComponent::onDetach(SceneNode &) {}

void CameraComponent::onUpdate(SceneNode &, double) {}

void CameraComponent::onRender(SceneNode &) {}
