#pragma once
#include "../StateMachine.h"
#include "../../Window/Window.h"
#include "../../Components/ButtonComponent.h"
#include "../../Components/InsertFieldComponent.h"

class ConnectionState : public State
{
public:
#pragma region  Constructor

    ConnectionState(StateMachine* stateMachine, Window* window);
      ConnectionState(const ConnectionState& other) = delete;
      ConnectionState& operator=(const ConnectionState& other) = delete;
      ~ConnectionState();

#pragma endregion

#pragma region  Override

    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnExit() override;

#pragma endregion

#pragma region  Class Methods

    void AddButton(const sf::Vector2f& pos, const sf::Color& color, const std::string& text, sf::Font* font, std::function<void()> function);
    void AddField(const sf::Vector2f& pos, const std::string& label, sf::Font* font);
    InsertFieldComponent* FindFieldByText(const std::string& text);

    void ShowIpField(const sf::Vector2f& pos);
    void ShowNameField(const sf::Vector2f& pos);
    void ShowBackButton(const sf::Vector2f& pos);
    void ShowConnectButton(const sf::Vector2f& pos);

    bool IsValidIpAddress(const char* ip);

#pragma endregion

private:

#pragma region  Variables

    bool m_IsTryingToConnect = false;

    Window* m_Window = nullptr;

    std::string m_Name = "";
    sf::Font* font;

    std::vector< ButtonComponent*> _btns;
    std::vector< InsertFieldComponent*> _fields;

#pragma endregion
};
