#include "../lib/gameState.hpp"
#include "lib/button.hpp"
#include "raylib.h"
#include "raymath.h"
#include "cmath"

constexpr float SCENE_CHANGE = 6.0f;

void drawMainMenu(GameState &gs){
    ClearBackground(Color{91, 110, 225, 255});
    // Vector2 i = MeasureTextEx(ui::GetActiveFont(), "Happy Cirno Day", 22, 1.0f);
    // printf("%f, %f\n", i.x, i.y);
    // Measure text and hard code the vector value below
    if (gs.timeSinceSceneChange < SCENE_CHANGE) {
        float t = Clamp(gs.timeSinceSceneChange / SCENE_CHANGE, 0.0f, 1.0f);
        float e = 1.0f - powf(1.0f - t, 3.0f);
        float y = VIRTUAL_SCREEN_H * (1.0f - e);
        DrawTextureV(
            gs.AssetManager.load("menu"),
            {0, y},
            WHITE
        );
        gs.timeSinceSceneChange += GetFrameTime();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            gs.timeSinceSceneChange = SCENE_CHANGE;
        }

    } else {
        DrawTextureV(
            gs.AssetManager.load("menu"),
            ZERO_VEC,
            WHITE
        );
        ui::DrawTextV("Knock on her door to play!", {20, 380}, 14,ColorAlpha(RED, fabsf(sinf(3*GetTime()))));
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            gs.timeSinceSceneChange = -1;
            changeSceneTo(gs, INSIDE);
            gs.current_screen = GAMEPLAY;
        }
    }
    ui::DrawTextV("Happy Cirno Day", {22, 20}, 22, ColorAlpha(YELLOW, Clamp(gs.timeSinceSceneChange/(SCENE_CHANGE/2.0f), 0, 1)));
    ui::DrawTextV("A short story by\nsamosagaming69 &\nsilli_chilli", {22, 48}, 14, ColorAlpha(GREEN, Clamp(gs.timeSinceSceneChange/(SCENE_CHANGE/2.0f), 0, 1)));
    ui::DrawTextV("Music: Tomboyish Prelude\nAlbum: Fantasy Kaleidoscope OST\nCircle: Yuuhei Satellite\nArrangement: Iceon", {3, 323}, 12, ColorAlpha(ORANGE, Clamp(gs.timeSinceSceneChange/(SCENE_CHANGE/2.0f), 0, 1)));
}
