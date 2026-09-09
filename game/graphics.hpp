#include <raylib.h>
#include <cmath>
#include <raymath.h>

#include "lib/gameState.hpp"

#define CIRNO_SPRITE_POSITION {-26, 156}

constexpr float TRANSITION_TIME = 1.5f; // in seconds

inline void drawCirno(GameState &gs) {
    switch (gs.curExp) {
        case NEUTRAL:
            DrawTextureV(gs.AssetManager.load("cirno-neutral"), CIRNO_SPRITE_POSITION, WHITE);
            break;
        case PISSED:
            DrawTextureV(gs.AssetManager.load("cirno-pissed"), CIRNO_SPRITE_POSITION, WHITE);
            break;
        case HURT:
            DrawTextureV(gs.AssetManager.load("cirno-hurt"), CIRNO_SPRITE_POSITION, WHITE);
            break;
        case JOY:
            DrawTextureV(gs.AssetManager.load("cirno-joy"), CIRNO_SPRITE_POSITION, WHITE);
            break;
        case WOW:
            DrawTextureV(gs.AssetManager.load("cirno-wow"), CIRNO_SPRITE_POSITION, WHITE);
            break;
        case TIERED:
            DrawTextureV(gs.AssetManager.load("cirno-tiered"), CIRNO_SPRITE_POSITION, WHITE);
            break;
        case LAUGH:
            DrawTextureV(gs.AssetManager.load("cirno-laugh"), CIRNO_SPRITE_POSITION, WHITE);
            break;
        case RELAXED:
            DrawTextureV(gs.AssetManager.load("cirno-relaxed"), CIRNO_SPRITE_POSITION, WHITE);
            break;
    }
}

inline Texture2D getCurrentScene(GameState &gs) {
    switch (gs.curPos) {
        case INSIDE:
            return gs.AssetManager.load("inside");
            break;
        case OUTSIDE:
            return gs.AssetManager.load(gs.flowerOnHead ? "outside2" : "outside");
            break;
        case FOREST:
            return gs.AssetManager.load(gs.riceCakeEaten ? "forest2" : "forest");
            break;
        case MENU:
            return gs.AssetManager.load("menu");
    }
};

inline Texture2D getCurrentScene(GameState &gs, POSITION scene) {
    switch (scene) {
        case INSIDE:
            return gs.AssetManager.load("inside");
            break;
        case OUTSIDE:
            return gs.AssetManager.load(gs.flowerOnHead ? "outside2" : "outside");
            break;
        case FOREST:
            return gs.AssetManager.load(gs.riceCakeEaten ? "forest2" : "forest");
            break;
        case MENU:
            return gs.AssetManager.load("menu");
    }
};

inline void DrawSceneChange(GameState &gs) {
    if (gs.timeSinceSceneChange < TRANSITION_TIME) {
        float t = Clamp(gs.timeSinceSceneChange / TRANSITION_TIME, 0.0f, 1.0f);
        float a = 0.5f - 0.5f * cosf(t * PI);
        DrawTextureV(getCurrentScene(gs, gs.prevPos),
                     ZERO_VEC,
                     ColorAlpha(WHITE, 1.0f - a));

        DrawTextureV(getCurrentScene(gs, gs.curPos),
                     ZERO_VEC,
                     ColorAlpha(WHITE, a));
        gs.timeSinceSceneChange = gs.timeSinceSceneChange + GetFrameTime();
    } else {
        gs.timeSinceSceneChange = -1.0f;
    }
}

inline void changeSceneTo(GameState &gs, POSITION scene) {
    gs.timeSinceSceneChange = 0;
    gs.prevPos = gs.curPos;
    gs.curPos = scene;
}
