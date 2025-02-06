#pragma once
#include "BaseComponent.h"
#include <SFML/Audio.hpp>

class InputHandler;

class SliderComponent : public BaseComponent
{
private:

	sf::RectangleShape m_Bar;
	sf::CircleShape m_Knob;
	float m_Value;
	bool m_IsDragging = false;
	const InputHandler& m_Input;
	sf::Music& m_Music;
	std::string m_Filename;

public:

	SliderComponent(const InputHandler& inputHandler, sf::Music& music);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Update(float dt, Window& window) override;

	void SetPosition(const sf::Vector2f& position) override;
	sf::Vector2f GetPosition() const override;
	void SetWidth(float width);
	sf::Vector2f GetSize() const override;

	void SaveVolumeValue() const;
	float LoadVolumeValue() const;
	bool IsMouseOver(Window& window);
	float getValue() const { return m_Value; }
};
