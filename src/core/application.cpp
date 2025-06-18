#include "core/application.h"
#include "gui/widget.h"
#include "gui/panel.h"
#include "gui/button.h"
#include "gui/label.h"
#include "gui/textbox.h"

namespace core {
    Application::Application(int width, int height, const std::string& title) {
        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

        m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if (!m_window) {
            SDL_Log("Failed to create window: %s", SDL_GetError());
            return;
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

        if (!m_renderer) {
            SDL_Log("Failed to create renderer: %s", SDL_GetError());
            SDL_DestroyWindow(m_window);
            return;
        }

        m_font = TTF_OpenFont("assets/fonts/arial.ttf", 24);

        if (!m_font) {
            SDL_Log("Failed to load font: %s", TTF_GetError());
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
            return;
        }

        m_root = std::make_unique<gui::Widget>(0, 0, width, height);

        auto panel = std::make_unique<gui::Panel>(0, 0, width, height, "mainPanel");
        auto secondPanel = std::make_unique<gui::Panel>(0, 0, width, height, "secondPanel");
        
        auto clickButton = std::make_unique<gui::Button>(10, 50, 280, 40, "Click Me", "clickButton");
        auto textBox = std::make_unique<gui::TextBox>(10, 100, 280, 40, m_font, "inputBox");
        auto textButton = std::make_unique<gui::Button>(10, 150, 280, 40, "Submit", "submitButton");
        auto imageButtonJPG = std::make_unique<gui::Button>(500, 50, 180, 180, "Image Button JPG", "imageButtonJPG");
        auto imageButtonPNG = std::make_unique<gui::Button>(500, 250, 180, 180, "Image Button PNG", "imageButtonPNG");

        core::texture_ptr jpgTexture(IMG_LoadTexture(m_renderer, "assets/images/test1.jpg"), SDL_DestroyTexture);
        core::texture_ptr pngTexture(IMG_LoadTexture(m_renderer, "assets/images/test1.png"), SDL_DestroyTexture);

        imageButtonJPG->setTexture(jpgTexture.get());
        imageButtonPNG->setTexture(pngTexture.get());
        
        m_textures.push_back(std::move(jpgTexture));
        m_textures.push_back(std::move(pngTexture));
                
        textButton->setOnClick([textBox = textBox.get()]() {
            SDL_Log("Submitted text: %s", textBox->getText().c_str());
        });

        clickButton->setOnClick([]() {
            SDL_Log("Button clicked!");
        });

        imageButtonJPG->setOnClick([this, imageButtonJPG = imageButtonJPG.get()]() {
            SDL_Log("JPG Button clicked!");

            core::texture_ptr newTexture(IMG_LoadTexture(m_renderer, "assets/images/test2.jpg"), SDL_DestroyTexture);

            if (newTexture) {
                imageButtonJPG->setTexture(newTexture.get());
                m_textures.push_back(std::move(newTexture));
            } else {
                SDL_Log("Failed to load new JPG texture: %s", IMG_GetError());
                return;
            }
        });

        imageButtonPNG->setOnClick([this, imageButtonPNG = imageButtonPNG.get()]() {
            SDL_Log("PNG Button clicked!");

            core::texture_ptr newTexture(IMG_LoadTexture(m_renderer, "assets/images/test2.png"), SDL_DestroyTexture);

            if (newTexture) {
                imageButtonPNG->setTexture(newTexture.get());
                m_textures.push_back(std::move(newTexture));
            } else {
                SDL_Log("Failed to load new JPG texture: %s", IMG_GetError());
                return;
            }
        });
    
        clickButton->setFont(m_font, m_renderer);
        textButton->setFont(m_font, m_renderer);

        panel->addChild(std::make_unique<gui::Label>(10, 10, 280, 30, "Hello, World!", m_font, SDL_Color{ 255, 0, 0, 255 }, "greetingLabel"));
        panel->addChild(std::move(clickButton));
        panel->addChild(std::move(textBox));
        panel->addChild(std::move(textButton));
        panel->addChild(std::move(imageButtonJPG));
        panel->addChild(std::move(imageButtonPNG));

        m_root->addChild(std::move(panel));
    }

    Application::~Application() {
        m_textures.clear();
        TTF_CloseFont(m_font);
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
    }

    void Application::run() {
        while (m_running) {
            handleEvents();
            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
            SDL_RenderClear(m_renderer);

            m_root->render(m_renderer);
            SDL_RenderPresent(m_renderer);
            SDL_Delay(16);
        }
    }

    void Application::onWindowResize(int newWidth, int newHeight) {
        // TODO: Handle widget resizing and update the root widget dimensions

        SDL_SetWindowSize(m_window, newWidth, newHeight);
        m_root->m_width = newWidth;
        m_root->m_height = newHeight;
        SDL_RenderSetViewport(m_renderer, nullptr);
    }

    void Application::handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) 
                m_running = false;

            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int newWidth = event.window.data1;
                    int newHeight = event.window.data2;
                    onWindowResize(newWidth, newHeight);
                }
            }
            m_root->handleEvent(event);
        }
    }
}