#include "gui/widget.h"
#include <algorithm>

namespace gui {
    std::unordered_map<std::string, Widget*> Widget::registry;

    Widget::Widget(int x, int y, int width, int height, const std::string& id)
        : x(x), y(y), width(width), height(height) {
        if (!id.empty()) 
            registry[id] = this;
    }

    Widget::~Widget() {
        if (!id.empty()) {
            registry.erase(id);
        }
    }

    void Widget::addChild(std::unique_ptr<Widget> child) {
        if (!child->id.empty()) registry[child->id] = child.get();
        child->parent = this;
        children.push_back(std::move(child));
    }

    Widget* Widget::getChild(const std::string& id) {
        auto it = registry.find(id);
        return it != registry.end() ? it->second : nullptr;
    }

    void Widget::removeById(const std::string& id) {
        Widget* widget = getChild(id);
        if (!widget || !widget->parent) return;

        auto& siblings = widget->parent->children;
        siblings.erase(std::remove_if(siblings.begin(), siblings.end(),
            [&](const std::unique_ptr<Widget>& child) {
                return child.get() == widget;
            }), siblings.end());

        registry.erase(id);
    }

    void Widget::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        for (const auto& child : children) {
            child->render(renderer, offsetX + x, offsetY + y);
        }
    }

    void Widget::handleEvent(const SDL_Event& event, int offsetX, int offsetY) {
        for (const auto& child : children) {
            child->handleEvent(event, offsetX + x, offsetY + y);
        }
    }
}