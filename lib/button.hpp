#pragma once

#include <functional>

#include "raylib.h"

namespace ui {

using ButtonCallback = std::function<void()>;

void Init();
void Shutdown();

void DrawText(const char* text, int pos_x, int pos_y, int font_size,
              Color color);

void DrawTextV(const char* text, Vector2 position, int font_size,
              Color color);

void DrawTextVClickable(const char* text, Vector2 position, int font_size,
                        const ButtonCallback& on_click, Vector2 mousePos);

void DrawTextRotationV(const char* text, Vector2 position, int font_size, float rotInDeg, Color color);

void DrawButton(float x, float y, float width, float height, const char* text,
                int font_size, const ButtonCallback& on_click, Vector2 mousePos);

void DrawButtonNoHover(float x, float y, float width, float height, const char* text,
                int font_size, const ButtonCallback& on_click, Vector2 mousePos, Color fillColor = RAYWHITE);

Font GetActiveFont();
}  // namespace ui
