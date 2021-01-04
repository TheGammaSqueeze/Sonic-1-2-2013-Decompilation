#ifndef RETROENGINE_H
#define RETROENGINE_H

// Disables POSIX use c++ name blah blah stuff
#pragma warning(disable : 4996)

// ================
// STANDARD LIBS
// ================
#include <stdio.h>
#include <string.h>

// ================
// STANDARD TYPES
// ================
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulong;

#define RSDK_DEBUG (1)

// Platforms (RSDKv4 only defines these 7 (I assume), but feel free to add your own custom platform define for easier platform code changes)
#define RETRO_WIN      (0)
#define RETRO_OSX      (1)
#define RETRO_XBOX_360 (2)
#define RETRO_PS3      (3)
#define RETRO_iOS      (4)
#define RETRO_ANDROID  (5)
#define RETRO_WP7      (6)

// Platform types (Game manages platform-specific code such as HUD position using this rather than the above)
#define RETRO_STANDARD      (0)
#define RETRO_MOBILE        (1)

#if defined _WIN32
#define RETRO_PLATFORM (RETRO_WIN)
#define RETRO_PLATTYPE (RETRO_STANDARD)
#elif defined __APPLE__
#if __IPHONEOS__
#define RETRO_PLATTYPE (RETRO_iOS)
#define RETRO_PLATTYPE (RETRO_MOBILE)
#else
#define RETRO_PLATFORM (RETRO_OSX)
#define RETRO_PLATTYPE (RETRO_STANDARD)
#endif
#else
#define RETRO_PLATFORM (RETRO_WIN)
#define RETRO_PLATTYPE (RETRO_STANDARD)
#endif

#if RETRO_PLATFORM == RETRO_WINDOWS || RETRO_PLATFORM == RETRO_OSX
#define RETRO_USING_SDL (1)
#else //Since its an else & not an elif these platforms probably aren't supported yet
#define RETRO_USING_SDL (0)
#endif

#define RETRO_GAME_STANDARD (0)
#define RETRO_GAME_MOBILE   (1)

#if RETRO_PLATFORM == RETRO_iOS || RETRO_PLATFORM == RETRO_ANDROID || RETRO_PLATFORM == RETRO_WP7
#define RETRO_GAMEPLATFORM (RETRO_GAME_MOBILE)
#else
#define RETRO_GAMEPLATFORM (RETRO_GAME_STANDARD)
#endif

#define RETRO_SW_RENDER  (0)
#define RETRO_HW_RENDER  (1)
#define RETRO_RENDERTYPE (RETRO_SW_RENDER)

#define RETRO_USE_HAPTICS (1)

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
    ENGINE_ENDGAME           = 7,
    ENGINE_FINISHTA           = 8,
};

enum RetroGameType {
    GAME_UNKNOWN = 0,
    GAME_SONIC1  = 1,
    GAME_SONIC2  = 2,
};

// General Defines
#define SCREEN_YSIZE (240)
#define SCREEN_CENTERY (SCREEN_YSIZE / 2)

#if RETRO_PLATFORM == RETRO_WIN
#include <SDL.h>
#include <vorbis/vorbisfile.h>
#elif RETRO_PLATFORM == RETRO_OSX
#include <SDL2/SDL.h>
#include <Vorbis/vorbisfile.h>

#include "cocoaHelpers.hpp"
#endif

extern bool usingCWD;

//Utils
#include "Ini.hpp"

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

//Native Entities
#include "RetroGameLoop.hpp"
#include "PauseMenu.hpp"

class RetroEngine
{
public:
    bool usingDataFile = false;
    bool usingBytecode = false;

    bool initialised = false;
    bool running     = false;

    int gameMode     = 1;
    int language     = RETRO_EN;
    int message      = 0;
    bool highResMode = false;

    bool trialMode      = false;
    bool onlineActive   = true;
    bool hapticsEnabled = true;

    int frameSkipSetting = 0;
    int frameSkipTimer   = 0;

    
    // Ported from RSDKv5
    bool devMenu = false;
    int startList  = 0;
    int startStage = 0;
    int gameSpeed        = 1;
    int fastForwardSpeed = 8;
    bool masterPaused    = false;
    bool frameStep       = false;

    bool showPaletteOverlay = false;

    void Init();
    void Run();

    bool LoadGameConfig(const char *Filepath);

    int callbackMessage = 0;
    int prevMessage     = 0;
    int waitValue       = 0;
    void Callback(int callbackID);

    bool finishedStartMenu = false;

    char gameWindowText[0x40];
    char gameDescriptionText[0x100];
    const char *gameVersion       = "1.0.0";
#if RETRO_GAMEPLATFORM == RETRO_GAME_STANDARD
    const char *gamePlatform = "STANDARD";
#elif RETRO_GAMEPLATFORM == RETRO_GAME_MOBILE
    const char *gamePlatform   = "MOBILE";
#endif

#if RETRO_RENDERTYPE == RETRO_SW_RENDER
    const char *gameRenderType = "SW_RENDERING";
#elif RETRO_RENDERTYPE == RETRO_HW_RENDER
    const char *gameRenderType = "HW_RENDERING";
#endif

#if RETRO_USE_HAPTICS
    const char *gameHapticSetting = "USE_F_FEEDBACK"; // NO_F_FEEDBACK is default, but people with controllers exist
#else
    const char *gameHapticSetting = "NO_F_FEEDBACK"; 
#endif

    int gameType = GAME_UNKNOWN;

    ushort *frameBuffer = nullptr;

    bool isFullScreen = false;

    bool fullScreen = false;
    bool borderless = false;
    bool vsync = false;
    int windowScale = 2;
    int refreshRate = 60;

#if RETRO_USING_SDL
    SDL_Window *window        = nullptr;
    SDL_Renderer *renderer    = nullptr;
    SDL_Texture *screenBuffer = nullptr;

    SDL_Event sdlEvents;
#endif
};

extern RetroEngine Engine;
#endif // !RETROENGINE_H