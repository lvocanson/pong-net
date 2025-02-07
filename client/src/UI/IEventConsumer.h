#pragma once
#include <SFML/Window/Event.hpp>

class IEventConsumer
{
public:

	virtual void OnEvent(const sf::Event& event) = 0;
};