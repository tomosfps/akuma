#include "gui/label.h"

namespace gui {
    Label::Label(int x, int y, int width, int height, const std::string& text, TTF_Font* font, SDL_Color colour, const std::string& id)
        : Widget(x, y, width, height, id), m_text(text), m_font(font), m_colour(colour) {}

    void Label::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        if (m_font == nullptr || m_text.empty() || !isVisible()) return;

        SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_text.c_str(), m_colour);
        if (surface == nullptr) return;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            SDL_FreeSurface(surface);
            return;
        }

        SDL_Rect dstRect = { offsetX + m_x, offsetY + m_y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        Widget::render(renderer, offsetX, offsetY);
    }

    void Label::setText(const std::string& text) {
        m_text = text;
    }
}