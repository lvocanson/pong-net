#include "ButtonComponent.h"
#include <Window/InputHandler.h>

ButtonComponent::ButtonComponent(const sf::Vector2f pos, const sf::Font& font, const sf::Color& idleColor, InputHandler* inputHandler, const sf::Vector2f size)
    : m_IdleColor(idleColor)
    , m_HoverColor(idleColor - sf::Color(50, 50, 50, 0))
    , m_ClickedColor(idleColor + sf::Color(50, 50, 50, 0))
    , onClickCallback(nullptr)
    , m_inputHandler(inputHandler)
    , m_Text(font)
{
    m_Shape.setSize(size);
    m_Shape.setFillColor(idleColor);

    sf::Vector2f newPos = sf::Vector2f(pos.x - (size.x * 0.5f), pos.y - (size.y * 0.5f));
    m_Shape.setPosition(newPos);
}

ButtonComponent::~ButtonComponent()
{

}

void ButtonComponent::Update(float dt)
{
    if (IsMouseOver())
    {
        m_Shape.setFillColor(m_HoverColor);

        if (m_inputHandler->IsMouseButtonPressed(sf::Mouse::Button::Left) && onClickCallback)
        {
            onClickCallback();
            m_Shape.setFillColor(m_ClickedColor);
        }
    }
    else
    {
        m_Shape.setFillColor(m_IdleColor);
    }
}

void ButtonComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Shape, states);
    target.draw(m_Text, states);
}

bool ButtonComponent::IsMouseOver()
{
    sf::Vector2f mousePos = (sf::Vector2f)m_inputHandler->GetMousePosition();
    sf::Vector2f buttonPos = m_Shape.getPosition();
    sf::Vector2f buttonSize = m_Shape.getSize();

    float Right = buttonPos.x + buttonSize.x;
    float Bottom = buttonPos.y + buttonSize.y;

    return mousePos.x >= buttonPos.x && mousePos.x <= Right &&  mousePos.y >= buttonPos.y && mousePos.y <= Bottom;
}

void ButtonComponent::SetPosition(const sf::Vector2f& position)
{
    m_Shape.setPosition(position);

    sf::FloatRect buttonBounds = m_Shape.getLocalBounds();
    sf::Vector2f buttonTextPosition = position;
    buttonTextPosition.x += (buttonBounds.size.x - m_Text.GetSize().x) / 2.0f;
    buttonTextPosition.y += (buttonBounds.size.y - m_Text.GetSize().y) / 2.0f;

    m_Text.SetPosition(buttonTextPosition);
}

sf::Vector2f ButtonComponent::GetPosition() const
{
    return m_Shape.getPosition();
}

sf::Vector2f ButtonComponent::GetSize() const
{
    return m_Shape.getSize();
}

const TextComponent& ButtonComponent::GetTextComponent() const
{
    return m_Text;
}

void ButtonComponent::SetOnClickCallback(std::function<void()> onClickCallback)
{
    this->onClickCallback = onClickCallback;
}

void ButtonComponent::SetButtonText(const std::string& text, const sf::Font& font, const sf::Color& textColor, unsigned int textSize, TextAlignment textAlignment)
{
    m_Text.SetText(text);
    m_Text.SetColor(textColor);
    m_Text.SetCharacterSize(textSize);
    m_Text.SetTextAlignment(textAlignment);

    sf::Vector2f buttonPosition = GetPosition();
    sf::Vector2f buttonSize = GetSize();
    sf::Vector2f textPosition = buttonPosition + sf::Vector2f((buttonSize.x - m_Text.GetSize().x) / 2.0f, (buttonSize.y - m_Text.GetSize().y) / 2.0f);
    m_Text.SetPosition(textPosition);
}
