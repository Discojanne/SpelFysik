#pragma once

#include <sstream>
#include "Cannonball.h"
#include <iostream>

class Game : public sf::Drawable
{
public:
	Game();
	~Game();

	void Update(float dt);
	bool initialize();

private:
	sf::Texture m_BackgroundTexture;
	sf::Sprite m_BackgroundSprite;
	Cannonball m_Cannonball;

	sf::Text text;
	std::ostringstream stream;
	sf::Font font;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};