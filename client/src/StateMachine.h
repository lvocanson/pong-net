#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <cassert>
#include <memory>

template <typename Derived>
class State
{
public:
	virtual ~State() = default;

	virtual void OnEnter(Derived& derived) = 0;
	virtual void Draw(sf::RenderWindow& wnd) const = 0;
	virtual void OnEvent(Derived& derived, const sf::Event& event) = 0;
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

	void Draw(sf::RenderWindow& wnd) const
	{
		m_currentState->Draw(wnd);
	}

	void OnEvent(const sf::Event& event)
	{
		m_currentState->OnEvent(static_cast<Derived&>(*this), event);
	}

protected:
	std::unique_ptr<State<Derived>> m_currentState;
};
