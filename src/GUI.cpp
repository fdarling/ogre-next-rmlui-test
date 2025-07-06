#include "GUI.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>

GUI::GUI(SDL_Window *window) : mRenderer(nullptr), mQuit(false)
{
    // SDL_GL_MakeCurrent(window, ceguiContext);
    int w = 0, h = 0;
    SDL_GetWindowSize(window, &w, &h);
}

GUI::~GUI()
{
}

void GUI::advance(float seconds_elapsed)
{
}

void GUI::handleEvent(const SDL_Event &event)
{
    /*switch (event.type)
    {
        case SDL_QUIT:
        mQuit = true;
        break;

        case SDL_MOUSEMOTION:
        static_cast<float>(event.motion.x), static_cast<float>(event.motion.y)
        break;

        case SDL_MOUSEBUTTONDOWN:
        event.button.button;
        break;

        case SDL_MOUSEBUTTONUP:
        event.button.button;
        break;

        case SDL_MOUSEWHEEL:
        static_cast<float>(event.wheel.y);
        break;

        case SDL_KEYDOWN:
        event.key.keysym.scancode;
        break;

        case SDL_KEYUP:
        event.key.keysym.scancode;
        break;

        case SDL_TEXTINPUT:
        event.text.text;
        break;

        case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            static_cast<float>(event.window.data1), static_cast<float>(event.window.data2)
            glViewport(0, 0, event.window.data1, event.window.data2);
        }
        else if (event.window.event == SDL_WINDOWEVENT_LEAVE)
        {
        }
        break;

        default:
        break;
    }*/
}

void GUI::draw()
{
}
