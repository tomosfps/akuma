#include "gui/button.h"
#include <SDL2/SDL.h>

namespace gui {
    Button::Button(int x, int y, int width, int height, const std::string& label, const std::string& id)
        : Widget(x, y, width, height, id), label(label) {}

    void Button::setFont(TTF_Font* newFont, SDL_Renderer* renderer) {
        font = newFont;

        if (labelTexture) {
            SDL_DestroyTexture(labelTexture);
            labelTexture = nullptr;
        }

        if (font && !label.empty()) {
            SDL_Surface* surface = TTF_RenderText_Blended(font, label.c_str(), textColour);
            if (surface) {
                labelTexture = SDL_CreateTextureFromSurface(renderer, surface);
                labelWidth = surface->w;
                labelHeight = surface->h;
                SDL_FreeSurface(surface);
            } else {
                SDL_Log("Failed to create label texture: %s", TTF_GetError());
            }
        }
    }

    void Button::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        SDL_Rect rect = { offsetX + x, offsetY + y, width, height };
    
        if (texture) {
            SDL_RenderCopy(renderer, texture, nullptr, &rect);
        } else {
            SDL_Color color = isClicked ? pressedColour : (isHovered ? hoverColour : defaultColour);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }

        if (labelTexture) {
            SDL_Rect textRect = {
                rect.x + (rect.w - labelWidth) / 2,
                rect.y + (rect.h - labelHeight) / 2,
                labelWidth, labelHeight
            };
            SDL_RenderCopy(renderer, labelTexture, nullptr, &textRect);
        }

        Widget::render(renderer, offsetX, offsetY);
    }

    void Button::handleEvent(const SDL_Event& event, int offsetX, int offsetY) {
        SDL_Rect rect = { offsetX + x, offsetY + y, width, height };
        SDL_Point mousePoint;

        if ((event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) && event.button.button == SDL_BUTTON_LEFT) {
            mousePoint = { event.button.x - offsetX, event.button.y - offsetY };
            bool inside = SDL_PointInRect(&mousePoint, &rect);
            isClicked = inside && event.type == SDL_MOUSEBUTTONDOWN;

            if (event.type == SDL_MOUSEBUTTONUP) {
                isClicked = false;
            }

            if (isClicked && onClick) {
                onClick();
            }
        } 
        
        else if (event.type == SDL_MOUSEMOTION) {
            mousePoint = { event.motion.x - offsetX, event.motion.y - offsetY };
            isHovered = SDL_PointInRect(&mousePoint, &rect);
        } 
        
        else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_LEAVE) {
            isHovered = false;
        }

        Widget::handleEvent(event, offsetX, offsetY);
    }

    void Button::setTexture(SDL_Texture* newTexture) {
        texture = newTexture;
    }

    void Button::setOnClick(std::function<void()> callback) {
        onClick = callback;
    }
}
