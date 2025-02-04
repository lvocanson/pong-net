#pragma once 

#include "State.h"
#include <map>
#include <iostream>

class StateMachine
{
public:
#pragma region Constructor

    StateMachine() = default;
    ~StateMachine();

#pragma endregion

#pragma region StateMachine Methods

    void Start() const;
    void Update(float dt);
    void Cleanup();

#pragma endregion

#pragma region State Methods

    void InitState(const std::string& initState);
    /// <summary>
    /// Add the state ref to the dictionnary
    /// </summary>
    void AddState(const std::string& stateName, State* newState);
    const State* GetState(const std::string& stateName);
    /// <summary>
    /// Switch the current state to the state with the name newState
    /// </summary>
    void SwitchState(const std::string& newState);

    inline State* GetCurrentState() const { return m_CurrentState; }

#pragma endregion

private:
#pragma region Variables

    State* m_CurrentState = nullptr;
    State* m_NextState = nullptr;

    std::map <std::string, State*> m_States;

#pragma endregion

};
