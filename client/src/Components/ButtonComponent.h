#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include <functional>

inline constexpr auto BUTTON_SIZE_STANDARD = sf::Vector2f(200, 100);
inline constexpr auto BUTTON_SIZE_EXTENDED = sf::Vector2f(300, 100);
inline constexpr auto BUTTON_SIZE_EXTRA_EXTENDED = sf::Vector2f(500, 100);
inline constexpr auto BUTTON_SIZE_EXTRA_EXTENDED_SMALL_Y = sf::Vector2f(500, 50);
inline constexpr auto BUTTON_LOBBY_SIZE = sf::Vector2f(800, 50);

class InputHandler;
class Window;

class ButtonComponent : public BaseComponent
{
public:
	ButtonComponent(const sf::Font& font, const InputHandler& inputHandler);

	void Update(float dt, Window& window) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetPosition(const sf::Vector2f& position) override;
	sf::Vector2f GetPosition() const override;
	void SetSize(sf::Vector2f size);
	sf::Vector2f GetSize() const override;
	void SetColor(sf::Color color);
	const TextComponent& GetTextComponent() const;

	bool IsPressed() const { return m_IsPressed; }
	void SetButtonText(const std::string& text, const sf::Color& textColor = TEXT_COLOR, unsigned int textSize = TEXT_SIZE);

private:
	sf::RectangleShape m_Shape;
	sf::Color m_IdleColor;

	bool m_IsPressed;
	const InputHandler& m_Input;
	TextComponent m_Text;

	bool IsMouseOver(Window& window);
};
