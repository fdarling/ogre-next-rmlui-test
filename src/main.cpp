#define ENABLE_RMLUI_CONTEXT

#include "FPSGame.h"
#include "GUI.h"

#include <OgreRoot.h>
#include <OgreFrameStats.h>

#include <RmlUi/Core/EventListener.h>
#include <RmlUi/Core/ElementDocument.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>

#include <cstdlib> // for EXIT_FAILURE and EXIT_SUCCESS

static const int WINDOW_WIDTH = 1280;
static const int WINDOW_HEIGHT = 720;

class ResetEventListener : public Rml::EventListener
{
public:
    ResetEventListener() : resetClicked(false)
    {
    }
    void ProcessEvent(Rml::Event &event) override
    {
        std::cout << "Reset clicked!" << std::endl;
        resetClicked = true;
        Ogre::Root * const root = Ogre::Root::getSingletonPtr();
        root->resetFrameStats();
    }
public:
    bool resetClicked;
};

static int mainBody(int argc, const char *argv[])
{
    // create window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1); // TODO explore the usefulness of this option
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window(
        SDL_CreateWindow(
            "FPSGame",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        ),
        SDL_DestroyWindow
    );
    if (!window)
    {
        fprintf(stderr, "error: SDL_CreateWindow() failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // NOTE: SDL_GLContext is already a pointer, you aren't supposed to use pointers like "SDL_GLContext *", and this is inconsistent with other SDL APIs... we have to remove the pointer aspect to make it play nice with std::unique_ptr<>

    // create an OpenGL context for RMLUI
#ifdef ENABLE_RMLUI_CONTEXT
    std::unique_ptr<std::remove_pointer<SDL_GLContext>::type, decltype(&SDL_GL_DeleteContext)> rmluiContext(SDL_GL_CreateContext(window.get()), SDL_GL_DeleteContext);
    if (!rmluiContext)
    {
        fprintf(stderr, "error: SDL_GL_CreateContext() failed for RMLUI: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
#endif // ENABLE_RMLUI_CONTEXT

    // create an OpenGL context for OGRE
    std::unique_ptr<std::remove_pointer<SDL_GLContext>::type, decltype(&SDL_GL_DeleteContext)> ogreContext(SDL_GL_CreateContext(window.get()), SDL_GL_DeleteContext);
    if (!ogreContext)
    {
        fprintf(stderr, "error: SDL_GL_CreateContext() failed for OGRE: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_GL_MakeCurrent(window.get(), ogreContext.get());
    FPSGame game(window.get());
    if (!game.getWindow())
        return EXIT_FAILURE;

#ifdef ENABLE_RMLUI_CONTEXT
    SDL_GL_MakeCurrent(window.get(), rmluiContext.get());
#endif // ENABLE_RMLUI_CONTEXT
    GUI::GUI gui(window.get());
    ResetEventListener resetEventListener;
    if (Rml::ElementDocument * const document = gui.getFrameStatsDocument())
    {
        if (Rml::Element * const element = gui.getFrameStatsDocument()->GetElementById("resetButton"))
            element->AddEventListener(Rml::EventId::Click, &resetEventListener);
    }

    Uint64 old_ticks = SDL_GetTicks64();

    bool showingGui = true;
    int guiMouseX = 0, guiMouseY = 0;
    // SDL_ShowCursor(SDL_DISABLE); // TODO maybe move this further up?
    while (
        !game.getQuit() &&
        !gui.getQuit() &&
        !SDL_QuitRequested())
    {
        // calculate time delta
        Uint64 new_ticks = SDL_GetTicks64();
        const float seconds_elapsed = static_cast<float>(new_ticks - old_ticks)/1000.0f;
        old_ticks = new_ticks;

        // process input / window events
        SDL_Event event;
        while (SDL_PollEvent(&event) &&
            !game.getQuit() &&
            !gui.getQuit() &&
            !SDL_QuitRequested())
        {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB)
            {
                if (showingGui)
                {
                    SDL_GetMouseState(&guiMouseX, &guiMouseY);
                }
                showingGui = !showingGui;
                SDL_SetRelativeMouseMode(!showingGui ? SDL_TRUE : SDL_FALSE);
                if (showingGui)
                {
                    // SDL_ShowCursor(SDL_DISABLE);
                    SDL_WarpMouseInWindow(window.get(), guiMouseX, guiMouseY);
                }
                if (Rml::ElementDocument * const mainMenuDoc = gui.getMainMenuDocument())
                {
                    if (showingGui)
                        mainMenuDoc->Show();
                    else
                        mainMenuDoc->Hide();
                }
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && (event.key.keysym.mod & KMOD_CTRL))
            {
                const bool wasFullscreen = SDL_GetWindowFlags(window.get()) & SDL_WINDOW_FULLSCREEN;
                const int displayIndex = SDL_GetWindowDisplayIndex(window.get());
                SDL_DisplayMode desktopDisplayMode;
                if (SDL_GetDesktopDisplayMode(displayIndex, &desktopDisplayMode) == 0)
                {
                    SDL_SetWindowDisplayMode(window.get(), &desktopDisplayMode);
                }
                SDL_SetWindowDisplayMode(window.get(), &desktopDisplayMode);
                SDL_SetWindowFullscreen(window.get(), wasFullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
            }
            else if (showingGui)
            {
                gui.handleEvent(event);
                if (event.type == SDL_WINDOWEVENT)
                    game.handleEvent(event);
            }
            else
            {
                game.handleEvent(event);
            }
        }

        game.advance(seconds_elapsed);
        //if (showingGui) // TODO why do things go strangely if I don't guard this?
        {
            static int counter;
            counter++;
            if (counter == 20)
            {
                counter = 0;
                GUI::FrameStatData &data = gui.getFrameStatData();
                Rml::DataModelHandle model = gui.getFrameStatModel();
                Ogre::Root * const root = Ogre::Root::getSingletonPtr();
                // root->resetFrameStats();
                const Ogre::FrameStats * const frameStats = root->getFrameStats();
                const Ogre::RenderingMetrics &renderingMetrics = root->getRenderSystem()->getMetrics();
                const float fps = frameStats->getRollingAverageFps();
                const float avgTime = (frameStats->getRollingAverage()*1000.0);
                const float bestTime = (frameStats->getBestTime()*1000.0);
                const float worstTime = (frameStats->getWorstTime()*1000.0);
                data.fps = fps;
                data.avgTime = avgTime;
                data.bestTime = bestTime;
                data.worstTime = worstTime;
                data.faceCount = renderingMetrics.mFaceCount;
                data.vertexCount = renderingMetrics.mVertexCount;
                if (model)
                    model.DirtyAllVariables();
            }
        }
        gui.advance(seconds_elapsed);

        // SDL_GL_MakeCurrent(window.get(), ogreContext.get());
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        SDL_GL_MakeCurrent(window.get(), ogreContext.get());
        game.draw();
        // if (showingGui)
        {
#ifdef ENABLE_RMLUI_CONTEXT
            SDL_GL_MakeCurrent(window.get(), rmluiContext.get());
#endif // ENABLE_RMLUI_CONTEXT
            gui.draw();
        }
        SDL_GL_SwapWindow(window.get());
    }

    return EXIT_SUCCESS;
}

int main(int argc, const char *argv[])
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "error: SDL_Init() failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // run the rest of the app that relies on SDL
    const int result = mainBody(argc, argv);

    // shut down SDL and exit the application
    SDL_Quit();
    return result;
}
