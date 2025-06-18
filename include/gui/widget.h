#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace gui {
    class Widget {
        public:
            std::string m_id;
            int m_x, m_y, m_width, m_height;
            Widget* m_parent = nullptr;
            std::vector<std::unique_ptr<Widget>> m_children;

            Widget(int x, int y, int width, int height, const std::string& id = "");
            virtual ~Widget();

            void addChild(std::unique_ptr<Widget> child);
            static Widget* getChild(const std::string& id);
            static void removeById(const std::string& id);
            static bool setChildVisibility(const std::string& id, bool visible);

            virtual void render(SDL_Renderer* renderer, int offsetX = 0, int offsetY = 0);
            virtual void handleEvent(const SDL_Event& event, int offsetX = 0, int offsetY = 0);
            virtual bool isVisible() const { return m_visible; }
            virtual void setVisible(bool visible) { m_visible = visible; }

        protected:
            bool m_visible = true;
            static std::unordered_map<std::string, Widget*> m_registry;
    };
}