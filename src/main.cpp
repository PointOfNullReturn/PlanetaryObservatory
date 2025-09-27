#include "core/Application.h"
#include "layers/SceneLayer.h"
#include "utils/Log.h"

#include <cstdlib>
#include <exception>
#include <memory>
#include <string>

int main()
{
    try
    {
        Application application;
        application.pushLayer(std::make_unique<SceneLayer>());
        return application.run();
    }
    catch (const std::exception& ex)
    {
        Log::error(std::string("Fatal error: ") + ex.what());
    }
    catch (...)
    {
        Log::error("Fatal error: unknown exception");
    }

    return EXIT_FAILURE;
}
