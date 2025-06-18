#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace gui {
    class Widget {
        public:
            std::string id;
            int x, y, width, height;
            Widget* parent = nullptr;
            std::vector<std::unique_ptr<Widget>> children;

            Widget(int x, int y, int width, int height, const std::string& id = "");
            virtual ~Widget();

            void addChild(std::unique_ptr<Widget> child);
            static Widget* getChild(const std::string& id);
            static void removeById(const std::string& id);

            virtual void render(SDL_Renderer* renderer, int offsetX = 0, int offsetY = 0);
            virtual void handleEvent(const SDL_Event& event, int offsetX = 0, int offsetY = 0);

        protected:
            static std::unordered_map<std::string, Widget*> registry;
    };
}