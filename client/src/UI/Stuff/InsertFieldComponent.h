#pragma once
/*
#include <SFML/Graphics.hpp>
#include <sstream>
#include "TextComponent.h"

constexpr unsigned int DEFAULT_CHARACTER_LIMIT = 20;
constexpr float CURSOR_BLINK_TIME = 0.5f;
inline constexpr auto FIELD_SIZE = sf::Vector2f(500, 30);

class InputHandler;
class Window;

class InsertFieldComponent : public BaseComponent
{
public:

	InsertFieldComponent(const sf::Font& font, const InputHandler& inputHandler);
	InsertFieldComponent(const sf::Font& font, const InputHandler& inputHandler, const sf::Vector2f& pos, const sf::Vector2f& size,
		const sf::Color& idleColor, const sf::Color& hoverColor,
		float outlineThickness, unsigned int characterLimit);

	void Update(float dt, Window& window) override;
	void ClearErrorMessage() { m_ErrorText.SetText(""); }

	void SetText(const std::string& text);
	void SetLabel(const std::string& label);
	void SetPosition(const sf::Vector2f& position) override;
	void SetSize(const sf::Vector2f& size = FIELD_SIZE);
	void SetFillColor(const sf::Color& color) { m_Rectangle.setFillColor(color); }
	void SetHoverColor(const sf::Color& color) { m_Rectangle.setOutlineColor(color); }
	void SetOutlineThickness(float thickness) { m_Rectangle.setOutlineThickness(thickness); }
	void SetCharacterLimit(unsigned int limit) { if (limit > 0) m_CharacterLimit = limit; }
	void ShowErrorMessage(const std::string& message) { m_ErrorText.SetText(message); }

	sf::Vector2f GetPosition() const override { return m_Rectangle.getPosition(); }
	sf::Vector2f GetSize() const override { return m_Rectangle.getSize(); }
	bool GetFocus() const { return m_Focus; }
	std::string_view GetText() const { return m_TextContent; }
	unsigned int GetCharacterLimit() const { return m_CharacterLimit; }
	size_t GetTextSize() const { return m_TextContent.length(); }
	const Text* GetLabelComponent() const { return m_Label; }

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool IsMouseOver(Window& window);
	void ReplaceCursor();
	void AppendCharacter(const sf::Event::TextEntered& text);
	void BlinkCursor(float dt);

private:

	sf::RectangleShape m_Rectangle;
	Text m_Text;
	Text* m_Label;
	Text m_Cursor;
	Text m_ErrorText;
	std::string m_TextContent;
	const InputHandler& m_Input;

	unsigned int m_CharacterLimit;
	bool m_Focus;
	float m_CursorTimer;
};
*/
