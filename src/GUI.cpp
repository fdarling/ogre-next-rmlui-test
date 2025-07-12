#include "GUI.h"
#include "ShellFileInterface.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>

#include <RmlUi_Platform_SDL.h>
#include <RmlUi_Renderer_GL3.h>

#include <iostream>

GUI::GUI(SDL_Window *window) : mQuit(false), mWindow(window), mSystemInterface(nullptr), mRenderInterface(nullptr)
{
    // SDL_GL_MakeCurrent(window, ceguiContext);
    int w = 0, h = 0;
    SDL_GetWindowSize(window, &w, &h);
    // std::cout << "Window size: " << w << "x" << h << std::endl;

    RmlGL3::Initialize();
    SystemInterface_SDL * const sdlInterface = new SystemInterface_SDL();
    mSystemInterface = sdlInterface;
    sdlInterface->SetWindow(window);
    RenderInterface_GL3 * const renderInterface = new RenderInterface_GL3();
    mRenderInterface = renderInterface;
    renderInterface->SetViewport(w, h);

    Rml::SetSystemInterface(mSystemInterface);
    Rml::SetRenderInterface(mRenderInterface);
    Rml::SetFileInterface(new ShellFileInterface("../"));
    Rml::Initialise();
    mContext = Rml::CreateContext("main", Rml::Vector2i(w, h));
    Rml::Debugger::Initialise(mContext);
    // Rml::Debugger::SetVisible(true);

    Rml::LoadFontFace("assets/LatoLatin-Regular.ttf");

    Rml::ElementDocument * const document = mContext->LoadDocument("data/ui.rml");
    if (document)
        document->Show();
    else
        std::cout << "ERROR: couldn't load the RmlUi document!" << std::endl;
}

GUI::~GUI()
{
    Rml::RemoveContext(mContext->GetName()); // NOTE: not really necessary, Rml::Shutdown() will take care of it anyways
    Rml::Shutdown();
    // if (mContext)
        // delete mContext;
    if (mRenderInterface)
        delete mRenderInterface;
    if (mSystemInterface)
        delete mSystemInterface;
}

void GUI::advance(float seconds_elapsed)
{
    mContext->Update();
}

void GUI::handleEvent(const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F8)
    {
        toggleDebug();
    }
    else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
    {
        const Rml::Vector2i dimensions = {event.window.data1, event.window.data2};
        mRenderInterface->SetViewport(dimensions.x, dimensions.y);
    }
    else
    {
        SDL_Event eventCopy = event;
        RmlSDL::InputEventHandler(mContext, mWindow, eventCopy);
    }
}

void GUI::draw()
{
    mRenderInterface->BeginFrame();
    mContext->Render();
    mRenderInterface->EndFrame();
}

void GUI::toggleDebug()
{
    Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
}
