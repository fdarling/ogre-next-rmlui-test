cmake_minimum_required(VERSION 3.13)

project(OgreNextRmlUiDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/home/USERNAME/apps/ogre-next")
list(APPEND CMAKE_MODULE_PATH "/home/USERNAME/apps/ogre-next/lib/OGRE-Next/cmake/")
list(APPEND CMAKE_PREFIX_PATH "/home/USERNAME/apps/RmlUi/")
list(APPEND CMAKE_MODULE_PATH "/home/USERNAME/apps/RmlUi/lib/cmake/")

find_package(sdl2 REQUIRED) # NOTE: this might need to be capitalized as "SDL2" in some circumstances
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OGRE 3.0 REQUIRED COMPONENTS OgreMain Bites Hlms HlmsPbs HlmsUnlit RenderSystemGL3Plus)
find_package(assimp REQUIRED)
find_package(RmlUi QUIET) # QUIET instead of REQUIRED as we are only looking for an *external* package at this point

# NOTE: the following lines are currently necessary due to RmlUi trying to pull in SDL1 libraries instead of SDL2. I believe it can support SDL3, so it would an issue there too.
# see https://github.com/mikke89/RmlUi/blob/a72f0809c4b40dc0f1cbf2e55ce0e8d70a997e99/Backends/CMakeLists.txt#L73
add_library(SDL::SDL ALIAS SDL2::SDL2)
add_library(SDL_image::SDL_image ALIAS SDL2_image::SDL2_image)

# determine if we are using an external RmlUi package, or our included submodule
# NOTE: currently we still need the submodule to be checked out, because we use the backend from it!
if(RmlUi_FOUND) # check to see if we are using an installed RmlUi...
    # set(RMLUI_BACKEND_PATH ${RmlUi_SOURCE_DIR}/Backends) # we don't have access to the sourcecode directory for an installed RmlUi :-(
    set(RMLUI_BACKEND_PATH "${PROJECT_SOURCE_DIR}/ext/RmlUi/Backends") # we can use our submodule though!
    add_library(rmlui_backend_SDL_GL3 INTERFACE)
    target_sources(rmlui_backend_SDL_GL3 INTERFACE
        ${RMLUI_BACKEND_PATH}/RmlUi_Platform_SDL.cpp
        ${RMLUI_BACKEND_PATH}/RmlUi_Renderer_GL3.cpp
    )
    target_include_directories(rmlui_backend_SDL_GL3 INTERFACE ${RMLUI_BACKEND_PATH})
else() # use our submodule RmlUi instead...
    set(RMLUI_BACKEND SDL_GL3)
    # https://github.com/Conquer-Space/Conquer-Space/blob/288a66fb6bbf4305956be66e07c4345986675f76/lib/CMakeLists.txt#L40C1-L42
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    # set(RMLUI_SVG_PLUGIN ON CACHE BOOL "" FORCE)
    add_subdirectory("${PROJECT_SOURCE_DIR}/ext/RmlUi")
    add_subdirectory("${PROJECT_SOURCE_DIR}/ext/RmlUi/Backends")
endif()

target_compile_definitions(rmlui_backend_SDL_GL3 INTERFACE
	RMLUI_SDL_VERSION_MAJOR=2
	RMLUI_NUM_MSAA_SAMPLES=4
#	RMLUI_GL3_CUSTOM_LOADER=<glad/gl.h>
#	 RMLUI_GL3_CUSTOM_LOADER=<GL/glew.h>
)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${OGRE_INCLUDE_DIRS}
    ${OGRE_INCLUDE_DIRS}/OGRE-Next/Hlms/Common
    ${OGRE_HlmsPbs_INCLUDE_DIRS}
    ${OGRE_HlmsUnlit_INCLUDE_DIRS}
    ${assimp_INCLUDE_DIRS}
)
link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${OGRE_LIBRARY_DIRS}
)

add_executable(${PROJECT_NAME}
    src/ShellFileInterface.cpp
    src/SceneLoader.cpp
    src/GUI.cpp
    src/FPSGame.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${OGRE_HlmsPbs_LIBRARIES}
    ${OGRE_HlmsUnlit_LIBRARIES}
    assimp::assimp
    RmlUi::RmlUi
    rmlui_backend_SDL_GL3
    OpenGL::GL
)
