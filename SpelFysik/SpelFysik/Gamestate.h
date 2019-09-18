#pragma once

#include <sstream>
#include "Projectile.h"
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
	Projectile m_Projectile;

	sf::Text text;
	std::ostringstream stream;
	sf::Font font;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};