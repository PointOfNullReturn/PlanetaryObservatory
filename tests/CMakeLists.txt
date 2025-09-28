set(TEST_SOURCES
    smoke_test.cpp
)

add_executable(PlanetaryObservatoryTests ${TEST_SOURCES})

target_include_directories(PlanetaryObservatoryTests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../third_party
        ${CMAKE_CURRENT_LIST_DIR}/../src
)

target_compile_features(PlanetaryObservatoryTests PRIVATE cxx_std_23)

add_test(NAME PlanetaryObservatoryTests COMMAND PlanetaryObservatoryTests)
