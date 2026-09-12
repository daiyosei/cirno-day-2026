#define DEBUG false

#include <raylib.h>
#include <raymath.h>

#include "../lib/gameState.hpp"
#include "../lib/dialogue.hpp"
#include "../game/story.hpp"
#include "../lib/button.hpp"

#define UI_PADDING_BOTTOM 10
#define UI_BUTTON_SIZE 32


void drawGameplay(GameState &gs){
    if (gs.GameOver && !IsDialogRunning(gs)) {
        if (gs.GameOverTime == -1.0f){
            gs.GameOverTime = GetTime();
        }

        float t = (GetTime() - gs.GameOverTime)/2.0f;
        if (t > 1.0f) return;

        float a = 0.5f - 0.5f * cosf(t * PI);

        DrawTextureV(getCurrentScene(gs, gs.curPos),
                     ZERO_VEC,
                     ColorAlpha(WHITE, 1.0f - a));
        return;
    }
    
    if (gs.timeSinceSceneChange == -1) {
        DrawTextureV(getCurrentScene(gs), ZERO_VEC, WHITE);
        drawCirno(gs);
        if (gs.curExp != HURT && gs.flowerOnHead) {
            DrawTextureV(gs.AssetManager.load("flower"),  ZERO_VEC, WHITE);
        }
    } else {
        DrawSceneChange(gs);
    }
    if (gs.curExp != NEUTRAL && !IsDialogRunning(gs)) {
        gs.curExp = NEUTRAL;
    }
    if (DEBUG) { DrawText(TextFormat("%d, %d", (int)gs.currentMousePos.x, (int)gs.currentMousePos.y), 0, 40, 20, YELLOW);}
    ui::DrawText(TextFormat("Explored : %d/12", gs.explored.size()), 5, 5, 17, RED);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) and !IsDialogRunning(gs)) {
        ProcessClick(gs);
    }
    DrawTextureV(gs.AssetManager.load("arrowhead"), {(int)(VIRTUAL_SCREEN_W/3)*2 + UI_BUTTON_SIZE/2.0f ,VIRTUAL_SCREEN_H - UI_BUTTON_SIZE - UI_PADDING_BOTTOM}, WHITE);
    DrawTextureV(gs.AssetManager.load("talk"), {(int)(VIRTUAL_SCREEN_W/2) - 88.0f/2 ,VIRTUAL_SCREEN_H - UI_BUTTON_SIZE - UI_PADDING_BOTTOM}, WHITE);
    if (IsDialogRunning(gs)) {
        DrawDialog(gs);
    }
}
