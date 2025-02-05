#include "SliderComponent.h"
#include <Window/InputHandler.h>
#include <iostream>

SliderComponent::SliderComponent(float x, float y, float width, float minValue, float maxValue, InputHandler* inputHandler, sf::Music* mainMusic)
    : minValue(minValue), maxValue(maxValue), value(minValue), m_inputHandler(inputHandler)
{
    music = mainMusic;
    // Création de la barre du slider
    bar.setSize(sf::Vector2f(width, 5));
    bar.setPosition(sf::Vector2(x, y));
    bar.setFillColor(sf::Color::White);

    // Création du curseur (knob)
    knob.setRadius(10);
    knob.setFillColor(sf::Color::Red);
    knob.setOrigin(sf::Vector2(knob.getRadius(), knob.getRadius()));
    knob.setPosition(sf::Vector2(x, y + bar.getSize().y / 2));
    value = 50;
    music->setVolume(value);
    knob.setPosition(sf::Vector2f(bar.getPosition().x + bar.getSize().x/2, knob.getPosition().y));
}

void SliderComponent::Update(float dt)
{
    sf::Vector2i mousePos = m_inputHandler->GetMousePosition();

    float sliderLeft = bar.getPosition().x;
    float sliderRight = bar.getPosition().x + bar.getSize().x;

    if (IsMouseOver())
    {
        if (m_inputHandler->IsMouseButtonPressed(sf::Mouse::Button::Left))
        {
            isDragging = true;
        }
    }
    if (m_inputHandler->IsMouseButtonReleased(sf::Mouse::Button::Left))
    {
        isDragging = false;
    }
    if (isDragging) {
        float newX = std::max(sliderLeft, std::min(static_cast<float>(mousePos.x), sliderRight));
        knob.setPosition(sf::Vector2f(newX, knob.getPosition().y));

        // Mise à jour de la valeur du slider
        value = minValue + ((newX - sliderLeft) / bar.getSize().x) * (maxValue - minValue);
        music->setVolume(value);
    }
}

void SliderComponent::SetPosition(const sf::Vector2f& position)
{
    bar.setPosition(position);
    knob.setPosition(position);
}

sf::Vector2f SliderComponent::GetPosition() const
{
    return bar.getPosition();
}

sf::Vector2f SliderComponent::GetSize() const
{
    return bar.getSize();
}

bool SliderComponent::IsMouseOver()
{
    sf::Vector2f mousePos = (sf::Vector2f)m_inputHandler->GetMousePosition();
    sf::Vector2f knobPos = knob.getPosition();
    float knobSize = knob.getRadius();

    float Right = knobPos.x + knobSize;
    float Left = knobPos.x - knobSize;
    float Bottom = knobPos.y + knobSize;
    float Top = knobPos.y - knobSize;

    return mousePos.x >= Left && mousePos.x <= Right && mousePos.y >= Top && mousePos.y <= Bottom;
}

void SliderComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(bar);
    target.draw(knob);
}