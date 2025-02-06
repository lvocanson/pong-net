#pragma once

#include <ClientApp.h>
#include "Game/Pong.h"

class GameState : public State<ClientApp>
{
public:
#pragma region  Constructor

    GameState();
    GameState(const GameState& other) = delete;
    GameState& operator=(const GameState& other) = delete;
    ~GameState();

#pragma endregion

#pragma region  Override

    void OnEnter(ClientApp& app) override;
    void OnUpdate(ClientApp& app, float deltaTime) override;
    void OnExit(ClientApp& app) override;

#pragma endregion

#pragma region  Class Methods

    void PollEvents(ClientApp& app);

#pragma endregion

private:

#pragma region  Variables

    Pong m_PongGame;
    PongDisplay* m_PongDisplay;
    unsigned m_LeftScore, m_RightScore;

#pragma endregion
};

