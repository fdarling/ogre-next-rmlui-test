#ifndef GUI_H
#define GUI_H

// forward declaration to avoid including <SDL.h>
typedef struct SDL_Window SDL_Window;
typedef union SDL_Event SDL_Event;

namespace Rml {

class SystemInterface;
class Context;

} // namespace Rml

class RenderInterface_GL3;

class GUI
{
public:
    GUI(SDL_Window *window);
    ~GUI();
    void advance(float seconds_elapsed);
    void handleEvent(const SDL_Event &event);
    void draw();
    bool getQuit() const {return mQuit;}
    void toggleDebug();
protected:
    bool mQuit;
    SDL_Window *mWindow;
    Rml::SystemInterface *mSystemInterface;
    RenderInterface_GL3 *mRenderInterface;
    Rml::Context *mContext;
};

#endif // GUI_H
