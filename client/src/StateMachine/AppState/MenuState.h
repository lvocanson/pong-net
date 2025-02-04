#pragma once
#include "../StateMachine.h"
#include "../../Components/ButtonComponent.h"
#include "../../Window/Window.h"

class MenuState : public State
{
public:
#pragma region  Constructor

    MenuState(StateMachine* stateMachine, Window* window);
    MenuState(const MenuState& other) = delete;
    MenuState& operator=(const MenuState& other) = delete;
    ~MenuState() override;

#pragma endregion

#pragma region  Override

    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnExit() override;

#pragma endregion

#pragma region  Class Methods

    void AddButton(const sf::Vector2f& pos, const sf::Color& color, const std::string& text, sf::Font* font, std::function<void()> function);
    ButtonComponent* FindButtonByText(const std::string& text);

    void ShowPlayButton(const sf::Vector2f& pos);
    void ShowConnectionButton(const sf::Vector2f& pos);
    void ShowDisconnectButton(const sf::Vector2f& pos);
    void ShowQuitButton(const sf::Vector2f& pos);

#pragma endregion

private:
#pragma region  Variables

    std::vector< ButtonComponent*> _btns;
    Window* m_Window = nullptr;

#pragma endregion

};