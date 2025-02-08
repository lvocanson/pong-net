#include "Scene.h"
#include "IDrawable.h"
#include "IUpdatable.h"
#include "IEventConsumer.h"

void Scene::Draw(sf::RenderWindow& wnd) const
{
	for (auto* drawable : m_Drawables)
	{
		drawable->Draw(wnd);
	}
}

void Scene::OnUpdate(float dt)
{
	// Copy in case an update modifies the original
	std::vector updatablesCpy = m_Updatables;
	for (auto* updatable : updatablesCpy)
	{
		updatable->OnUpdate(dt);
	}
}

void Scene::OnEvent(const sf::Event& event)
{
	// Copy in case an event modifies the original
	std::vector consumersCpy = m_EventConsumers;
	for (auto* consumer : consumersCpy)
	{
		consumer->OnEvent(event);
	}
}
