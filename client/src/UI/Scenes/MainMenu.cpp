#include "MainMenu.h"

MainMenu::MainMenu(const sf::Font& font)
	: m_Text(font)
	, m_Button()
{
	m_Text.SetString("Hello!");

	m_Button.SetRect({{25.f, 25.f}, {25.f, 25.f}});
	m_Button.SetCallback(std::bind(&MainMenu::OnButtonClicked, this));

	Enable(m_Text);
	Enable(m_Button);
}

void MainMenu::OnButtonClicked()
{
	Disable(m_Button);
}
