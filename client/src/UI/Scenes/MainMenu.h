#pragma once
#include "UI/Scene.h"
#include "UI/Stuff/Button.h"
#include "UI/Stuff/Text.h"

class MainMenu : public Scene
{
public:

	MainMenu(const sf::Font& font);

	void OnButtonClicked();

private:

	Text m_Text;
	Button m_Button;
};