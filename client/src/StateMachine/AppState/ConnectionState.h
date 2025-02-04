#pragma once
#include "../StateMachine.h"
#include "../../Window/Window.h"
#include "../../Components/ButtonComponent.h"
#include "../../Components/InsertFieldComponent.h"

class ConnectionState : public State
{
public:
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnExit() override;


    ConnectionState(StateMachine* stateMachine, Window* window);
    ConnectionState(const ConnectionState& other) = delete;
    ConnectionState& operator=(const ConnectionState& other) = delete;
    ~ConnectionState();

    void ShowIpField();
    void ShowNameField();
    void ShowBackButton();
    void ShowConnectButton();

    bool IsValidIpAddress(const char* ip);

private:

    bool m_IsTryingToConnect = false;

    Window* m_Window = nullptr;

    std::string m_Name = "";

    ButtonComponent* m_BackButton = nullptr;
    ButtonComponent* m_ConnectButton = nullptr;

    InsertFieldComponent* m_IpField = nullptr;
    InsertFieldComponent* m_NameField = nullptr;
};
