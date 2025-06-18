#include "gui/boxlayout.h"

namespace gui {
    BoxLayoutManager::BoxLayoutManager(int x, int y, int width, int height, BoxLayout layout, const std::string& id)
        : Widget(x, y, width, height, id), m_layout(layout) {}

    void BoxLayoutManager::addChild(std::unique_ptr<Widget> child) {
        int offset = 0;
        for (const auto& existingChild : m_children) {
            offset += (m_layout == BoxLayout::HORIZONTAL ? existingChild->m_width : existingChild->m_height) + m_spacing;
        }

        if (m_layout == BoxLayout::HORIZONTAL) {
            child->m_x = m_x + offset;
            child->m_y = 0;
        } else {
            child->m_x = 0;
            child->m_y = m_y + offset;
        }
        Widget::addChild(std::move(child));
    }

    void BoxLayoutManager::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        if (!isVisible()) return;

        for (const auto& child : m_children) {
            child->render(renderer, m_x + offsetX, m_y + offsetY);
        }
    }
}