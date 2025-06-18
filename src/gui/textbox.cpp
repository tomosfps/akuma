#include "gui/textbox.h"
#include <SDL2/SDL.h>

namespace gui {
    TextBox::TextBox(int x, int y, int width, int height, TTF_Font* font, const std::string& id)
        : Widget(x, y, width, height, id), font(font) {}

    void TextBox::setText(const std::string& text) {
        this->text = text;
        cursorPosition = text.size();
    }

    void TextBox::updateTexture(SDL_Renderer* renderer) {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        if (text.empty()) return;

        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColour);

        if (!surface) {
            SDL_Log("Failed to create text surface: %s", TTF_GetError());
            return;
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        }

        SDL_FreeSurface(surface);
    }

    void TextBox::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        SDL_Rect boxRect = { offsetX + x, offsetY + y, width, height };

        SDL_SetRenderDrawColor(renderer, boxColour.r, boxColour.g, boxColour.b, boxColour.a);
        SDL_RenderFillRect(renderer, &boxRect);

        SDL_SetRenderDrawColor(renderer, borderColour.r, borderColour.g, borderColour.b, borderColour.a);
        SDL_RenderDrawRect(renderer, &boxRect);

        if (!text.empty()) {
            if (!texture) updateTexture(renderer);

            int textWidth, textHeight;
            SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);
            SDL_RenderSetClipRect(renderer, &boxRect);
            SDL_Rect textRect = { offsetX + x + 5 - scrollOffsetX, offsetY + y + (height - textHeight) / 2, textWidth, textHeight };
            SDL_RenderCopy(renderer, texture, nullptr, &textRect);
            SDL_RenderSetClipRect(renderer, nullptr);
        }

        if (focused) {
            std::string subStr = text.substr(0, cursorPosition);
            int cursorX = 5 - scrollOffsetX;
            if (!subStr.empty()) {
                SDL_Surface* surface = TTF_RenderText_Blended(font, subStr.c_str(), textColour);
                if (surface) {
                    cursorX += surface->w;
                    SDL_FreeSurface(surface);
                }
            }

            SDL_SetRenderDrawColor(renderer, textColour.r, textColour.g, textColour.b, textColour.a);
            SDL_Rect cursorRect = { offsetX + x + cursorX, offsetY + y + 5, 2, height - 10 };
            SDL_RenderFillRect(renderer, &cursorRect);
        }
    }

    void TextBox::handleEvent(const SDL_Event& event, int offsetX, int offsetY) {
        SDL_Rect boxRect = { offsetX + x, offsetY + y, width, height };
        SDL_Point mousePoint;

        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                mousePoint = { event.button.x - offsetX, event.button.y - offsetY };
                focused = SDL_PointInRect(&mousePoint, &boxRect);
                break;

            case SDL_TEXTINPUT:
                if (focused) {
                    text.insert(cursorPosition, event.text.text);
                    cursorPosition += strlen(event.text.text);
                    if (texture) {
                        SDL_DestroyTexture(texture);
                        texture = nullptr;
                    }
                }
                break;

            case SDL_KEYDOWN:
                if (focused) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && cursorPosition > 0) {
                        text.erase(cursorPosition - 1, 1);
                        cursorPosition--;
                        if (texture) {
                            SDL_DestroyTexture(texture);
                            texture = nullptr;
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_DELETE && cursorPosition < text.size()) {
                        text.erase(cursorPosition, 1);
                        if (texture) {
                            SDL_DestroyTexture(texture);
                            texture = nullptr;
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_LEFT && cursorPosition > 0) {
                        cursorPosition--;
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT && cursorPosition < text.size()) {
                        cursorPosition++;
                    }
                }
                break;

            default:
                break;
        }

        if (font) {
            std::string subStr = text.substr(0, cursorPosition);
            int cursorPixelX = 0;
            if (!subStr.empty()) {
                TTF_SizeText(font, subStr.c_str(), &cursorPixelX, nullptr);
            }

            const int paddingLeft = 5;
            const int paddingRight = 5;
            int visibleWidth = width - paddingLeft - paddingRight;

            if (cursorPixelX - scrollOffsetX > visibleWidth) {
                scrollOffsetX = cursorPixelX - visibleWidth;
            } else if (cursorPixelX - scrollOffsetX < 0) {
                scrollOffsetX = cursorPixelX;
                if (scrollOffsetX < 0) scrollOffsetX = 0;
            }
        }

        Widget::handleEvent(event, offsetX, offsetY);
    }

}