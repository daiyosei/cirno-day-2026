#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <set>

#include "assetManager.hpp"

constexpr int MAX_USERNAME_LENGTH = 16;
constexpr int VIRTUAL_SCREEN_W = 225;
constexpr int VIRTUAL_SCREEN_H = 400;
constexpr float WEB_RENDER_SCALE = 2.0f;
constexpr Vector2 ZERO_VEC = {0,0};

inline Music g_bgMusic;

enum DialogueState {
    DIALOGUE_OFF,
    DIALOGUE_SCROLLING,
    DIALOGUE_WAITING,
};

enum screen {
    MAIN_MENU,
    GAMEPLAY,
};


enum EXPRESSION {
    NEUTRAL,
    PISSED,
    HURT,
    JOY,
    LAUGH,
    RELAXED,
    TIERED,
    WOW,
};

struct Dialogues
{
    // Texture2D portrait;
    std::string characterName;
    EXPRESSION curExp;
    std::string text;
    // Sound scrollSound;
    float textSpeed;
};

enum POSITION {
    INSIDE,
    OUTSIDE,
    FOREST,
    MENU
};


struct GameState {
    screen current_screen = MAIN_MENU; // TODO : Change before prod
    int gameScore = 0;
    int highScore = 0;
    bool paused = false;
    bool gameOver = false;
    /*
     * We use a cartesian system that is right handed
     * right is posetive x and up is posetive y
     * The origin is at the bottom left corner of the
     * playable area.
     * Max limit is 100, 100 (note that this scale is as percentage)
     * x = y wont give a straight line with 45 degree angle
     * ^ +y
     * |
     * |
     * |_________ > +x
     */

    /* Dialogue specific */
    DialogueState dialogState = DIALOGUE_OFF;
    std::vector<Dialogues> currentDialog;
    float dialogueTimer = 0.0f;
    int currentLineIndex = 0;
    int visibleChar = 0;

    short storyStage = 1;

    /* Asset Manager */
    assetManager AssetManager;

    float scale = 1.0f;
    float renderScale = 1.0f;
    Vector2 currentMousePos = {0,0};
    Font numberFont;
    RenderTexture screenTex;

    bool touchScreenMode = false;

    POSITION curPos = MENU;
    EXPRESSION curExp = NEUTRAL;

    float timeSinceSceneChange = -1;
    POSITION prevPos = INSIDE;

    short flickCount = 0;
    short chestCount = 0;
    bool flowerOnHead = false;
    bool riceCakeEaten = false;
    bool frogDialogDone = false;
    bool usedTalkButton = false;
    std::set<int> explored = {};
};
