#ifndef GUI_H
#define GUI_H

#include <RmlUi/Core/DataModelHandle.h>

// forward declaration to avoid including <SDL.h>
typedef struct SDL_Window SDL_Window;
typedef union SDL_Event SDL_Event;

namespace Rml {

class SystemInterface;
class Context;
class ElementDocument;

} // namespace Rml

class RenderInterface_GL3;

namespace GUI {

struct FrameStatData
{
	float fps = 0.0;
	float avgTime = 0.0;
    float bestTime = 0.0;
    float worstTime = 0.0;
    int faceCount = 0;
    int vertexCount = 0;
};

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
    FrameStatData & getFrameStatData() {return _frameStatData;}
    const FrameStatData & getFrameStatData() const {return _frameStatData;}
    Rml::DataModelHandle getFrameStatModel() {return _frameStatModel;}
    const Rml::DataModelHandle getFrameStatModel() const {return _frameStatModel;}
    Rml::ElementDocument * getMainMenuDocument() {return _mainMenuDocument;}
    const Rml::ElementDocument * getMainMenuDocument() const {return _mainMenuDocument;}
    Rml::ElementDocument * getFrameStatsDocument() {return _frameStatsDocument;}
    const Rml::ElementDocument * getFrameStatsDocument() const {return _frameStatsDocument;}
protected:
    bool mQuit;
    SDL_Window *mWindow;
    Rml::SystemInterface *mSystemInterface;
    RenderInterface_GL3 *mRenderInterface;
    Rml::Context *mContext;
    FrameStatData _frameStatData;
    Rml::DataModelHandle _frameStatModel;
    Rml::ElementDocument *_frameStatsDocument;
    Rml::ElementDocument *_mainMenuDocument;
};

} // namespace GUI

#endif // GUI_H
