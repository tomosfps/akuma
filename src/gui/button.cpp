#include "gui/button.h"
#include <SDL2/SDL.h>

namespace gui {
    Button::Button(int x, int y, int width, int height, const std::string& label, const std::string& id)
        : Widget(x, y, width, height, id), label(label) {}

    void Button::setFont(TTF_Font* newFont, SDL_Renderer* renderer) {
        m_font = newFont;

        if (m_labelTexture) {
            SDL_DestroyTexture(m_labelTexture);
            m_labelTexture = nullptr;
        }

        if (m_font && !label.empty()) {
            SDL_Surface* surface = TTF_RenderText_Blended(m_font, label.c_str(), m_textColour);
            if (surface) {
                m_labelTexture = SDL_CreateTextureFromSurface(renderer, surface);
                m_labelWidth = surface->w;
                m_labelHeight = surface->h;
                SDL_FreeSurface(surface);
            } else {
                SDL_Log("Failed to create label texture: %s", TTF_GetError());
            }
        }
    }

    void Button::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        if (!isVisible()) return;

        SDL_Rect rect = { offsetX + m_x, offsetY + m_y, m_width, m_height };
    
        if (m_texture) {
            SDL_RenderCopy(renderer, m_texture, nullptr, &rect);
        } else {
            SDL_Color color = m_isClicked ? m_pressedColour : (m_isHovered ? m_hoverColour : m_defaultColour);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }

        if (m_labelTexture) {
            SDL_Rect textRect = {
                rect.x + (rect.w - m_labelWidth) / 2,
                rect.y + (rect.h - m_labelHeight) / 2,
                m_labelWidth, m_labelHeight
            };
            SDL_RenderCopy(renderer, m_labelTexture, nullptr, &textRect);
        }

        Widget::render(renderer, offsetX, offsetY);
    }

    void Button::handleEvent(const SDL_Event& event, int offsetX, int offsetY) {
        if (!isVisible()) return;

        SDL_Rect rect = { offsetX + m_x, offsetY + m_y, m_width, m_height };
        SDL_Point mousePoint;

        if ((event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) && event.button.button == SDL_BUTTON_LEFT) {
            mousePoint = { event.button.x, event.button.y };
            bool inside = SDL_PointInRect(&mousePoint, &rect);
            m_isClicked = inside && event.type == SDL_MOUSEBUTTONDOWN;

            if (event.type == SDL_MOUSEBUTTONUP) {
                m_isClicked = false;
            }

            if (inside && event.type == SDL_MOUSEBUTTONUP && m_onClick) {
                m_onClick();
            }
        } 
        
        else if (event.type == SDL_MOUSEMOTION) {
            mousePoint = { event.motion.x, event.motion.y };
            m_isHovered = SDL_PointInRect(&mousePoint, &rect);
        } 

        else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_LEAVE) {
            m_isHovered = false;
        }

        Widget::handleEvent(event, offsetX, offsetY);
    }

    void Button::setTexture(SDL_Texture* newTexture) {
        m_texture = newTexture;
    }

    void Button::setOnClick(std::function<void()> callback) {
        m_onClick = callback;
    }
}
