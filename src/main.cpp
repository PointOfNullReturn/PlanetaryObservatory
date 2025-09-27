#include "core/Application.h"
#include "layers/SceneLayer.h"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>

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
        std::cerr << "Fatal error: " << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Fatal error: unknown exception" << std::endl;
    }

    return EXIT_FAILURE;
}
