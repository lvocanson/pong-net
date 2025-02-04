#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include <functional>

#define BUTTON_SIZE sf::Vector2f(200, 100)

class ButtonComponent : public BaseComponent
{
public:
    ButtonComponent(const sf::Vector2f pos, const sf::Color& idleColor, const sf::Vector2f size = BUTTON_SIZE);
    ~ButtonComponent() override;

    void Update(float dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void SetPosition(const sf::Vector2f& position) override;
    sf::Vector2f GetPosition() const override;
    sf::Vector2f GetSize() const override;
    const TextComponent* GetTextComponent() const;

    void SetOnClickCallback(std::function<void()> onClickCallback);
    void SetButtonText(const std::string& text, sf::Font& font, const sf::Color& textColor = TEXT_COLOR, unsigned int textSize = TEXT_SIZE, TextAlignment textAlignment = TEXT_DEFAULT_ALIGNEMENT);

private:
    sf::RectangleShape m_Shape;
    sf::Color m_IdleColor;
    sf::Color m_HoverColor;
    sf::Color m_ClickedColor;

    std::function<void()> onClickCallback;

    TextComponent* m_Text = nullptr;;

    bool IsMouseOver();
};
