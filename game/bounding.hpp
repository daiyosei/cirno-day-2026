#include <raylib.h>
#include "../lib/gameState.hpp"
#define UI_PADDING_BOTTOM 10
#define UI_BUTTON_SIZE 32

inline Rectangle Calendar = {170, 65, 40, 45};
inline Rectangle Forehead = {80, 190, 58, 50};
inline Rectangle Chest = {75, 130, 70, 50};
inline Rectangle Futton = {145, 190, 80, 110};

inline Rectangle YokaiMountain = {0, 0, 225, 128};
inline Rectangle Igloo = {130, 160, 100, 90};
inline Rectangle MistyLake = {0, 128, 225, 32};
inline Rectangle Flower = {25, 350, 47, 47};

inline Rectangle Frog = {32, 270, 55, 45};
inline Rectangle RiceCake =  {160, 180, 64, 100};

// UI components
inline Rectangle UI_arrowBtn = {(int)(VIRTUAL_SCREEN_W/3)*2 + UI_BUTTON_SIZE/2.0f ,VIRTUAL_SCREEN_H - UI_BUTTON_SIZE - UI_PADDING_BOTTOM, 32, 32};
inline Rectangle UI_talkBtn = {(int)(VIRTUAL_SCREEN_W/2) - 88.0f/2,VIRTUAL_SCREEN_H - UI_BUTTON_SIZE - UI_PADDING_BOTTOM,88, 32};
