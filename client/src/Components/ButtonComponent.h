#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include <functional>

inline constexpr auto BUTTON_SIZE_STANDARD = sf::Vector2f(200, 100);
inline constexpr auto BUTTON_SIZE_EXTENDED = sf::Vector2f(300, 100);
inline constexpr auto BUTTON_SIZE_EXTRA_EXTENDED = sf::Vector2f(500, 100);

class InputHandler;

class ButtonComponent : public BaseComponent
{
public:
	ButtonComponent(const sf::Font& font, const InputHandler& inputHandler);

	void Update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetPosition(const sf::Vector2f& position) override;
	sf::Vector2f GetPosition() const override;
	void SetSize(sf::Vector2f size);
	sf::Vector2f GetSize() const override;
	void SetColor(sf::Color color);
	const TextComponent& GetTextComponent() const;

	void SetOnClickCallback(std::function<void()>& callback);
	void SetButtonText(const std::string& text, const sf::Font& font, const sf::Color& textColor = TEXT_COLOR, unsigned int textSize = TEXT_SIZE);

private:
	sf::RectangleShape m_Shape;
	sf::Color m_IdleColor;

	std::function<void()> m_OnClickCallback;

	const InputHandler& m_Input;
	TextComponent m_Text;

	bool IsMouseOver();
};
