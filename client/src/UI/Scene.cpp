#include "Scene.h"
#include "IDrawable.h"
#include "IEventConsumer.h"

void Scene::Draw(sf::RenderWindow& wnd) const
{
	for (auto* drawable : m_Drawables)
	{
		drawable->Draw(wnd);
	}
}

void Scene::OnEvent(ClientApp& app, const sf::Event& event)
{
	// Copy in case an event modifies the original
	auto consumersCpy = m_EventConsumers;
	for (auto* consumer : consumersCpy)
	{
		consumer->OnEvent(event);
	}
}
