#include "gui/textbox.h"
#include <SDL2/SDL.h>

namespace gui {
    TextBox::TextBox(int x, int y, int width, int height, TTF_Font* font, const std::string& id)
        : Widget(x, y, width, height, id), m_font(font) {}

    void TextBox::setText(const std::string& text) {
        this->m_text = text;
        m_cursorPosition = text.size();
    }

    void TextBox::updateTexture(SDL_Renderer* renderer) {
        if (m_texture) {
            SDL_DestroyTexture(m_texture);
            m_texture = nullptr;
        }

        if (m_text.empty()) return;

        SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_text.c_str(), m_textColour);

        if (!surface) {
            SDL_Log("Failed to create text surface: %s", TTF_GetError());
            return;
        }

        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!m_texture) {
            SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        }

        SDL_FreeSurface(surface);
    }

    void TextBox::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
        if (!isVisible()) return;

        SDL_Rect boxRect = { offsetX + m_x, offsetY + m_y, m_width, m_height };

        SDL_SetRenderDrawColor(renderer, m_boxColour.r, m_boxColour.g, m_boxColour.b, m_boxColour.a);
        SDL_RenderFillRect(renderer, &boxRect);

        SDL_SetRenderDrawColor(renderer, m_borderColour.r, m_borderColour.g, m_borderColour.b, m_borderColour.a);
        SDL_RenderDrawRect(renderer, &boxRect);

        if (!m_text.empty()) {
            if (!m_texture) updateTexture(renderer);

            int textWidth, textHeight;
            SDL_QueryTexture(m_texture, nullptr, nullptr, &textWidth, &textHeight);
            SDL_RenderSetClipRect(renderer, &boxRect);
            SDL_Rect textRect = { offsetX + m_x + 5 - m_scrollOffsetX, offsetY + m_y + (m_height - textHeight) / 2, textWidth, textHeight };
            SDL_RenderCopy(renderer, m_texture, nullptr, &textRect);
            SDL_RenderSetClipRect(renderer, nullptr);
        }

        if (m_focused) {
            std::string subStr = m_text.substr(0, m_cursorPosition);
            int cursorX = 5 - m_scrollOffsetX;
            if (!subStr.empty()) {
                SDL_Surface* surface = TTF_RenderText_Blended(m_font, subStr.c_str(), m_textColour);
                if (surface) {
                    cursorX += surface->w;
                    SDL_FreeSurface(surface);
                }
            }

            SDL_SetRenderDrawColor(renderer, m_textColour.r, m_textColour.g, m_textColour.b, m_textColour.a);
            SDL_Rect cursorRect = { offsetX + m_x + cursorX, offsetY + m_y + 5, 2, m_height - 10 };
            SDL_RenderFillRect(renderer, &cursorRect);
        }
    }

    void TextBox::handleEvent(const SDL_Event& event, int offsetX, int offsetY) {
        if (!isVisible()) return;

        SDL_Rect boxRect = { offsetX + m_x, offsetY + m_y, m_width, m_height };
        SDL_Point mousePoint;

        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                mousePoint = { event.button.x, event.button.y };
                m_focused = SDL_PointInRect(&mousePoint, &boxRect);
                break;

            case SDL_TEXTINPUT:
                if (m_focused) {
                    m_text.insert(m_cursorPosition, event.text.text);
                    m_cursorPosition += strlen(event.text.text);
                    if (m_texture) {
                        SDL_DestroyTexture(m_texture);
                        m_texture = nullptr;
                    }
                }
                break;

            case SDL_KEYDOWN:
                if (m_focused) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && m_cursorPosition > 0) {
                        m_text.erase(m_cursorPosition - 1, 1);
                        m_cursorPosition--;
                        if (m_texture) {
                            SDL_DestroyTexture(m_texture);
                            m_texture = nullptr;
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_DELETE && m_cursorPosition < m_text.size()) {
                        m_text.erase(m_cursorPosition, 1);
                        if (m_texture) {
                            SDL_DestroyTexture(m_texture);
                            m_texture = nullptr;
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_LEFT && m_cursorPosition > 0) {
                        m_cursorPosition--;
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT && m_cursorPosition < m_text.size()) {
                        m_cursorPosition++;
                    }
                }
                break;

            default:
                break;
        }

        if (m_font) {
            std::string subStr = m_text.substr(0, m_cursorPosition);
            int cursorPixelX = 0;
            if (!subStr.empty()) {
                TTF_SizeText(m_font, subStr.c_str(), &cursorPixelX, nullptr);
            }

            const int paddingLeft = 5;
            const int paddingRight = 5;
            int visibleWidth = m_width - paddingLeft - paddingRight;

            if (cursorPixelX - m_scrollOffsetX > visibleWidth) {
                m_scrollOffsetX = cursorPixelX - visibleWidth;
            } else if (cursorPixelX - m_scrollOffsetX < 0) {
                m_scrollOffsetX = cursorPixelX;
                if (m_scrollOffsetX < 0) m_scrollOffsetX = 0;
            }
        }

        Widget::handleEvent(event, offsetX, offsetY);
    }

}