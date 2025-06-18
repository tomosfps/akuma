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
            const std::string& getText() const { return text; };

            void setFocused(bool focused) { focused = focused; }
            bool isFocused() const { return focused; }

        private:
            std::string text;
            TTF_Font* font;
            SDL_Color textColour = { 0, 0, 0, 255 };
            SDL_Color boxColour = { 255, 255, 255, 255 };
            SDL_Color borderColour = { 0, 0, 0, 255 };

            SDL_Texture* texture = nullptr;
            void updateTexture(SDL_Renderer* renderer);

            size_t cursorPosition = 0;
            bool focused = false;
            Uint32 lastCursorToggle = 0;
            bool showCursor = true;

            int scrollOffsetX = 0;
    };
}
