#ifndef RETROENGINE_H
#define RETROENGINE_H

// Disables POSIX use c++ name blah blah stuff
#pragma warning(disable : 4996)

// Setting this to true removes (almost) ALL changes from the original code, the trade off is that a playable game cannot be built, it is advised to
// be set to true only for preservation purposes
#define RETRO_USE_ORIGINAL_CODE (0)
#define RETRO_USE_MOD_LOADER    (0)

#if !RETRO_USE_ORIGINAL_CODE
#undef RETRO_USE_MOD_LOADER
#define RETRO_USE_MOD_LOADER (1)
#endif //  !RETRO_USE_ORIGINAL_CODE

// ================
// STANDARD LIBS
// ================
#include <stdio.h>
#include <string.h>
#include <cmath>

// ================
// STANDARD TYPES
// ================
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;
// typedef unsigned long long ulong;

// Platforms (RSDKv4 only defines these 7 (I assume), but feel free to add your own custom platform define for easier platform code changes)
#define RETRO_WIN      (0)
#define RETRO_OSX      (1)
#define RETRO_XBOX_360 (2)
#define RETRO_PS3      (3)
#define RETRO_iOS      (4)
#define RETRO_ANDROID  (5)
#define RETRO_WP7      (6)
// Custom Platforms start here
#define RETRO_UWP (7)

// Platform types (Game manages platform-specific code such as HUD position using this rather than the above)
#define RETRO_STANDARD (0)
#define RETRO_MOBILE   (1)

#if defined _WIN32

#if defined WINAPI_FAMILY
#if WINAPI_FAMILY != WINAPI_FAMILY_APP
#define RETRO_PLATFORM   (RETRO_WIN)
#define RETRO_DEVICETYPE (RETRO_STANDARD)
#else
#include <WInRTIncludes.hpp>

#define RETRO_PLATFORM   (RETRO_UWP)
#define RETRO_DEVICETYPE (UAP_GetRetroGamePlatform())
#endif
#else
#define RETRO_PLATFORM   (RETRO_WIN)
#define RETRO_DEVICETYPE (RETRO_STANDARD)
#endif

#elif defined __APPLE__
#if __IPHONEOS__
#define RETRO_PLATFORM   (RETRO_iOS)
#define RETRO_DEVICETYPE (RETRO_MOBILE)
#else
#define RETRO_PLATFORM   (RETRO_OSX)
#define RETRO_DEVICETYPE (RETRO_STANDARD)
#endif
#elif defined __ANDROID__
#define RETRO_PLATFORM   (RETRO_ANDROID)
#define RETRO_DEVICETYPE (RETRO_MOBILE)
#include <jni.h>
#else
#define RETRO_PLATFORM   (RETRO_WIN)
#define RETRO_DEVICETYPE (RETRO_STANDARD)
#endif

#define DEFAULT_SCREEN_XSIZE 424
#define DEFAULT_FULLSCREEN   false
#define RETRO_USING_MOUSE
#define RETRO_USING_TOUCH
// set this to 1 (integer scale) for other platforms that don't support bilinear and don't have an even screen size
#define RETRO_DEFAULTSCALINGMODE 2

#ifndef BASE_PATH
#define BASE_PATH ""
#endif

#if RETRO_PLATFORM == RETRO_WIN || RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_UWP || RETRO_PLATFORM == RETRO_ANDROID
#define RETRO_USING_SDL1 (0)
#define RETRO_USING_SDL2 (1)
#else // Since its an else & not an elif these platforms probably aren't supported yet
#define RETRO_USING_SDL1 (0)
#define RETRO_USING_SDL2 (0)
#endif

#if RETRO_PLATFORM == RETRO_iOS || RETRO_PLATFORM == RETRO_ANDROID || RETRO_PLATFORM == RETRO_WP7
#define RETRO_GAMEPLATFORM (RETRO_MOBILE)
#elif RETRO_PLATFORM == RETRO_UWP
#define RETRO_GAMEPLATFORM (UAP_GetRetroGamePlatform())
#else
#define RETRO_GAMEPLATFORM (RETRO_STANDARD)
#endif

#define RETRO_SW_RENDER  (0)
#define RETRO_HW_RENDER  (1)
#define RETRO_RENDERTYPE (RETRO_SW_RENDER)

#ifdef USE_SW_REN
#undef RETRO_RENDERTYPE
#define RETRO_RENDERTYPE (RETRO_SW_RENDER)
#endif

#ifdef USE_HW_REN
#undef RETRO_RENDERTYPE
#define RETRO_RENDERTYPE (RETRO_HW_RENDER)
#endif

#if RETRO_RENDERTYPE == RETRO_SW_RENDER
#define RETRO_USING_OPENGL (0)
#elif RETRO_RENDERTYPE == RETRO_HW_RENDER
#define RETRO_USING_OPENGL (1)
#endif

#define RETRO_SOFTWARE_RENDER (RETRO_RENDERTYPE == RETRO_SW_RENDER)
#define RETRO_HARDWARE_RENDER (RETRO_RENDERTYPE == RETRO_HW_RENDER)

#if RETRO_USING_OPENGL
#include <GL/glew.h>
#include <GL/glu.h>

#if RETRO_USING_SDL2
#include <SDL_opengl.h>
#endif
#endif

#define RETRO_USE_HAPTICS (1)

// reverts opcode list back to how it was in earliest builds, fixes bugs on some datafiles
// generally advised to keep this set to 0
#define RETRO_REV01 (0)

enum RetroLanguages {
    RETRO_EN = 0,
    RETRO_FR = 1,
    RETRO_IT = 2,
    RETRO_DE = 3,
    RETRO_ES = 4,
    RETRO_JP = 5,
    RETRO_PT = 6,
    RETRO_RU = 7,
    RETRO_KO = 8,
    RETRO_ZH = 9,
    RETRO_ZS = 10,
};

enum RetroStates {
    ENGINE_DEVMENU     = 0,
    ENGINE_MAINGAME    = 1,
    ENGINE_INITDEVMENU = 2,
    ENGINE_WAIT        = 3,
    ENGINE_SCRIPTERROR = 4,
    ENGINE_INITPAUSE   = 5,
    ENGINE_EXITPAUSE   = 6,
    ENGINE_ENDGAME     = 7,
    ENGINE_RESETGAME   = 8,

#if !RETRO_USE_ORIGINAL_CODE
    // Custom GameModes (required to make some features work)
    ENGINE_STARTMENU   = 0x80,
    ENGINE_CONNECT2PVS = 0x81,
    ENGINE_INITMODMENU = 0x82,
#endif
};

enum RetroGameType {
    GAME_UNKNOWN = 0,
    GAME_SONIC1  = 1,
    GAME_SONIC2  = 2,
};

// General Defines
#define SCREEN_YSIZE   (240)
#define SCREEN_CENTERY (SCREEN_YSIZE / 2)

#if RETRO_PLATFORM == RETRO_WIN || RETRO_PLATFORM == RETRO_UWP || RETRO_PLATFORM == RETRO_ANDROID
#if RETRO_USING_SDL2
#include <SDL.h>
#elif RETRO_USING_SDL1
#include <SDL.h>
#endif
#include <vorbis/vorbisfile.h>
#elif RETRO_PLATFORM == RETRO_OSX
#include <SDL2/SDL.h>
#include <Vorbis/vorbisfile.h>

#include "cocoaHelpers.hpp"

#elif RETRO_USING_SDL2
#include <SDL2/SDL.h>
#include <vorbis/vorbisfile.h>
#else

#endif

#if !RETRO_USE_ORIGINAL_CODE
extern bool usingCWD;
extern bool engineDebugMode;
#endif

// Utils
#if !RETRO_USE_ORIGINAL_CODE
#include "Ini.hpp"
#endif

#include "Math.hpp"
#include "Reader.hpp"
#include "String.hpp"
#include "Animation.hpp"
#include "Audio.hpp"
#include "Input.hpp"
#include "Object.hpp"
#include "Palette.hpp"
#include "Drawing.hpp"
#include "Scene3D.hpp"
#include "Collision.hpp"
#include "Scene.hpp"
#include "Script.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
#include "Userdata.hpp"
#include "Debug.hpp"

// Native Entities
#include "PauseMenu.hpp"
#include "RetroGameLoop.hpp"

class RetroEngine
{
public:
    RetroEngine()
    {
        if (RETRO_GAMEPLATFORM == RETRO_STANDARD)
            gamePlatform = "STANDARD";
        else
            gamePlatform = "MOBILE";
    }

    bool usingDataFile = false;
    bool usingBytecode = false;

    char dataFile[RETRO_PACKFILE_COUNT][0x80];

    bool initialised = false;
    bool running     = false;

    int gameMode = 1;
    int language = RETRO_EN;
    int message  = 0;

    bool trialMode      = false;
    bool onlineActive   = true;
    bool hapticsEnabled = true;

    int frameSkipSetting = 0;
    int frameSkipTimer   = 0;

#if !RETRO_USE_ORIGINAL_CODE
    // Ported from RSDKv5
    bool devMenu         = false;
    int startList        = -1;
    int startStage       = -1;
    int startPlayer      = -1;
    int startSave        = -1;
    int gameSpeed        = 1;
    int fastForwardSpeed = 8;
    bool masterPaused    = false;
    bool frameStep       = false;
    int dimTimer         = 0;
    int dimLimit         = 0;
    float dimPercent     = 1.0;
    float dimMax         = 1.0;

    bool showPaletteOverlay = false;
    bool useHQModes         = true;
#endif

    void Init();
    void Run();

    bool LoadGameConfig(const char *Filepath);

    int callbackMessage = 0;
    int prevMessage     = 0;
    int waitValue       = 0;

    char gameWindowText[0x40];
    char gameDescriptionText[0x100];
    const char *gameVersion  = "1.1.2";
    const char *gamePlatform = nullptr;

#if RETRO_RENDERTYPE == RETRO_SW_RENDER
    const char *gameRenderType = "SW_RENDERING";
#elif RETRO_RENDERTYPE == RETRO_HW_RENDER
    const char *gameRenderType = "HW_RENDERING";
#endif

#if RETRO_USE_HAPTICS
    const char *gameHapticSetting = "USE_F_FEEDBACK"; // None is default, but people with controllers exist
#else
    const char *gameHapticSetting = "NO_F_FEEDBACK";
#endif

#if !RETRO_USE_ORIGINAL_CODE
    byte gameType = GAME_UNKNOWN;
#if RETRO_USE_MOD_LOADER
    bool modMenuCalled = false;
#endif
#endif

#if RETRO_SOFTWARE_RENDER
    ushort *frameBuffer   = nullptr;
    ushort *frameBuffer2x = nullptr;
#endif

#if !RETRO_USE_ORIGINAL_CODE
    bool isFullScreen = false;

    bool startFullScreen  = false; // if should start as fullscreen
    bool borderless       = false;
    bool vsync            = false;
    int scalingMode       = RETRO_DEFAULTSCALINGMODE;
    int windowScale       = 2;
    int refreshRate       = 60; // user-picked screen update rate
    int screenRefreshRate = 60; // hardware screen update rate
    int targetRefreshRate = 60; // game logic update rate

    uint frameCount      = 0; // frames since scene load
    int renderFrameIndex = 0;
    int skipFrameIndex   = 0;

    int windowXSize; // width of window/screen in the previous frame
    int windowYSize; // height of window/screen in the previous frame
#endif

#if !RETRO_USE_ORIGINAL_CODE
#if RETRO_USING_SDL2
    SDL_Window *window     = nullptr;
    SDL_Renderer *renderer = nullptr;
#if RETRO_SOFTWARE_RENDER
    SDL_Texture *screenBuffer   = nullptr;
    SDL_Texture *screenBuffer2x = nullptr;
#endif // RETRO_SOFTWARE_RENDERER

    SDL_Event sdlEvents;

#if RETRO_USING_OPENGL
    SDL_GLContext m_glContext; // OpenGL context
#endif // RETRO_USING_OPENGL
#endif // RETRO_USING_SDL2

#if RETRO_USING_SDL1
    SDL_Surface *windowSurface = nullptr;

    SDL_Surface *screenBuffer   = nullptr;
    SDL_Surface *screenBuffer2x = nullptr;

    SDL_Event sdlEvents;
#endif // RETRO_USING_SDL1
#endif //! RETRO_USE_ORIGINAL_CODE
};

extern RetroEngine Engine;
#endif // !RETROENGINE_H
