# CMake policies. 0025 needs to be before project(), so putting all there.
# Use AppleClang instead of Clang on Apple
if(POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif()
# Don't treat imported targets with :: as files
if(POLICY CMP0028)
    cmake_policy(SET CMP0028 NEW)
endif()
# Enable MACOSX_RPATH by default
if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()
# Quoted variables should not be dereferenced
if(POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

find_package(Magnum REQUIRED
        Sdl2Application
        DebugTools
        GL
        MeshTools
        Primitives
        Shaders
        SceneGraph)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(MODULE_DIST_PATH "${PROJECT_SOURCE_DIR}/dist/")

add_executable(MyApplication MyApplication.cpp)

target_link_libraries(MyApplication PRIVATE
    Magnum::Application
    Magnum::DebugTools
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders
    Magnum::SceneGraph
)

if(CORRADE_TARGET_EMSCRIPTEN)
    install(TARGETS MyApplication DESTINATION ${MODULE_DIST_PATH})
    install(FILES
        MyApplication.html
        EmscriptenApplication.js
        WebApplication.css
        DESTINATION ${MODULE_DIST_PATH})
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/MyApplication.js.mem
        ${CMAKE_CURRENT_BINARY_DIR}/MyApplication.wasm
        DESTINATION ${MODULE_DIST_PATH} OPTIONAL)
endif()
