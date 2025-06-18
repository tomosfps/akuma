#pragma once

#include "widget.h"

namespace gui {
    enum BoxLayout {
        HORIZONTAL,
        VERTICAL
    };

    class BoxLayoutManager : public Widget {
        public:
            BoxLayoutManager(int x, int y, int width, int height, BoxLayout layout, const std::string& id = "");

            void addChild(std::unique_ptr<Widget> child) override;
            void render(SDL_Renderer* renderer, int offsetX = 0, int offsetY = 0) override;
        
        private:
            BoxLayout m_layout;
            int m_spacing = 10;
    };
}