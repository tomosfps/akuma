#pragma once

#include "widget.h"
#include <SDL2/SDL_ttf.h>

namespace gui {
    class Label : public Widget {
        public:
            std::string text;
            TTF_Font* font;
            SDL_Color colour;

            Label(int x, int y, int width, int height, const std::string& text = "", TTF_Font* font = nullptr, SDL_Color colour = { 255, 255, 255 }, const std::string& id = "");
            void render(SDL_Renderer* renderer, int offsetX = 0, int offsetY = 0) override;
    };
}