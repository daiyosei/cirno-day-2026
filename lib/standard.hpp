#pragma once
#include "gameState.hpp"
#include <raylib.h>

float maxOfTwo(float a, float b);
float minOfTwo(float a, float b);
float genRandBw(float min, float max);
Vector2 rotatePoint(Vector2 point, Vector2 pivot, float angleDegree);
Vector2 C2SProj(Vector2 cartesian_cordinates, Vector4 rectangle_space, Vector2 object_size = {0, 0}, Vector2 absolute_deviation_bias={0,0},bool use_clamp = true);
float S2CYaxis(float yValue, Vector4 rectangle_space);
float S2CXaxis(float xValue, Vector4 rectangle_space);
void newGame(GameState& gs);
bool returnTrueProbPerc(float percentageTrue);
Vector2 ScreenToVirtual(Vector2 screenPos, Vector2 offset, float scale);
