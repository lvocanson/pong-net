#pragma once

#include "ClientApp.h"
#include "../StateMachine.h"
#include "../../Components/ButtonComponent.h"
#include "../../Window/Window.h"

class MenuState : public State<ClientApp>
{
public:
#pragma region  Constructor

    MenuState();
    MenuState(const MenuState& other) = delete;
    MenuState& operator=(const MenuState& other) = delete;
    ~MenuState() override;

#pragma endregion

#pragma region  Override

    virtual void OnEnter(ClientApp& app) override;
    virtual void OnUpdate(ClientApp& app, float deltaTime) override;
    virtual void OnExit(ClientApp& app) override;

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
    ClientApp* m_clientApp;

#pragma endregion

};