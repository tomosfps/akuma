#pragma once

#include "gui/widget.h"
#include "gui/boxlayout.h"
#include "gui/button.h"
#include <unordered_map>

namespace gui {
    class Tabs : public Widget {
        public:
            Tabs(int x, int y, int width, int height, TTF_Font* font, SDL_Renderer* renderer, BoxLayout layout = BoxLayout::HORIZONTAL, const std::string& id = "");
            void addTab(const std::string& title, std::unique_ptr<Widget> content);
            void removeTab(const std::string& title);
            void render(SDL_Renderer* renderer, int offsetX = 0, int offsetY = 0) override;

        protected:
            static constexpr int s_tabHeight = 40;

        private:
            std::unique_ptr<BoxLayoutManager> m_tabBar;
            BoxLayoutManager* m_tabBarPtr = nullptr;
            std::unordered_map<std::string, Widget*> m_tabs;
            std::string m_activeTab;
            TTF_Font* m_font;
            SDL_Renderer* m_renderer;
    };
}