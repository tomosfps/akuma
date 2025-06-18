#include "gui/tabs.h"

namespace gui {
    Tabs::Tabs(int x, int y, int width, int height, TTF_Font* font, SDL_Renderer* renderer, BoxLayout layout, const std::string& id) 
    : Widget(x, y, width, height, id), m_font(font), m_renderer(renderer) {
        SDL_assert(m_font && "Tabs font must not be null");
        SDL_assert(m_renderer && "Tabs renderer must not be null");

        m_tabBar = std::make_unique<BoxLayoutManager>(0, 0, width, s_tabHeight, layout, id);
        m_tabBarPtr = m_tabBar.get();
        Widget::addChild(std::move(m_tabBar));
    }

    void Tabs::addTab(const std::string& title, std::unique_ptr<Widget> panelTab) {
        auto tabID = "tab_" + title;

        panelTab->m_x = 0;
        panelTab->m_y = s_tabHeight; 
        panelTab->m_width = m_width;
        panelTab->m_height = m_height - s_tabHeight;
        panelTab->setVisible(m_tabs.empty());
        Widget::addChild(std::move(panelTab));
        m_tabs[tabID] = m_children.back().get();

        int buttonWidth = m_width / std::max(1, static_cast<int>(m_tabs.size() + 1));
        
        auto tabButton = std::make_unique<Button>(0, 0, buttonWidth, s_tabHeight, title, tabID);
        tabButton->setFont(m_font, m_renderer);
        tabButton->setOnClick([this, tabID]() {
            for (auto& [key, panel] : m_tabs) {
                panel->setVisible(false);
            }
            auto it = m_tabs.find(tabID);
            if (it != m_tabs.end()) {
                it->second->setVisible(true);
                m_activeTab = tabID;
            }
        });

        m_tabBarPtr->addChild(std::move(tabButton));
    }

    void Tabs::removeTab(const std::string& title) {
        auto tabID = "tab_" + title;
        auto it = m_tabs.find(tabID);
        if (it != m_tabs.end()) {
            m_tabs.erase(it);
            m_tabBar->removeById(tabID);
            if (m_activeTab == tabID) {
                m_activeTab.clear();
                if (!m_tabs.empty()) {
                    m_tabs.begin()->second->setVisible(true);
                    m_activeTab = m_tabs.begin()->first;
                }
            }
        }
    }

    void Tabs::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        if (!isVisible()) return;
        Widget::render(renderer, offsetX, offsetY);
    }
}