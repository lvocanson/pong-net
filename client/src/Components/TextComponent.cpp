#include "TextComponent.h"
//#include "src/core/Managers/Resources/FontRegistry.h"

TextComponent::TextComponent(const sf::Font& font)
	: m_Text(font, "", 50u)
{
	m_Text.setString("");
	//m_Text.setFont(*FontRegistry::GetFont("coolvetica.otf"));
	m_Text.setFillColor(sf::Color::White);
	m_Text.setCharacterSize(20);

	SetPosition(sf::Vector2f(0, 0));
}

TextComponent::TextComponent(const std::string& text, sf::Font& font, BaseComponent& parentComponent, const sf::Color& color, unsigned int size)
	: m_Text(font, text, size)
{
	m_Text.setOrigin(m_Text.getLocalBounds().getCenter());
	SetPosition(parentComponent.GetPosition());
}

void TextComponent::Update(float dt)
{

}

void TextComponent::SetPosition(const sf::Vector2f& position)
{
	m_Text.setPosition(position);
}

void TextComponent::SetText(const std::string& text)
{
	m_Text.setString(text);
	m_Text.setOrigin(m_Text.getLocalBounds().getCenter());
}

void TextComponent::SetColor(const sf::Color& color)
{
	m_Text.setFillColor(color);
}

void TextComponent::SetCharacterSize(unsigned int size)
{
	m_Text.setCharacterSize(size);
	m_Text.setOrigin(m_Text.getLocalBounds().getCenter());
}

void TextComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Text, states);
}

const sf::Vector2f& TextComponent::GetBounds() const
{
	return m_Text.getLocalBounds().size;
}

sf::Vector2f TextComponent::GetPosition() const
{
	return m_Text.getPosition();
}

const sf::Text& TextComponent::GetText() const
{
	return m_Text;
}
