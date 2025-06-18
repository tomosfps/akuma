#pragma once

#include "widget.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>

namespace gui {
    class Button : public Widget {
        public:
            std::string label;
            bool isClicked = false;
            bool isHovered = false;
            std::function<void()> onClick;

            Button(int x, int y, int width, int height, const std::string& label = "", const std::string& id = "");
            void render(SDL_Renderer* renderer, int offsetX = 0, int offsetY = 0) override;
            void handleEvent(const SDL_Event& event, int offsetX = 0, int offsetY = 0) override;
            
            void setTexture(SDL_Texture* newTexture);
            void setFont(TTF_Font* newFont, SDL_Renderer* renderer);
            void setOnClick(std::function<void()> callback);

        private:
            SDL_Color defaultColour = { 255, 200, 200, 255 };
            SDL_Color pressedColour = { 150, 150, 150, 255 };
            SDL_Color hoverColour = { 200, 200, 255, 255 };
            SDL_Color textColour = { 0, 0, 0, 255 };
            SDL_Texture* texture = nullptr;

            SDL_Texture* labelTexture = nullptr;
            int labelWidth = 0;
            int labelHeight = 0;
            TTF_Font* font = nullptr;
    };
}