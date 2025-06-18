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
            bool m_isClicked = false;
            bool m_isHovered = false;
            std::function<void()> m_onClick;

            Button(int x, int y, int width, int height, const std::string& label = "", const std::string& id = "");
            void render(SDL_Renderer* renderer, int offsetX = 0, int offsetY = 0) override;
            void handleEvent(const SDL_Event& event, int offsetX = 0, int offsetY = 0) override;
            
            void setTexture(SDL_Texture* newTexture);
            void setFont(TTF_Font* newFont, SDL_Renderer* renderer);
            void setOnClick(std::function<void()> callback);

        private:
            SDL_Color m_defaultColour = { 255, 200, 200, 255 };
            SDL_Color m_pressedColour = { 150, 150, 150, 255 };
            SDL_Color m_hoverColour = { 200, 200, 255, 255 };
            SDL_Color m_textColour = { 0, 0, 0, 255 };
            SDL_Texture* m_texture = nullptr;

            SDL_Texture* m_labelTexture = nullptr;
            int m_labelWidth = 0;
            int m_labelHeight = 0;
            TTF_Font* m_font = nullptr;
    };
}