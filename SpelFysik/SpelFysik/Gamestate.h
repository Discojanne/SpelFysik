#pragma once

#include <SFML/Graphics.hpp>
class Gamestate : public sf::Drawable
{
public:
	Gamestate();
	~Gamestate();

	bool initialize();
	void Update(float dt);

private:
	sf::Texture m_BackgroundTexture;
	sf::Sprite m_BackgroundSprite;
	//sf::Projectile m_Projectile;

	sf::Text m_text;
	//sf::ostringstream stream;
	sf::Font m_font;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};