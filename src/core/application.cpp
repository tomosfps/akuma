#include "core/application.h"
#include "gui/widget.h"
#include "gui/panel.h"
#include "gui/button.h"
#include "gui/label.h"
#include "gui/textbox.h"
#include "gui/boxlayout.h"
#include "gui/tabs.h"

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
        auto tabs = std::make_unique<gui::Tabs>(0, 0, width, height, m_font, m_renderer, gui::BoxLayout::HORIZONTAL, "mainTabs");
   
        auto testingTab = std::make_unique<gui::Panel>(0, 0, width, height - 40, "testingTab");
        auto testToolbar = std::make_unique<gui::BoxLayoutManager>(0, 0, width, 40, gui::BoxLayout::HORIZONTAL, "testToolbar");
        auto testBtn1 = std::make_unique<gui::Button>(0, 0, 120, 40, "Test Button", "testBtn1");
        testBtn1->setFont(m_font, m_renderer);
        testBtn1->setOnClick([]() {
            SDL_Log("Test button clicked!");
        });
        testToolbar->addChild(std::move(testBtn1));
        
        auto testBtn2 = std::make_unique<gui::Button>(0, 0, 120, 40, "Debug Mode", "testBtn2");
        testBtn2->setFont(m_font, m_renderer);
        testBtn2->setOnClick([]() {
            SDL_Log("Debug mode toggled!");
        });
        testToolbar->addChild(std::move(testBtn2));
        testingTab->addChild(std::move(testToolbar));
        testingTab->addChild(std::make_unique<gui::Label>(20, 60, 300, 30, "GUI Component Testing", m_font, SDL_Color{ 255, 255, 255, 255 }, "testTitle"));
        
        auto testInput = std::make_unique<gui::TextBox>(20, 100, 250, 35, m_font, "testInput");
        auto testSubmit = std::make_unique<gui::Button>(280, 100, 100, 35, "Test", "testSubmit");
        testSubmit->setFont(m_font, m_renderer);
        testSubmit->setOnClick([testInput = testInput.get()]() {
            SDL_Log("Test input: %s", testInput->getText().c_str());
        });
        testingTab->addChild(std::move(testInput));
        testingTab->addChild(std::move(testSubmit));

        core::texture_ptr testJPG1(IMG_LoadTexture(m_renderer, "assets/images/test1.jpg"), SDL_DestroyTexture);
        core::texture_ptr testPNG1(IMG_LoadTexture(m_renderer, "assets/images/test1.png"), SDL_DestroyTexture);
        
        // Image toggle button 1
        auto imageBtn1 = std::make_unique<gui::Button>(20, 150, 150, 100, "Toggle Image 1", "imageBtn1");
        imageBtn1->setTexture(testJPG1.get());
        m_textures.push_back(std::move(testJPG1));
        
        imageBtn1->setOnClick([this, imageBtn1 = imageBtn1.get()]() {
            SDL_Log("JPG Button clicked!");
            core::texture_ptr newTexture(IMG_LoadTexture(m_renderer, "assets/images/test2.jpg"), SDL_DestroyTexture);

            if (newTexture) {
                imageBtn1->setTexture(newTexture.get());
                m_textures.push_back(std::move(newTexture));
            } else {
                SDL_Log("Failed to load new JPG texture: %s", IMG_GetError());
                return;
            }
        });
        testingTab->addChild(std::move(imageBtn1));

        auto imageBtn2 = std::make_unique<gui::Button>(180, 150, 150, 100, "Toggle Image 2", "imageBtn2");
        imageBtn2->setTexture(testPNG1.get());
        m_textures.push_back(std::move(testPNG1));
        
        imageBtn2->setOnClick([this, imageBtn2 = imageBtn2.get()]() {
            SDL_Log("PNG Button clicked!");
            core::texture_ptr newTexture(IMG_LoadTexture(m_renderer, "assets/images/test2.png"), SDL_DestroyTexture);

            if (newTexture) {
                imageBtn2->setTexture(newTexture.get());
                m_textures.push_back(std::move(newTexture));
            } else {
                SDL_Log("Failed to load new JPG texture: %s", IMG_GetError());
                return;
            }
        });

        testingTab->addChild(std::move(imageBtn2));
        testingTab->addChild(std::make_unique<gui::Label>(20, 270, 400, 30, "Click the button to change this text!", m_font, SDL_Color{ 255, 255, 0, 255 }, "dynamicLabel"));

        auto changeLabelBtn = std::make_unique<gui::Button>(20, 310, 200, 40, "Change Label Text", "changeLabelBtn");
        changeLabelBtn->setFont(m_font, m_renderer);
        
        changeLabelBtn->setOnClick([this]() {
            auto* label = static_cast<gui::Label*>(gui::Widget::getChild("dynamicLabel"));
            if (label) {
                static std::vector<std::string> labelTexts = {
                    "Text changed successfully!",
                    "This is the second message!",
                    "Testing dynamic label updates...",
                    "Labels work perfectly!",
                    "Back to the beginning!"
                };
                
                label->setText(labelTexts[m_labelClickCount % labelTexts.size()]);
                SDL_Log("Label text changed to: %s", labelTexts[m_labelClickCount % labelTexts.size()].c_str());
                m_labelClickCount++;
            }
        });
        testingTab->addChild(std::move(changeLabelBtn));

        auto loginTab = std::make_unique<gui::Panel>(0, 0, width, height - 40, "loginTab");
        int centerX = width / 2 - 150;
        int startY = height / 2 - 120;
        
        loginTab->addChild(std::make_unique<gui::Label>(centerX + 120, startY - 40, 400, 40, "Akuma", m_font, SDL_Color{ 255, 0, 0, 255 }, "loginTitle"));
        loginTab->addChild(std::make_unique<gui::Label>(centerX, startY, 300, 25, "Username or Email:", m_font, SDL_Color{ 200, 255, 200, 255 }, "usernameLabel"));

        auto usernameInput = std::make_unique<gui::TextBox>(centerX, startY + 30, 300, 40, m_font, "usernameInput");
        loginTab->addChild(std::make_unique<gui::Label>(centerX, startY + 80, 300, 25, "Password:", m_font, SDL_Color{ 200, 255, 200, 255 }, "passwordLabel"));
        auto passwordInput = std::make_unique<gui::TextBox>(centerX, startY + 110, 300, 40, m_font, "passwordInput");

        auto loginButton = std::make_unique<gui::Button>(centerX, startY + 170, 300, 45, "Login", "loginButton");
        loginButton->setFont(m_font, m_renderer);
        loginButton->setOnClick([usernameInput = usernameInput.get(), passwordInput = passwordInput.get()]() {
            SDL_Log("Login attempt - Username: %s, Password: %s", 
                    usernameInput->getText().c_str(), 
                    passwordInput->getText().c_str());
        });
        
        auto registerButton = std::make_unique<gui::Button>(centerX + 50, startY + 230, 200, 60, "Create Account", "registerButton");
        registerButton->setFont(m_font, m_renderer);
        registerButton->setOnClick([]() {
            SDL_Log("Redirect to registration page");
        });
        
        loginTab->addChild(std::move(usernameInput));
        loginTab->addChild(std::move(passwordInput));
        loginTab->addChild(std::move(loginButton));
        loginTab->addChild(std::move(registerButton));

        tabs->addTab("Testing", std::move(testingTab));
        tabs->addTab("Login", std::move(loginTab));
        
        panel->addChild(std::move(tabs));
        m_root->addChild(std::move(panel));
    }

    SDL_Texture* Application::loadTexture(const std::string& path) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            SDL_Log("Failed to load image %s: %s", path.c_str(), IMG_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            SDL_Log("Failed to create texture from %s: %s", path.c_str(), SDL_GetError());
            return nullptr;
        }

        // Store texture in the managed vector
        m_textures.emplace_back(texture, [](SDL_Texture* t) { 
            if (t) SDL_DestroyTexture(t); 
        });

        return texture;
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