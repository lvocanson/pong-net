#include "InsertFieldComponent.h"
#include <Window/InputHandler.h>
#include <Window/Window.h>
#include <SFML/Window/Keyboard.hpp>

InsertFieldComponent::InsertFieldComponent(const sf::Font& font, const InputHandler& inputHandler)
	: m_CharacterLimit(DEFAULT_CHARACTER_LIMIT)
	, m_Focus(false)
	, m_CursorTimer(0.0f)
	, m_Text(font)
	, m_Label(nullptr)
	, m_Cursor(font)
	, m_ErrorText(font)
	, m_Input(inputHandler)
{
	m_Rectangle.setSize(FIELD_SIZE);
	m_Rectangle.setFillColor(sf::Color(171, 171, 171));
	m_Rectangle.setOutlineColor(sf::Color::White);
	m_Rectangle.setOutlineThickness(1.0f);
	m_Rectangle.setOrigin(m_Rectangle.getLocalBounds().getCenter());

	m_Text.SetColor(sf::Color::Black);

	m_Label = new TextComponent(font);
	m_Label->SetText("InsertField");
	m_Label->SetColor(sf::Color(171, 171, 171));
	m_Label->SetCharacterSize(25);

	m_Cursor.SetText("|");
	m_Cursor.SetColor(sf::Color::Black);
	m_Cursor.SetVisible(false);

	m_ErrorText.SetText("");
	m_ErrorText.SetColor(sf::Color::Red);
	m_ErrorText.SetCharacterSize(25);

	SetPosition(sf::Vector2f(0.0f, 0.0f));
}

InsertFieldComponent::InsertFieldComponent(const sf::Font& font, const InputHandler& inputHandler, const sf::Vector2f& pos, const sf::Vector2f& size,
	const sf::Color& idleColor, const sf::Color& hoverColor,
	float outlineThickness, unsigned int characterLimit)
	: m_CharacterLimit(characterLimit)
	, m_Focus(false)
	, m_CursorTimer(0.0f)
	, m_Text(font)
	, m_Label(nullptr)
	, m_Cursor(font)
	, m_ErrorText(font)
	, m_Input(inputHandler)
{
	m_Label = new TextComponent(font);
	m_Rectangle.setSize(size);
	m_Rectangle.setFillColor(idleColor);
	m_Rectangle.setOutlineColor(hoverColor);
	m_Rectangle.setOutlineThickness(outlineThickness);

	sf::Vector2f newPos = sf::Vector2f(pos.x - (size.x * 0.5f), pos.y - (size.y * 0.5f));
	SetPosition(newPos);
}

void InsertFieldComponent::BlinkCursor(float dt)
{
	if (m_CursorTimer >= CURSOR_BLINK_TIME)
	{
		m_CursorTimer = 0.0f;
		m_Cursor.SetVisible(!m_Cursor.IsVisible());
	}
	else
	{
		m_CursorTimer += dt;
	}
}

void InsertFieldComponent::Update(float dt, Window& window)
{
	if (IsMouseOver(window))
	{
		m_Rectangle.setOutlineThickness(2.0f);

		if (m_Input.IsMouseButtonPressed(sf::Mouse::Button::Left))
		{
			m_Focus = true;
			m_Cursor.SetVisible(true);
		}
	}
	else if (m_Input.IsKeyPressed(sf::Keyboard::Key::Enter) || m_Input.IsMouseButtonPressed(sf::Mouse::Button::Left))
	{
		m_Focus = false;
		m_Rectangle.setOutlineThickness(0.0f);
		m_Cursor.SetVisible(false);
		m_CursorTimer = 0.0f;
	}


	if (!m_Focus) return;

	BlinkCursor(dt);

	if (window.IsFocused())
	{
		AppendCharacter(window.GetMessageEntered());
	}
}

void InsertFieldComponent::SetText(const std::string& text)
{
	//substring text to fit the character limit
	if (text.size() > m_CharacterLimit)
	{
		m_TextContent = text.substr(0, m_CharacterLimit);
	}
	else
	{
		m_TextContent = text;
	}

	m_Text.SetText(m_TextContent);
	ReplaceCursor();
}

void InsertFieldComponent::SetLabel(const std::string& label)
{
	m_Label->SetText(label);
}

void InsertFieldComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Rectangle, states);
	target.draw(m_Text, states);
	target.draw(*m_Label, states);
	target.draw(m_ErrorText, states);

	if (m_Cursor.IsVisible())
		target.draw(m_Cursor, states);
}

bool InsertFieldComponent::IsMouseOver(Window& window)
{
	return m_Rectangle.getGlobalBounds().contains(sf::Vector2f(m_Input.GetMousePosition(window)));
}

void InsertFieldComponent::AppendCharacter(const sf::Event::TextEntered& text)
{
	m_TextContent = m_Text.GetText().getString();

	if (text.unicode == U'\b' && m_TextContent.length() > 0) 
	{
		m_TextContent = m_TextContent.substr(0, m_TextContent.length() - 1);
	}
	else if (text.unicode != U'\b') 
	{
		m_TextContent += text.unicode;
	}
	m_Text.SetText(m_TextContent);
	ReplaceCursor();
}

void InsertFieldComponent::SetPosition(const sf::Vector2f& position)
{
	m_Rectangle.setPosition(position);

	sf::Vector2f RectSize = m_Rectangle.getLocalBounds().size;
	m_Label->SetPosition(m_Rectangle.getPosition() - sf::Vector2f(0, RectSize.y));
	m_ErrorText.SetPosition(m_Rectangle.getPosition() - sf::Vector2f(0, -RectSize.y));

	const float xPos = position.x + m_Rectangle.getOutlineThickness();
	const float yPos = position.y + m_Rectangle.getOutlineThickness();

	m_Text.SetPosition(sf::Vector2f(xPos, yPos));
	ReplaceCursor();
}

void InsertFieldComponent::SetSize(const sf::Vector2f& size)
{
	m_Rectangle.setSize(size);
}

void InsertFieldComponent::ReplaceCursor()
{
	
	m_Cursor.SetPosition(m_Text.GetPosition() + sf::Vector2f(m_Text.GetBounds().x *0.5f , 0.f));
}
