#pragma once
#include "BaseComponent.h"
#include <SFML/Audio.hpp>

class InputHandler;

class SliderComponent : public BaseComponent 
{
private:
    sf::RectangleShape bar;
    sf::CircleShape knob;
    float minValue, maxValue, value;
    bool isDragging = false;
    InputHandler* m_inputHandler;
    sf::Music* music;
    std::string filename;


public:
    SliderComponent(sf::Vector2f& pos, float width, float minValue, float maxValue, InputHandler* inputHandler, sf::Music* music);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void Update(float dt) override;

    void SetPosition(const sf::Vector2f& position) override;
    sf::Vector2f GetPosition() const override;
    sf::Vector2f GetSize() const override;

    void SaveVolumeValue();
    float LoadVolumeValue();
    bool IsMouseOver();
    float getValue() const { return value; }
};