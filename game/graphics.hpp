#include <raylib.h>
#include <cmath>
#include <raymath.h>

#include "lib/gameState.hpp"

#define CIRNO_SPRITE_POSITION {-26, 156}

constexpr float TRANSITION_TIME = 1.5f; // in seconds

inline Texture2D getCirno(GameState &gs)
{
    switch (gs.curExp) {
        case NEUTRAL:
            return gs.AssetManager.load("cirno-neutral");
        case PISSED:
            return gs.AssetManager.load("cirno-pissed");
        case HURT:
            return gs.AssetManager.load("cirno-hurt");
        case JOY:
            return gs.AssetManager.load("cirno-joy");
        case WOW:
            return gs.AssetManager.load("cirno-wow");
        case TIERED:
            return gs.AssetManager.load("cirno-tiered");
        case LAUGH:
            return gs.AssetManager.load("cirno-laugh");
        case RELAXED:
            return gs.AssetManager.load("cirno-relaxed");
    }
}

inline void drawCirno(GameState &gs) {
    DrawTextureV(getCirno(gs), CIRNO_SPRITE_POSITION, WHITE);
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
        float t = Clamp(gs.timeSinceSceneChange / (TRANSITION_TIME / 2), 0.0f, 1.0f);
        float a = 0.5f - 0.5f * cosf(t * PI);

        DrawTextureV(getCurrentScene(gs, gs.prevPos),
                     ZERO_VEC,
                     ColorAlpha(WHITE, 1.0f - a));

        DrawTextureV(getCurrentScene(gs, gs.curPos),
                     ZERO_VEC,
                     ColorAlpha(WHITE, a));

        if (gs.timeSinceSceneChange > (TRANSITION_TIME/2)) {
            float halfTime = gs.timeSinceSceneChange - (TRANSITION_TIME / 2);
            float t2 = Clamp(halfTime / (TRANSITION_TIME / 2), 0.0f, 1.0f);
            float a2 = 0.5f - 0.5f * cosf(t2 * PI);

            DrawTextureV(getCirno(gs),
                         CIRNO_SPRITE_POSITION,
                         ColorAlpha(WHITE, a2));

            if (gs.curExp != HURT && gs.flowerOnHead) {
                DrawTextureV(gs.AssetManager.load("flower"),  ZERO_VEC, ColorAlpha(WHITE, a2));
            }
        }

        gs.timeSinceSceneChange = gs.timeSinceSceneChange + GetFrameTime();
    }
    else {
        gs.timeSinceSceneChange = -1.0f;
    }
}

inline void changeSceneTo(GameState &gs, POSITION scene) {
    gs.timeSinceSceneChange = 0;
    gs.prevPos = gs.curPos;
    gs.curPos = scene;
}
