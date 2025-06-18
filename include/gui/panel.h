#pragma once

#include "widget.h"

namespace gui {
    class Panel : public Widget {
        public:
            Panel(int x, int y, int w, int h, const std::string& id = "");
            void render(SDL_Renderer* renderer, int offsetX = 0, int offsetY = 0) override;
    };
}