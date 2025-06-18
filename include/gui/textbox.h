#pragma once

#include "widget.h"
#include <string>
#include <SDL2/SDL_ttf.h>

namespace gui {
    class TextBox: public Widget {
        public:
            TextBox(int x, int y, int width, int height, TTF_Font* font = nullptr, const std::string& id = "");
            void render(SDL_Renderer* renderer, int offsetX = 0, int offsetY = 0) override;
            void handleEvent(const SDL_Event& event, int offsetX = 0, int offsetY = 0) override;

            void setText(const std::string& text);
            const std::string& getText() const { return m_text; };

            void setFocused(bool focused) { m_focused = focused; }
            bool isFocused() const { return m_focused; }

        private:
            std::string m_text;
            TTF_Font* m_font;
            SDL_Color m_textColour = { 0, 0, 0, 255 };
            SDL_Color m_boxColour = { 255, 255, 255, 255 };
            SDL_Color m_borderColour = { 0, 0, 0, 255 };

            SDL_Texture* m_texture = nullptr;
            void updateTexture(SDL_Renderer* renderer);

            size_t m_cursorPosition = 0;
            bool m_focused = false;
            Uint32 m_lastCursorToggle = 0;
            bool m_showCursor = true;

            int m_scrollOffsetX = 0;
    };
}
