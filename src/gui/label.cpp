#include "gui/label.h"

namespace gui {
    Label::Label(int x, int y, int width, int height, const std::string& text, TTF_Font* font, SDL_Color colour, const std::string& id)
        : Widget(x, y, width, height, id), text(text), font(font), colour(colour) {}

    void Label::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        if (font == nullptr || text.empty()) return;

        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), colour);
        if (surface == nullptr) return;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            SDL_FreeSurface(surface);
            return;
        }

        SDL_Rect dstRect = { offsetX + x, offsetY + y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        Widget::render(renderer, offsetX, offsetY);
    }
}