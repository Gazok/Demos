#include <SFML/Window/Mouse.hpp> 
#include "Mouse.hpp"

sf::Vector2i Mouse::pos_ = {0,0};
sf::Vector2i Mouse::lastPos_ = {0,0};

sf::Vector2i Mouse::delta()
{
    return pos_ - lastPos_;
}

sf::Vector2i const& Mouse::getPosition()
{
    sf::Vector2i pos = sf::Mouse::getPosition();

    lastPos_ = pos_;
    pos_     = pos;

    return pos_;
}

sf::Vector2i Mouse::getPosition(sf::Window const& rel)
{
    return getPosition() + rel.getPosition();
}

sf::Vector2i const& Mouse::getLastPosition()
{
    getPosition();
    return lastPos_;
}

sf::Vector2i Mouse::getLastPosition(sf::Window const& rel)
{
    getPosition();
    sf::Vector2i winPos = rel.getPosition();

    return lastPos_ + winPos;
}

void Mouse::setPosition(sf::Vector2i const& pos)
{
    lastPos_ = pos_; 
    pos_ = pos;

    sf::Mouse::setPosition(pos);
}

void Mouse::setPosition(sf::Vector2i const& pos, sf::Window const& rel)
{
    sf::Vector2i winPos = rel.getPosition();

    setPosition(pos + winPos);
}

void Mouse::setLastPosition(sf::Vector2i const& pos)
{
    lastPos_ = pos;
}

void Mouse::setLastPosition(sf::Vector2i const& pos, sf::Window const& rel)
{
    sf::Vector2i winPos = rel.getPosition();
    setLastPosition(pos + winPos);
}

void Mouse::update()
{
    getPosition();
}
