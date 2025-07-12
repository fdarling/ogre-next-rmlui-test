cmake_minimum_required(VERSION 3.13)

project(OgreNextRmlUiDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/home/USERNAME/apps/ogre-next")
list(APPEND CMAKE_MODULE_PATH "/home/USERNAME/apps/ogre-next/lib/OGRE-Next/cmake/")
list(APPEND CMAKE_PREFIX_PATH "/home/USERNAME/repos/RmlUi/build")
list(APPEND CMAKE_MODULE_PATH "/home/USERNAME/repos/RmlUi/build/install/")

# set(RmlUi_STATIC true)
# set(RmlUi_DIR ${PROJECT_SOURCE_DIR}/ext/RmlUi)
set(RmlUi_DIR /home/USERNAME/repos/ext/RmlUi/build)
if(RmlUi_STATIC)
  # set(RmlUi_STATIC ${PROJECT_SOURCE_DIR}/ext/RmlUi/build/lib)
  set(RmlUi_STATIC /home/USERNAME/repos/RmlUi/build/lib)
endif()

find_package(sdl2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OGRE 3.0 REQUIRED COMPONENTS OgreMain Bites Hlms HlmsPbs HlmsUnlit RenderSystemGL3Plus)
find_package(assimp REQUIRED)
find_package(RmlUi REQUIRED)

# message("YOLO RmlUi_INCLUDE_DIRS = " ${RmlUi_INCLUDE_DIRS})

set(RMLUI_BACKEND_PATH "/home/USERNAME/repos/RmlUi/Backends")
# set(RMLUI_BACKEND_PATH ${RmlUi_SOURCE_DIR}/Backends)
add_library(rmlui_backend_SDL_GL3 INTERFACE)
target_sources(rmlui_backend_SDL_GL3 INTERFACE
	${RMLUI_BACKEND_PATH}/RmlUi_Platform_SDL.cpp
	${RMLUI_BACKEND_PATH}/RmlUi_Renderer_GL3.cpp
)
target_include_directories(rmlui_backend_SDL_GL3 INTERFACE ${RMLUI_BACKEND_PATH})
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
    OpenGL::GL
    rmlui_backend_SDL_GL3
)
