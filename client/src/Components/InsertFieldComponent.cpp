#include "InsertFieldComponent.h"
#include <Window/InputHandler.h>
#include <SFML/Window/Keyboard.hpp>
//#include "src/core/Managers/InputHandler.h"


InsertFieldComponent::InsertFieldComponent(sf::Font& font, InputHandler* inputHandler)
    : m_CharacterLimit(DEFAULT_CHARACTER_LIMIT)
    , m_Focus(false)
    , m_CursorTimer(0.0f)
    , m_Text(font)
    , m_Label(nullptr)
    , m_Cursor(font)
    , m_ErrorText(font)
    , m_inputHandler(inputHandler)
{
    m_Rectangle.setSize(FIELD_SIZE);
    m_Rectangle.setFillColor(sf::Color(171, 171, 171));
    m_Rectangle.setOutlineColor(sf::Color::White);
    m_Rectangle.setOutlineThickness(1.0f);

    m_Text.SetColor(sf::Color::Black);

    m_Label = new TextComponent(font);
    m_Label->SetText("InsertField");
    m_Label->SetColor(sf::Color(171, 171, 171));
    m_Label->SetCharacterSize(25);

    m_Cursor.SetText("|");
    m_Cursor.SetColor(sf::Color::Black);
    m_Cursor.SetVisible(false);

    m_ErrorText.SetText("");
    m_ErrorText.SetColor(sf::Color::Red);
    m_ErrorText.SetCharacterSize(25);

    SetPosition(sf::Vector2f(0.0f, 0.0f));
}

InsertFieldComponent::InsertFieldComponent(sf::Font& font, InputHandler* inputHandler, const sf::Vector2f& pos, const sf::Vector2f& size,
    const sf::Color& idleColor, const sf::Color& hoverColor,
    float outlineThickness, unsigned int characterLimit)
    : m_CharacterLimit(characterLimit)
    , m_Focus(false)
    , m_CursorTimer(0.0f)
    , m_Text(font)
    , m_Label(nullptr)
    , m_Cursor(font)
    , m_ErrorText(font)
    , m_inputHandler(inputHandler)
{
    m_Label = new TextComponent(font);
    m_Rectangle.setSize(size);
    m_Rectangle.setFillColor(idleColor);
    m_Rectangle.setOutlineColor(hoverColor);
    m_Rectangle.setOutlineThickness(outlineThickness);

    SetPosition(sf::Vector2f(0.0f, 0.0f));
}

void InsertFieldComponent::BlinkCursor(float dt)
{
    if (m_CursorTimer >= CURSOR_BLINK_TIME)
    {
        m_CursorTimer = 0.0f;
        m_Cursor.SetVisible(!m_Cursor.IsVisible());
    }
    else
    {
        m_CursorTimer += dt;
    }
}

void InsertFieldComponent::Update(float dt)
{
    if (IsMouseOver())
    {
        m_Rectangle.setOutlineThickness(2.0f);

        if (m_inputHandler->IsMouseButtonPressed(sf::Mouse::Button::Left))
        {
            m_Focus = true;
            m_Cursor.SetVisible(true);
        }
    }
    else if (m_inputHandler->IsKeyPressed(sf::Keyboard::Key::Enter) || m_inputHandler->IsMouseButtonPressed(sf::Mouse::Button::Left))
    {
        m_Focus = false;
        m_Rectangle.setOutlineThickness(0.0f);
        m_Cursor.SetVisible(false);
        m_CursorTimer = 0.0f;
    }


    if (!m_Focus) return;

    BlinkCursor(dt);

    if (Window::IsFocused())
    {
        if (GetTextSize() > 0 && m_inputHandler->IsKeyPressed(sf::Keyboard::Key::Backspace))
        {
            m_TextContent.pop_back();
            m_Text.SetText(m_TextContent);
            ReplaceCursor();
        }

        if (GetTextSize() < m_CharacterLimit)
        {
            // Handle all the letters of the alphabet
            for (sf::Keyboard::Key key = sf::Keyboard::Key::A; key <= sf::Keyboard::Key::Z; key = static_cast<sf::Keyboard::Key>(static_cast<int>(key) + 1))
            {
                if (m_inputHandler->IsKeyPressed(key))
                {
                    const char baseChar = m_inputHandler->IsKeyHeld(sf::Keyboard::Key::LShift) || m_inputHandler->IsKeyHeld(sf::Keyboard::Key::RShift) ? 'A' : 'a';
                    AppendCharacter(static_cast<const char>(baseChar + ((int)key - (int)sf::Keyboard::Key::A)));
                }
            }

            // Handle all the numbers
            for (sf::Keyboard::Key key = sf::Keyboard::Key::Num0; key <= sf::Keyboard::Key::Num9; key = static_cast<sf::Keyboard::Key>(static_cast<int>(key) + 1))
            {
                if (m_inputHandler->IsKeyPressed(key))
                {
                    AppendCharacter(static_cast<const char>('0' + ((int)key - (int)sf::Keyboard::Key::Num0)));
                }
            }

            // Handle all the numpad numbers
            for (sf::Keyboard::Key key = sf::Keyboard::Key::Numpad0; key <= sf::Keyboard::Key::Numpad9; key = static_cast<sf::Keyboard::Key>(static_cast<int>(key) + 1))
            {
                if (m_inputHandler->IsKeyPressed(key))
                {
                    AppendCharacter(static_cast<const char>('0' + ((int)key - (int)sf::Keyboard::Key::Numpad0)));
                }
            }

            // Handle the dot
            if (m_inputHandler->IsKeyPressed(sf::Keyboard::Key::Period))
            {
                AppendCharacter('.');
            }
            else if (m_inputHandler->IsKeyPressed(sf::Keyboard::Key::Delete))
            {
                SetText("");
            }
        }
    }
}

void InsertFieldComponent::SetText(const std::string& text)
{
    //substring text to fit the character limit
    if (text.size() > m_CharacterLimit)
    {
        m_TextContent = text.substr(0, m_CharacterLimit);
    }
    else
    {
        m_TextContent = text;
    }

    m_Text.SetText(m_TextContent);
    ReplaceCursor();
}

void InsertFieldComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Rectangle, states);
    target.draw(m_Text, states);
    target.draw(*m_Label, states);
    target.draw(m_ErrorText, states);

    if (m_Cursor.IsVisible())
        target.draw(m_Cursor, states);
}

bool InsertFieldComponent::IsMouseOver()
{
    const sf::Vector2f mousePos = (sf::Vector2f)m_inputHandler->GetMousePosition();
    const sf::Vector2f buttonPos = m_Rectangle.getPosition();
    const sf::Vector2f buttonSize = m_Rectangle.getSize();

    return mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
        mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y;
}

void InsertFieldComponent::AppendCharacter(const char& c)
{
    m_TextContent.push_back(c);
    m_Text.SetText(m_TextContent);
    ReplaceCursor();
}

void InsertFieldComponent::SetPosition(const sf::Vector2f& position)
{
    m_Rectangle.setPosition(position);

    m_Label->SetPosition(m_Rectangle.getPosition() - sf::Vector2f(0, m_Label->GetSize().y + m_Rectangle.getOutlineThickness() + 5));
    m_ErrorText.SetPosition(m_Rectangle.getPosition() + sf::Vector2f(0, m_Rectangle.getSize().y + m_Rectangle.getOutlineThickness()));

    const float xPos = position.x + m_Rectangle.getOutlineThickness();
    const float yPos = position.y + m_Rectangle.getOutlineThickness();

    m_Text.SetPosition(sf::Vector2f(xPos, yPos));
    ReplaceCursor();
}

void InsertFieldComponent::ReplaceCursor()
{
    m_Cursor.SetPosition(m_Text.GetPosition() + sf::Vector2f(m_Text.GetSize().x + 3, m_Text.GetSize().y * 0.3f));
}
