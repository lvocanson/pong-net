#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include <functional>

#define BUTTON_SIZE_STANDARD sf::Vector2f(200, 100)
#define BUTTON_SIZE_EXTENDED sf::Vector2f(300, 100)
#define BUTTON_SIZE_EXTRA_EXTENDED sf::Vector2f(500, 100)

class InputHandler;

class ButtonComponent : public BaseComponent
{
public:
    ButtonComponent(const sf::Vector2f pos, const sf::Font& font, const sf::Color& idleColor, InputHandler* inputHandler, const sf::Vector2f size = BUTTON_SIZE_EXTENDED);
    ~ButtonComponent() override;

    void Update(float dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void SetPosition(const sf::Vector2f& position) override;
    sf::Vector2f GetPosition() const override;
    sf::Vector2f GetSize() const override;
    const TextComponent& GetTextComponent() const;

    void SetOnClickCallback(std::function<void()> onClickCallback);
    void SetButtonText(const std::string& text, const sf::Font& font, const sf::Color& textColor = TEXT_COLOR, unsigned int textSize = TEXT_SIZE, TextAlignment textAlignment = TEXT_DEFAULT_ALIGNEMENT);

private:
    sf::RectangleShape m_Shape;
    sf::Color m_IdleColor;
    sf::Color m_HoverColor;
    sf::Color m_ClickedColor;

    std::function<void()> onClickCallback;

    InputHandler* m_inputHandler;
    TextComponent m_Text;

    bool IsMouseOver();
};
