#pragma once

#include <SFML\Graphics.hpp>
#include <math.h>

#define WIDTH_OF_TEXTURE 16
#define g 9.82
#define PI 3.14159265
#define rho 1.21				// densitet luften
#define mu 0.0000183			// viskositet luft
#define eKoef -0.3				// krock koefficient

// https://www.arc.id.au/Cannonballs.html
#define ballWidth 0.1f			// Cannon ball width (m)
#define ballMass 4.1f

#define gunpowderAmount 300		// in grams
#define startAngel      15		// degrees

class Projectile : public sf::Drawable
{
public:
	Projectile();
	~Projectile();

	void Update(float dt);
	sf::Vector2f getPos();
	sf::Vector2f getVelocity();
	float getVelocityFloat();
	void setVelocity(sf::Vector2f newV);
	void setStartAngle(sf::Vector2f newV);
	float calculateStartVelocity(int howMuchExplosives);
	float calculateDragforce();
	void generateWind();

	sf::Vector2f getUnitVector(sf::Vector2f v);
	float getLengthOfVector(sf::Vector2f v);

private:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;	// contains position of Projectile

	bool m_isAirbourne;
	sf::Vector2f m_velocity;
	sf::Vector2f m_startSpeed;
	float m_width;
	float m_mass;

	sf::Vector2f wind;

	void updateVelocity(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};