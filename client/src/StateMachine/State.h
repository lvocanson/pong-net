#pragma once 
#include "../CoreDefinitions.h"

class StateMachine;

class State
{
public:
#pragma region  Constructor

    State(StateMachine* stateMachine) : m_StateMachine(stateMachine) {};
    virtual ~State() { m_StateMachine = nullptr; };

#pragma endregion

#pragma region  State

    virtual void OnEnter() = 0;
    virtual void OnUpdate(float dt) = 0;
    virtual void OnExit() = 0;
    //virtual void OnReceiveData(const Json& serializeData) {};

#pragma endregion

protected:
#pragma region  Variables

    StateMachine* m_StateMachine;

#pragma endregion

};
