#include "gui/widget.h"
#include <algorithm>

namespace gui {
    std::unordered_map<std::string, Widget*> Widget::m_registry;

    Widget::Widget(int x, int y, int width, int height, const std::string& m_id)
        : m_x(x), m_y(y), m_width(width), m_height(height) {
        if (!m_id.empty()) 
            m_registry[m_id] = this;
    }

    Widget::~Widget() {
        if (!m_id.empty()) {
            m_registry.erase(m_id);
        }
    }

    void Widget::addChild(std::unique_ptr<Widget> child) {
        if (!child->m_id.empty()) m_registry[child->m_id] = child.get();
        child->m_parent = this;
        m_children.push_back(std::move(child));
    }

    Widget* Widget::getChild(const std::string& m_id) {
        auto it = m_registry.find(m_id);
        return it != m_registry.end() ? it->second : nullptr;
    }

    void Widget::removeById(const std::string& m_id) {
        Widget* widget = getChild(m_id);
        if (!widget || !widget->m_parent) return;

        auto& siblings = widget->m_parent->m_children;
        siblings.erase(std::remove_if(siblings.begin(), siblings.end(),
            [&](const std::unique_ptr<Widget>& child) {
                return child.get() == widget;
            }), siblings.end());

        m_registry.erase(m_id);
    }

    void Widget::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        if (!m_visible) {
            SDL_Log("Widget %s is not visible, skipping render", m_id.c_str());
            return;
        }

        for (const auto& child : m_children) {
            child->render(renderer, offsetX + m_x, offsetY + m_y);
        }
    }

    void Widget::handleEvent(const SDL_Event& event, int offsetX, int offsetY) {
        for (const auto& child : m_children) {
            child->handleEvent(event, offsetX + m_x, offsetY + m_y);
        }
    }

    bool Widget::setChildVisibility(const std::string& id, bool visible) {
        auto child = getChild(id);
        if (child) {
            child->setVisible(visible);
            return true;
        }
        return false;
    }
}