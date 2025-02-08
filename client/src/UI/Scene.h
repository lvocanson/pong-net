#pragma once
#include "Utils/Misc.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

class ClientApp;
class IDrawable;
class IUpdatable;
class IEventConsumer;

class Scene
{
protected:

	template <typename T>
	void Enable(T& obj)
	{
		if constexpr (std::is_base_of_v<IDrawable, T>)
		{
			m_Drawables.push_back(&obj);
		}
		if constexpr (std::is_base_of_v<IUpdatable, T>)
		{
			m_Updatables.push_back(&obj);
		}
		if constexpr (std::is_base_of_v<IEventConsumer, T>)
		{
			m_EventConsumers.push_back(&obj);
		}
	}

	template <typename... Ts>
	void Enable(Ts&... objs)
	{
		(Enable(objs), ...);
	}

	template <typename T>
	void Disable(T& obj)
	{
		if constexpr (std::is_base_of_v<IDrawable, T>)
		{
			auto it = std::find(m_Drawables.begin(), m_Drawables.end(), &obj);
			m_Drawables.erase(it);
		}
		if constexpr (std::is_base_of_v<IDrawable, T>)
		{
			auto it = std::find(m_Updatables.begin(), m_Updatables.end(), &obj);
			m_Updatables.erase(it);
		}
		if constexpr (std::is_base_of_v<IEventConsumer, T>)
		{
			auto it = std::find(m_EventConsumers.begin(), m_EventConsumers.end(), &obj);
			m_EventConsumers.erase(it);
		}
	}

	template <typename... Ts>
	void Disable(Ts&... objs)
	{
		(Disable(objs), ...);
	}

public:

	virtual void Draw(sf::RenderWindow& wnd) const;
	virtual void OnUpdate(float dt);
	virtual void OnEvent(const sf::Event& event);

private:

	std::vector<IDrawable*> m_Drawables;
	std::vector<IUpdatable*> m_Updatables;
	std::vector<IEventConsumer*> m_EventConsumers;
};