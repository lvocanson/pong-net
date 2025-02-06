#pragma once

#include "ClientApp.h"
#include "../StateMachine.h"
#include "../../Components/ButtonComponent.h"
#include "../../Components/SliderComponent.h"
#include "../../Window/Window.h"
#include <SFML/Graphics.hpp>

class MenuState : public State<ClientApp>
{
public:
#pragma region  Constructor

    MenuState(ClientApp& app);
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

    void InitSlider(sf::Vector2f& pos, float width, float minValue, float maxValue);
    void InitPlayButton(const sf::Vector2f& pos);
    void InitConnectionButton(const sf::Vector2f& pos);
    void InitDisconnectButton(const sf::Vector2f& pos);
    void InitQuitButton(const sf::Vector2f& pos);

    void ActiveButtonFunction(ClientApp& app);

#pragma endregion

private:
#pragma region  Variables

    ClientApp* m_clientApp;

    union
    {
        struct
        {
            ButtonComponent PlayBtn;
            ButtonComponent ConnectBtn;
            ButtonComponent DisconnectBtn;
            ButtonComponent QuitBtn;
        };

        std::array<ButtonComponent, 4> _btns;
    }; 

    SliderComponent _slider;
    TextComponent _sliderText;

    ButtonFunction _currentFunction;
    
#pragma endregion

};