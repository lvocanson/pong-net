#pragma once

#include "ClientApp.h"
#include "../StateMachine.h"
#include "../../Window/Window.h"
#include "../../Components/ButtonComponent.h"
#include "../../Components/InsertFieldComponent.h"

class ConnectionState : public State<ClientApp>
{
public:
#pragma region  Constructor

      ConnectionState(ClientApp& app);
      ConnectionState(const ConnectionState& other) = delete;
      ConnectionState& operator=(const ConnectionState& other) = delete;
      ~ConnectionState();

#pragma endregion

#pragma region  Override

      void OnEnter(ClientApp& derived) override;
      void OnUpdate(ClientApp& derived, float deltaTime) override;
      void OnExit(ClientApp& derived) override;

#pragma endregion

#pragma region  Class Methods

    void InitIpField(const sf::Vector2f& pos);
    void InitUsernameField(const sf::Vector2f& pos);
    void InitBackButton(const sf::Vector2f& pos);
    void InitConnectButton(const sf::Vector2f& pos);

    void ActiveButtonFunction(ClientApp& app);

#pragma endregion

private:

#pragma region  Variables

    bool m_IsTryingToConnect = false;

    ClientApp* m_clientApp;
    sf::Font* m_font;

    union
    {
        struct
        {
            ButtonComponent BackBtn;
            ButtonComponent ConnectBtn;
        };

        std::array<ButtonComponent, 2> _btns;
    };

    union
    {
        struct
        {
            InsertFieldComponent IpField;
            InsertFieldComponent UsernameField;
        };

        std::array<InsertFieldComponent, 2> _fields;
    };

    ButtonFunction _currentFunction;

    std::string m_Name = "";

#pragma endregion
};
