#ifndef PLANETARY_OBSERVATORY_LAYERS_SCENELAYER_H
#define PLANETARY_OBSERVATORY_LAYERS_SCENELAYER_H

#include "core/Layer.h"
#include "scene/Scene.h"
#include "scenegraph/SceneGraph.h"

#include <memory>

class Application;

class SceneLayer : public Layer {
public:
  SceneLayer();
  ~SceneLayer() override = default;

  void onAttach(Application &application) override;
  void onDetach() override;
  void onUpdate(double deltaTime) override;
  void onRender() override;
  void onImGuiRender();
  void onResize(int width, int height) override;
  void onKey(int key, int scancode, int action, int mods) override;

private:
  void updateProjection(int width, int height);
  void handleCharacterInput(char key);

  Application *m_application = nullptr;
  std::unique_ptr<Scene> m_scene;
  std::unique_ptr<SceneGraph> m_sceneGraph;
  double m_accumulator = 0.0;
  const double m_animationIntervalSeconds = 1.0 / 30.0;
};

#endif
