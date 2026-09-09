#pragma once

#include <raylib.h>
#include <vector>
#include "gameState.hpp"

void SayDialog(GameState &gs, const std::vector<Dialogues> &conversation);
// void wait_for(GameState &gs, float time);
void UpdateDialog(GameState &gs);
void DrawDialog(GameState &gs);
bool IsDialogRunning(const GameState &gs);
