#include "button.hpp"
#include "raylib.h"
#include <raymath.h>

constexpr Color HOVER_COLOR = Color{140, 0, 23, 255};
constexpr Color FILL_COLOR = Color{148, 35, 67, 255};
constexpr Color BORDER_COLOR = Color{204, 0, 57, 255};

namespace ui {
namespace {

constexpr const char* kUIFontPath = "../assets/font/font.otf";
constexpr const char* kUIClickSfxPath = "../assets/audio/sfx/buttonClick.wav";

bool g_initialized = false;
bool g_loaded_font = false;
Font g_ui_font = {};
Sound g_ui_clickSfx ;

}  // namespace

Font GetActiveFont() { return g_loaded_font ? g_ui_font : GetFontDefault(); }

void Init() {
  if (g_initialized) return;

  if (FileExists(kUIFontPath)) {
    g_ui_font = LoadFontEx(kUIFontPath, 256, nullptr, 0);
    g_loaded_font = g_ui_font.texture.id != 0;
  }
  g_ui_clickSfx = LoadSound(kUIClickSfxPath);

  g_initialized = true;
}

void Shutdown() {
  if (!g_initialized) return;

  if (g_loaded_font) {
    UnloadFont(g_ui_font);
    g_loaded_font = false;
  }

  g_initialized = false;
}

void DrawText(const char* text, int pos_x, int pos_y, int font_size,
              Color color) {
  if (g_loaded_font) {
    DrawTextEx(g_ui_font, text, {static_cast<float>(pos_x), static_cast<float>(pos_y)},
               static_cast<float>(font_size), 1.0f, color);
    return;
  }

  ::DrawText(text, pos_x, pos_y, font_size, color);
}

void DrawTextV(const char* text, Vector2 position, int font_size,
              Color color) {
  if (g_loaded_font) {
    DrawTextEx(g_ui_font, text, {static_cast<float>(position.x), static_cast<float>(position.y)},
               static_cast<float>(font_size), 1.0f, color);
    return;
  }

  ::DrawText(text, position.x, position.y, font_size, color);
}

void DrawTextRotationV(const char* text, Vector2 position, int font_size, float rotInDeg, Color color) {
    if (g_loaded_font) {
        DrawTextPro(g_ui_font, text, position, {0,0},rotInDeg,font_size,1.0f,color);
        return;
    }
    DrawTextPro(GetFontDefault(), text, position, {0,0},rotInDeg,font_size,1.0f,color);
}

void DrawButton(float x, float y, float width, float height, const char* text,
                int font_size, const ButtonCallback& on_click, Vector2 mousePos) {
  Rectangle bounds = {x, y, width, height};
  const bool is_hovered = CheckCollisionPointRec(mousePos, bounds);
  const Color fill_color = is_hovered ? HOVER_COLOR : FILL_COLOR;

  DrawRectangleRounded(bounds, 0.14f, 8, fill_color);
  DrawRectangleRoundedLinesEx(bounds, 0.14f, 4, 4.0f, BORDER_COLOR);

  const Font font = GetActiveFont();
  const Vector2 text_size = MeasureTextEx(font, text, static_cast<float>(font_size), 1.0f);
  const float text_x = x + (width - text_size.x) * 0.5f;
  const float text_y = y + (height - text_size.y) * 0.5f;
  ui::DrawText(text, static_cast<int>(text_x), static_cast<int>(text_y), font_size, BLACK);

  if (is_hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    PlaySound(g_ui_clickSfx);
    if (on_click) {
      on_click();
    }
  }
}

void DrawButtonNoHover(float x, float y, float width, float height, const char* text,
                int font_size, const ButtonCallback& on_click, Vector2 mousePos, Color fillColor) {
  Rectangle bounds = {x, y, width, height};
  const bool is_hovered = CheckCollisionPointRec(mousePos, bounds);

  DrawRectangleRounded(bounds, 0.14f, 8, fillColor);
  DrawRectangleRoundedLinesEx(bounds, 0.14f, 8, 2.0f, BORDER_COLOR);

  const Font font = GetActiveFont();
  const Vector2 text_size = MeasureTextEx(font, text, static_cast<float>(font_size), 1.0f);
  const float text_x = x + (width - text_size.x) * 0.5f;
  const float text_y = y + (height - text_size.y) * 0.5f;
  ui::DrawText(text, static_cast<int>(text_x), static_cast<int>(text_y), font_size, BLACK);

  if (is_hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    // auto& asset_manager = assets::AssetManager::Instance();
    // PlaySound(asset_manager.SoundEffect("switch"));
    if (on_click) {
      on_click();
    }
  }
}


void DrawTextVClickable(const char* text, Vector2 position, int font_size,
                        const ButtonCallback& on_click, Vector2 mousePos) {
  Vector2 text_size;
  if (g_loaded_font) {
    text_size = MeasureTextEx(g_ui_font, text, static_cast<float>(font_size), 1.0f);
  } else {
    // Default Raylib font measurement
    text_size.x = static_cast<float>(MeasureText(text, font_size));
    text_size.y = static_cast<float>(font_size);
  }
  Rectangle bounds = {position.x, position.y, text_size.x, text_size.y};
  const bool is_hovered = CheckCollisionPointRec(mousePos, bounds);

  // Apply hover colors
  const Color text_color = is_hovered ? WHITE : YELLOW;

  ui::DrawTextV(text, position, font_size, text_color);

  if (is_hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    if (on_click) {
      on_click();
    }
  }
}

}  // namespace ui
