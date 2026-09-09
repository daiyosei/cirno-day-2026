#include <cstdlib>
#include <algorithm>
#include <raylib.h>

#include "lib/button.hpp"
#include "lib/dialogue.hpp"
#include "lib/gameState.hpp"
#include "screen/gameplay.cpp"
#include "screen/main_menu.cpp"
#include "lib/assetManager.hpp"
#include "lib/standard.hpp"

#if defined (__EMSCRIPTEN__)
#include <emscripten.h>
#include <emscripten/em_js.h>
#endif
// #include "lib/lbParser.hpp"

namespace {

void drawFrame(GameState& currentGameState) {
    UpdateMusicStream(g_bgMusic);
    // if (GetMusicTimePlayed(g_bgMusic) >= GetMusicTimeLength(g_bgMusic)) {
    //     SeekMusicStream(g_bgMusic, 0);
    // }
    RenderTexture screenTex = currentGameState.screenTex;
    float screenW = GetScreenWidth();
    float screenH = GetScreenHeight();
    float renderScale = currentGameState.renderScale;

    if (IsDialogRunning(currentGameState)) {
        UpdateDialog(currentGameState);
    }

    BeginDrawing();
    ClearBackground(BLACK);
    if (true){ //screenW > screenH) {
        float scaleX = screenW / (float)VIRTUAL_SCREEN_W;
        float scaleY = screenH / (float)VIRTUAL_SCREEN_H;

        currentGameState.scale = std::min(scaleX, scaleY);

        float destW = VIRTUAL_SCREEN_W * currentGameState.scale;
        float destH = VIRTUAL_SCREEN_H * currentGameState.scale;

        float destX = (screenW - destW) * 0.5f;
        float destY = (screenH - destH) * 0.5f;

        Vector2 offset = { destX, destY };
        currentGameState.currentMousePos = ScreenToVirtual(GetMousePosition(), offset, currentGameState.scale);

        BeginTextureMode(screenTex);
        Camera2D camera = {};
        camera.target = {0.0f, 0.0f};
        camera.offset = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = renderScale;
        BeginMode2D(camera);
        switch (currentGameState.current_screen) {
        case MAIN_MENU:
            drawMainMenu(currentGameState);
            break;
        case GAMEPLAY:
            drawGameplay(currentGameState);
            break;
        }
        EndMode2D();
        EndTextureMode();

        DrawTexturePro(
            screenTex.texture,
            {0, 0, (float)screenTex.texture.width, -(float)screenTex.texture.height},
            {destX, destY, destW, destH},
            {0, 0},
            0,
            WHITE
        );}
    else {
        ui::DrawTextV("Please Play in\nLandscape\nMode", {10,50}, 40, WHITE);
    }
    // DrawText(TextFormat("%d, %f", 1, GetTime()), 0, 0, 20, YELLOW);
    EndDrawing();
}
}

void updateDrawFrame(void * arg){
    GameState* currentGameState = static_cast<GameState*>(arg);
    drawFrame(*currentGameState);
}
#if defined (__EMSCRIPTEN__)
EM_JS(int, isOnPhone, (), {
    const ua = navigator.userAgent || "";
    const mobileDetected =
        /Android|iPhone|iPad|iPod|Mobile|webOS|BlackBerry|IEMobile|Opera Mini/i.test(
            ua,
        ) ||
        (navigator.maxTouchPoints || 0) > 1 ||
        (window.matchMedia && window.matchMedia("(pointer: coarse)").matches);
    if (mobileDetected) {
        return 1;
    } else {
        return 0;
    }
})
#endif

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(VIRTUAL_SCREEN_W, VIRTUAL_SCREEN_H, "Cirno Day");
    // SetWindowOpacity(0.5f);
    GameState currentGameState;
    InitAudioDevice();
    srand(111111);
    ui::Init();
    currentGameState.numberFont = LoadFont("../assets/font/score.otf");

    // currentGameState.bulletList[0] = new Bullet{
    //     LoadTexture("assets/img/bullet/knife.png"),
    //     linear
    // };
    // TODO : Implement Asset Manager
    // DisableCursor();
    g_bgMusic = LoadMusicStream("../assets/audio/bgm/bg.ogg");
    // PlayMusicStream(g_bgMusic);
    SetMusicVolume(g_bgMusic, 0.5f);
    currentGameState.AssetManager.init();
    #if defined (__EMSCRIPTEN__)
    currentGameState.touchScreenMode = (bool)isOnPhone();
    #else
    currentGameState.touchScreenMode = false;
    #endif
    currentGameState.renderScale = WEB_RENDER_SCALE;
    currentGameState.screenTex = LoadRenderTexture(
        (int)(VIRTUAL_SCREEN_W * currentGameState.renderScale),
        (int)(VIRTUAL_SCREEN_H * currentGameState.renderScale)
    );
    SetTextureFilter(currentGameState.screenTex.texture, TEXTURE_FILTER_BILINEAR);
    // int repeatInWidth = screenW/200;
    // int repeatinHeight = screenH/200;

    // BeginTextureMode(backgroundTex);
    //     for (int w = 0; w < repeatInWidth + 1; w++) {
    //         for (int h = 0; h < repeatinHeight + 1; h++) {
    //             DrawTextureV(currentGameState.AssetManager.load("hud_background"), {200.0f*w, 200.0f*h} , WHITE);
    //         }
    //     }
    // EndTextureMode();
    #if defined (__EMSCRIPTEN__)
        emscripten_set_main_loop_arg(updateDrawFrame, &currentGameState, 0, 1);
    #else
        SetTargetFPS(60);
        while(!WindowShouldClose()){
            drawFrame(currentGameState);
        }
    #endif

    currentGameState.AssetManager.cleanup();

    ui::Shutdown();
    CloseWindow();
    return 0;
}
