#ifndef PLANETARY_OBSERVATORY_CORE_LAYER_H
#define PLANETARY_OBSERVATORY_CORE_LAYER_H

class Application;

class Layer
{
public:
    virtual ~Layer() = default;

    virtual void onAttach(Application& application) {}
    virtual void onDetach() {}
    virtual void onUpdate(double /*deltaTime*/) {}
    virtual void onRender() {}
    virtual void onResize(int /*width*/, int /*height*/) {}
    virtual void onKey(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {}
};

#endif
