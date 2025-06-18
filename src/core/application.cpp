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

        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if (!window) {
            SDL_Log("Failed to create window: %s", SDL_GetError());
            return;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        if (!renderer) {
            SDL_Log("Failed to create renderer: %s", SDL_GetError());
            SDL_DestroyWindow(window);
            return;
        }

        font = TTF_OpenFont("assets/fonts/arial.ttf", 24);

        if (!font) {
            SDL_Log("Failed to load font: %s", TTF_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            return;
        }

        root = std::make_unique<gui::Widget>(0, 0, width, height);

        auto panel = std::make_unique<gui::Panel>(0, 0, width, height, "mainPanel");
        auto button = std::make_unique<gui::Button>(10, 50, 280, 40, "Click Me", "clickButton");
        auto textBox = std::make_unique<gui::TextBox>(10, 100, 280, 40, font, "inputBox");
        auto textButton = std::make_unique<gui::Button>(10, 150, 280, 40, "Submit", "submitButton");
        
        textButton->setOnClick([textBox = textBox.get()]() {
            SDL_Log("Submitted text: %s", textBox->getText().c_str());
        });

        button->setOnClick([]() {
            SDL_Log("Button clicked!");
        });
    
        button->setFont(font, renderer);
        textButton->setFont(font, renderer);

        panel->addChild(std::make_unique<gui::Label>(10, 10, 280, 30, "Hello, World!", font, SDL_Color{ 255, 0, 0, 255 }, "greetingLabel"));
        panel->addChild(std::move(button));
        panel->addChild(std::move(textBox));
        panel->addChild(std::move(textButton));
        root->addChild(std::move(panel));
    }

    Application::~Application() {
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
    }

    void Application::run() {
        while (running) {
            handleEvents();
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            root->render(renderer);
            SDL_RenderPresent(renderer);

            SDL_Delay(16);
        }
    }

    void Application::onWindowResize(int newWidth, int newHeight) {
        // TODO: Handle widget resizing and update the root widget dimensions

        SDL_SetWindowSize(window, newWidth, newHeight);
        root->width = newWidth;
        root->height = newHeight;
        SDL_RenderSetViewport(renderer, nullptr);
    }

    void Application::handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) 
                running = false;

            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int newWidth = event.window.data1;
                    int newHeight = event.window.data2;
                    onWindowResize(newWidth, newHeight);
                }
            }
            root->handleEvent(event);
        }
    }
}