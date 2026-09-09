#include "standard.hpp"
#include "gameState.hpp"
#include <cstdlib>
#include <cmath>
#include <raylib.h>
#include <raymath.h>

constexpr float CARTESIAN_MIN = 0.0f;
constexpr float CARTESIAN_MAX = 100.0f;
constexpr float CARTESIAN_SCALE = 100.0f;

float maxOfTwo(float a, float b) {
    if (a > b) {return a;}
    else {return b;}
}

float minOfTwo(float a, float b) {
    if (a < b) {return a;}
    else {return b;}
}

float genRandBw(float min,float max) {
    return (((float)rand()/RAND_MAX)*(max-min))+min;
}

Vector2 rotatePoint(Vector2 point, Vector2 pivot, float angleDegree) {
    float rad = angleDegree * DEG2RAD;
    float s = sinf(rad);
    float c = cosf(rad);
    point.x -= pivot.x;
    point.y -= pivot.y;
    float xNew = point.x * c - point.y * s;
    float yNew = point.x * s + point.y * c;
    return (Vector2){ xNew + pivot.x, yNew + pivot.y };
}

Vector2 C2SProj(Vector2 cartesian_cordinates, Vector4 rectangle_space, Vector2 object_size,
    Vector2 absolute_deviation_bias,bool use_clamp) {
    /*Cartesian to Screen Projection */
    float y_axis_length = rectangle_space.w - rectangle_space.y;
    float x_axis_length = rectangle_space.z - rectangle_space.x;
    float usable_width = maxOfTwo(0.0f, x_axis_length - object_size.x);
    float usable_height = maxOfTwo(0.0f, y_axis_length - object_size.y);
    float normalized_x;
    float normalized_y;
    if (use_clamp) {
        normalized_x = Clamp(cartesian_cordinates.x, CARTESIAN_MIN, CARTESIAN_MAX)/CARTESIAN_SCALE;
        normalized_y = Clamp(cartesian_cordinates.y, CARTESIAN_MIN, CARTESIAN_MAX)/CARTESIAN_SCALE;
    } else {
        normalized_x = cartesian_cordinates.x/CARTESIAN_SCALE;
        normalized_y = cartesian_cordinates.y/CARTESIAN_SCALE;
    }
    float y_pos = (1 - normalized_y)*usable_height;
    float x_pos = normalized_x*usable_width;
    return {rectangle_space.x + x_pos + absolute_deviation_bias.x,
        rectangle_space.y + y_pos + absolute_deviation_bias.y};
}

float S2CYaxis(float yValue, Vector4 rectangle_space) {
    float y_axis_length = rectangle_space.w - rectangle_space.y;
    return ((yValue/y_axis_length) * 100);
}

float S2CXaxis(float xValue, Vector4 rectangle_space) {
    float x_axis_length = rectangle_space.z - rectangle_space.x;
    return ((xValue/x_axis_length)*100);
}

void newGame(GameState& gs){
    gs.gameScore = 0;
    gs.paused = false;
    gs.gameOver = false;
    gs.dialogState = DIALOGUE_OFF;
    gs.dialogueTimer = 0.0f;
    gs.currentLineIndex = 0;
    gs.visibleChar = 0;
}

bool returnTrueProbPerc(float percentageTrue){
    float i = genRandBw(0, 1);
    if (i < (percentageTrue/100)) {
        return true;
    }
    return false;
}

Vector2 ScreenToVirtual(Vector2 screenPos, Vector2 offset, float scale)
{
    return Vector2{
        (screenPos.x - offset.x) / scale,
        (screenPos.y - offset.y) / scale
    };
}
