#pragma once

#include <SFML\Graphics.hpp>

#define WIDTH_OF_TEXTURE 16
#define g 9.82
#define PI 3.14159265
#define rho 1.21				// densitet luften
#define mu 0.0000183			// viskositet luft

#define krockKoefWall 0.06f // Could not find specifically for grass nor stone against iron. but found some approximations. https://en.wikipedia.org/wiki/Coefficient_of_restitution#Speeds_after_impact, under 'Predicting from material properties'
#define krockKoefGround 0.3f

#define grassIronFriction 0.26f  // Could not find for iron vs grass, choosed iron vs straw fiber.

#define pixelSize 4.0f			// meters

// https://www.arc.id.au/Cannonballs.html
#define ballWidth 0.1f			// Cannon ball width (m)
#define ballMass 4.1f
#define gunpowderAmount 300		// in grams 300		9
#define startAngel      4		// degrees 4		30

class Cannonball : public sf::Drawable {
public:
	Cannonball();
	~Cannonball();

	void Update(float dt);
	void Init();

	sf::Vector2f getPos();
	sf::Vector2f getVelocity();
	float calculateDragforce();

private:

	float CalculateStartVelocity();
	void calculateFriction();
	void updateVelocity(float dt);
	
	float getLengthOfVector(sf::Vector2f v);
	sf::Vector2f getUnitVector(sf::Vector2f v);

	

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture m_Texture;
	sf::Sprite m_Sprite;	// contains position of Projectile

	sf::Vector2f m_velocity;
	bool m_isAirbourne = false;

};