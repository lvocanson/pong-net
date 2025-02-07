#pragma once
#include "ClientApp.h"
#include "StateMachine.h"
#include <swap_back_array.h>

class IDrawable;
class IEventConsumer;

class Scene : public State<ClientApp>
{
protected:

	template <typename T>
	void Enable(T& obj)
	{
		if constexpr (std::is_base_of_v<IDrawable, T>)
		{
			m_Drawables.push_back(&obj);
		}
		if constexpr (std::is_base_of_v<IEventConsumer, T>)
		{
			m_EventConsumers.push_back(&obj);
		}
	}

	template <typename T>
	void Disable(T& obj)
	{
		if constexpr (std::is_base_of_v<IDrawable, T>)
		{
			auto it = std::find(m_Drawables.begin(), m_Drawables.end(), &obj);
			m_Drawables.erase_swap(it);
		}
		if constexpr (std::is_base_of_v<IEventConsumer, T>)
		{
			auto it = std::find(m_EventConsumers.begin(), m_EventConsumers.end(), &obj);
			m_EventConsumers.erase_swap(it);
		}
	}

protected:

	// Inherited via State
	virtual void OnEnter(ClientApp&) override {};
	virtual void Draw(sf::RenderWindow& wnd) const override;
	virtual void OnEvent(ClientApp&, const sf::Event& event) override;
	virtual void OnExit(ClientApp&) override {};

private:

	stc::swap_back_array<IDrawable*> m_Drawables;
	stc::swap_back_array<IEventConsumer*> m_EventConsumers;
};