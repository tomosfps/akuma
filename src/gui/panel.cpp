#include "gui/panel.h"
#include <SDL2/SDL.h>

namespace gui {
    Panel::Panel(int x, int y, int w, int h, const std::string& id)
        : Widget(x, y, w, h, id) {}

    void Panel::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        if (!isVisible()) return;

        SDL_Rect rect = { offsetX + m_x, offsetY + m_y, m_width, m_height };
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &rect);
        Widget::render(renderer, offsetX, offsetY);
    }
}