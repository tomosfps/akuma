#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <gui/widget.h>

namespace core {
    class Application {
        public:
            Application(int width, int height, const std::string& title);
            ~Application();
            void run();
            void onWindowResize(int newWidth, int newHeight);

        private:
            SDL_Window* window;
            SDL_Renderer* renderer;
            TTF_Font* font;
            std::unique_ptr<gui::Widget> root;
            bool running = true;

            void handleEvents();
            void render();
    };
}