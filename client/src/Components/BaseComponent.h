#pragma once
#include <SFML/Graphics.hpp>

class BaseComponent : public sf::Drawable
{
public:

    ~BaseComponent() override = default;

    virtual void SetPosition(const sf::Vector2f& position) = 0;
    virtual sf::Vector2f GetPosition() const = 0;
    virtual sf::Vector2f GetSize() const = 0;
    void SetVisible(bool visible) { m_IsVisible = visible; }
    bool IsVisible() const { return m_IsVisible; }

    virtual void Update(float) = 0;

protected:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    bool m_IsVisible = true;
};
