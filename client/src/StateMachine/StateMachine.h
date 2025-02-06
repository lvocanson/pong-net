#pragma once

#include <cassert>
#include <memory>

template <typename Derived>
class State
{
public:
	virtual ~State() = default;

	virtual void OnEnter(Derived& derived) = 0;
	virtual void OnUpdate(Derived& derived, float deltaTime) = 0;
	virtual void OnExit(Derived& derived) = 0;
};

template <typename Derived>
class StateMachine
{
public:

	template <typename StateType, typename... Args>
	void SetFirstState(Args&&... args)
	{
		assert(m_currentState == nullptr);
		m_currentState = std::make_unique<StateType>(std::forward<Args>(args)...);
		m_currentState->OnEnter(static_cast<Derived&>(*this));
	}

	template <typename StateType, typename... Args>
	void ChangeState(Args&&... args)
	{
		m_currentState->OnExit(static_cast<Derived&>(*this));
		m_currentState = std::make_unique<StateType>(std::forward<Args>(args)...);
		m_currentState->OnEnter(static_cast<Derived&>(*this));
	}

	void Update(float deltaTime)
	{
		m_currentState->OnUpdate(static_cast<Derived&>(*this), deltaTime);
	}

protected:
	std::unique_ptr<State<Derived>> m_currentState;
};
