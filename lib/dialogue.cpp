#include "dialogue.hpp"
#include "button.hpp"
#include "lib/gameState.hpp"
#include <raylib.h>

void SayDialog(GameState &gs, const std::vector<Dialogues> &conversation) {
    gs.currentDialog = conversation;
    gs.currentLineIndex = 0;
    gs.visibleChar = 0;
    gs.dialogueTimer = 0.0f;
    gs.dialogState = gs.currentDialog.empty() ? DIALOGUE_OFF : DIALOGUE_SCROLLING;
}

// void wait_for(GameState &gs, float time) {
//     gs.timeUntilStory = time;
//     gs.inStory = false;
//     gs.storyPosition += 1;
// }

void UpdateDialog(GameState &gs) {
    if (gs.dialogState == DIALOGUE_OFF || gs.currentDialog.empty()) {
        return;
    }

    if (gs.dialogState == DIALOGUE_SCROLLING) {
        gs.dialogueTimer += GetFrameTime();
        const float speed = gs.currentDialog[gs.currentLineIndex].textSpeed;
        if (gs.dialogueTimer >= speed) {
            gs.dialogueTimer = 0.0f;
            gs.visibleChar += 1;

            if (gs.visibleChar >= (int)(gs.currentDialog[gs.currentLineIndex].text.length())) {
                gs.dialogState = DIALOGUE_WAITING;
            }
        }

        if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            gs.visibleChar = (int)(gs.currentDialog[gs.currentLineIndex].text.length());
            gs.dialogState = DIALOGUE_WAITING;
        }
        return;
    }

    if (gs.dialogState == DIALOGUE_WAITING && (IsKeyPressed(KEY_Z)) || IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        gs.currentLineIndex += 1;
        gs.visibleChar = 0;
        gs.dialogueTimer = 0.0f;
        if (gs.currentLineIndex >= (int)(gs.currentDialog.size())) {
            gs.dialogState = DIALOGUE_OFF;
            gs.currentDialog.clear();
        } else {
            gs.dialogState = DIALOGUE_SCROLLING;
        }
    }
}

void DrawDialog(GameState &gs) {
    gs.curExp = gs.currentDialog[gs.currentLineIndex].curExp;
    if (gs.dialogState == DIALOGUE_OFF || gs.currentDialog.empty()) {
        return;
    }
    if (gs.currentLineIndex < 0 || gs.currentLineIndex >= (int)(gs.currentDialog.size())) {
        return;
    }

    const int screenWidth = VIRTUAL_SCREEN_W;
    const int screenHeight =VIRTUAL_SCREEN_H;
    Dialogues &currentLine = gs.currentDialog[gs.currentLineIndex];

    const int padding = 10;
    const int boxWidth = VIRTUAL_SCREEN_W-2*padding;
    const int boxHeight = 90;
    const int boxX = padding;
    const int boxY = screenHeight - boxHeight - 30;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(BLACK, 0.8f));
    // DrawRectangleLinesEx({boxX, boxY, boxWidth, boxHeight}, 2,  WHITE);

    // DrawTexturePro(currentLine.portrait, srcRect, dstRect, {0.0f, 0.0f}, 0.0f, WHITE);
    // DrawRectangleLinesEx(dstRect, 2, RAYWHITE);

    // ui::DrawText(currentLine.characterName.c_str(), boxX + padding, boxY - 15, 18, YELLOW);

    std::string displayString = currentLine.text.substr(0, gs.visibleChar);
    ui::DrawText(displayString.c_str(), boxX + padding, boxY + 15, 14, RAYWHITE);

    if (gs.dialogState == DIALOGUE_WAITING && ((int)(GetTime() * 3) % 2 == 0)) {
        ui::DrawText(gs.touchScreenMode?"Touch anywhere to proceed":"Press 'Z' to proceed", boxX + padding, boxY + boxHeight - 2*padding, 8, WHITE);
    }
}

bool IsDialogRunning(const GameState &gs) {
    return gs.dialogState == DIALOGUE_SCROLLING || gs.dialogState == DIALOGUE_WAITING;
}
