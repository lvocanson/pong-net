#pragma once
#include "Scene.h"
#include <cassert>
#include <memory>

class ClientApp;

class SceneMachine
{
public:

	template <typename SceneType, typename... Args>
	void SetFirstScene(Args&&... args)
	{
		assert(m_currentState == nullptr);
		m_currentState = std::make_unique<SceneType>(std::forward<Args>(args)...);
	}

	template <typename SceneType, typename... Args>
	void ChangeScene(Args&&... args)
	{
		m_currentState = std::make_unique<SceneType>(std::forward<Args>(args)...);
	}

	void Draw(sf::RenderWindow& wnd) const
	{
		m_currentState->Draw(wnd);
	}

	void Update(float dt)
	{
		m_currentState->OnUpdate(dt);
	}

	void OnEvent(const sf::Event& event)
	{
		m_currentState->OnEvent(event);
	}

protected:
	std::unique_ptr<Scene> m_currentState;
};
