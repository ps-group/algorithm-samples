#pragma once
#include "stdafx.h"

// Implements menu bar with simple menu and popup actions list
class CAppMenu : public sf::Drawable
{
public:
    using ActionHandler = std::function<void()>;

    CAppMenu(const std::string &caption);

    size_t AddAction(const std::string &caption, const ActionHandler &handler);
    void SetActionEnabled(size_t index, bool enabled);
    void SetFrame(sf::FloatRect const& frame);
    bool OnEvent(const sf::Event &event);

protected:
    // Drawable interface
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void InitTitle(const std::string &caption);
    void OnMenuClicked();
    void OnPopupClicked(float x, float y);
    sf::Vector2f GetPopupSize()const;
    sf::Vector2f GetPopupPosition()const;
    sf::FloatRect GetPopupFrame()const;

    struct SAction
    {
        std::unique_ptr<sf::Text> m_text;
        bool m_enabled = false;
        ActionHandler m_handler;
    };

    sf::FloatRect m_frame;
    sf::Font m_font;
    sf::Text m_title;
    sf::RectangleShape m_shape;
    sf::RectangleShape m_popup;
    std::vector<SAction> m_actions;
    bool m_isOpen = false;
    bool m_mousePressCaptured = false;
};
