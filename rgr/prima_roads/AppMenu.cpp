#include "stdafx.h"
#include "AppMenu.h"
#include <boost/range/numeric.hpp>

#ifdef max
#undef max
#endif

namespace
{
const sf::Color ENABLED_COLOR(0, 0, 0);
const sf::Color DISABLED_COLOR(120, 120, 120);
const sf::Color POPUP_BACK_COLOR(250, 250, 250);
const sf::Color MENU_BACK_COLOR(230, 220, 220);
const float MIN_POPUP_WIDTH = 100;
const float ACTION_HEIGHT = 22;
const unsigned FONT_SIZE = 16;
const float TEXT_PADDING_X = 5;
const float TEXT_PADDING_Y = 0;

class CTransformTranslateScope
{
public:
    CTransformTranslateScope(sf::Transform &transform, sf::Vector2f delta)
        : m_transform(transform)
        , m_delta(delta)
    {
        m_transform.translate(m_delta.x, m_delta.y);
    }

    ~CTransformTranslateScope()
    {
        m_transform.translate(-m_delta.x, -m_delta.y);
    }

private:
    sf::Transform &m_transform;
    sf::Vector2f m_delta;
};
}

CAppMenu::CAppMenu(const std::string &caption)
{
    m_font.loadFromFile("res/Ubuntu-R.ttf");
    m_shape.setFillColor(MENU_BACK_COLOR);
    m_popup.setFillColor(POPUP_BACK_COLOR);
    InitTitle(caption);
    SetFrame(m_title.getLocalBounds());
}

size_t CAppMenu::AddAction(const std::string &caption, const CAppMenu::ActionHandler &handler)
{
    SAction action;
    action.m_text = std::make_unique<sf::Text>(caption, m_font);
    action.m_handler = handler;
    action.m_text->setPosition(TEXT_PADDING_X, TEXT_PADDING_Y + ACTION_HEIGHT * float(m_actions.size()));
    action.m_text->setCharacterSize(FONT_SIZE);
    action.m_text->setColor(ENABLED_COLOR);

    m_actions.emplace_back(std::move(action));
    m_popup.setSize(GetPopupSize());

    return m_actions.size() - 1;
}

void CAppMenu::SetActionEnabled(size_t index, bool enabled)
{
    m_actions[index].m_enabled = enabled;
    m_actions[index].m_text->setColor(enabled ? ENABLED_COLOR : DISABLED_COLOR);
}

void CAppMenu::SetFrame(const sf::FloatRect &frame)
{
    m_frame = frame;
    m_shape.setPosition(frame.left, frame.top);
    m_shape.setSize(sf::Vector2f(frame.width, frame.height));
    m_title.setPosition(TEXT_PADDING_X + m_frame.left, TEXT_PADDING_Y + m_frame.top);
}

bool CAppMenu::OnEvent(const sf::Event &event)
{
    bool handled = false;
    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
    {
        const auto &data = event.mouseButton;
		sf::Vector2f mousePos(float(data.x), float(data.y));
        m_mousePressCaptured = m_frame.contains(mousePos)
                || (m_isOpen && GetPopupFrame().contains(mousePos));
        break;
    }
    case sf::Event::MouseButtonReleased:
    {
        const auto &data = event.mouseButton;
		sf::Vector2f mousePos(float(data.x), float(data.y));
        if (m_mousePressCaptured)
        {
            if (m_frame.contains(mousePos))
            {
                OnMenuClicked();
                handled = true;
            }
            else if (m_isOpen)
            {
                sf::FloatRect popupFrame = GetPopupFrame();
                if (popupFrame.contains(mousePos))
                {
                    OnPopupClicked(mousePos.x - popupFrame.left, mousePos.y - popupFrame.top);
                    handled = true;
                }
                else
                {
                    m_isOpen = false;
                }
            }
        }
        m_mousePressCaptured = false;
        break;
    }
    default:
        break;
    }
    return handled;
}

void CAppMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
    target.draw(m_title, states);
    if (m_isOpen)
    {
        CTransformTranslateScope scope(states.transform, GetPopupPosition());
        target.draw(m_popup, states);
        for (const SAction &action : m_actions)
        {
            target.draw(*action.m_text, states);
        }
    }
}

void CAppMenu::InitTitle(const std::string &caption)
{
    m_title.setCharacterSize(FONT_SIZE);
    m_title.setFont(m_font);
    m_title.setString(caption);
    m_title.setColor(ENABLED_COLOR);
}

void CAppMenu::OnMenuClicked()
{
    m_isOpen = !m_isOpen;
}

void CAppMenu::OnPopupClicked(float x, float y)
{
    (void)x;
    size_t actionIndex = size_t(y / ACTION_HEIGHT) % m_actions.size();
    SAction &action = m_actions[actionIndex];
    if (action.m_enabled && action.m_handler)
    {
        action.m_handler();
    }
    m_isOpen = false;
}

sf::Vector2f CAppMenu::GetPopupSize() const
{
    auto accumulateFn = [](float maxWidth, const SAction &action) {
        return std::max(maxWidth, 2.f * TEXT_PADDING_X + action.m_text->getLocalBounds().width);
    };
    float maxTextWidth = boost::accumulate(m_actions, MIN_POPUP_WIDTH, accumulateFn);
    return sf::Vector2f(maxTextWidth, ACTION_HEIGHT * float(m_actions.size()));
}

sf::Vector2f CAppMenu::GetPopupPosition() const
{
    return sf::Vector2f(m_frame.left, m_frame.top + m_frame.height);
}

sf::FloatRect CAppMenu::GetPopupFrame() const
{
    sf::Vector2f pos = GetPopupPosition();
    sf::Vector2f size = GetPopupSize();
    return sf::FloatRect(pos.x, pos.y, size.x, size.y);
}
