#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <gui/widget.h>

namespace core {
    using texture_ptr = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>;

    class Application {
        public:
            Application(int width, int height, const std::string& title);
            ~Application();
            void run();
            void onWindowResize(int newWidth, int newHeight);

        private:
            SDL_Window* m_window;
            SDL_Renderer* m_renderer;
            TTF_Font* m_font;
            std::unique_ptr<gui::Widget> m_root;
            std::vector<core::texture_ptr> m_textures;
            bool m_running = true;

            // testing variable for changing text
            int m_labelClickCount = 0;

            void handleEvents();
            void render();
            
        public:
            SDL_Texture* loadTexture(const std::string& path);
    };
}