#include "Cannonball.h"

Cannonball::Cannonball() {

	m_Texture.loadFromFile("../Resources/player.png");
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setTextureRect(sf::IntRect(0, 0, WIDTH_OF_TEXTURE, WIDTH_OF_TEXTURE));
	m_Sprite.setOrigin(WIDTH_OF_TEXTURE / 2.0f, WIDTH_OF_TEXTURE / 2.0f);
	m_Sprite.setPosition(WIDTH_OF_TEXTURE / 2.0f, 700.0f - WIDTH_OF_TEXTURE / 2.0f);

}

Cannonball::~Cannonball() {
}

void Cannonball::Update(float dt) {

	if (m_isAirbourne) {
		updateVelocity(dt);
	}
		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (!m_isAirbourne)
		m_isAirbourne = true;
	}
}

void Cannonball::Init() {

	// Calculate starting velocity for the cannonball from the cannon explotion.
	float v = CalculateStartVelocity();
	m_velocity = sf::Vector2f(cosf(startAngel * PI / 180) * v, sinf(startAngel * PI / 180) * v);


}

float Cannonball::CalculateStartVelocity() {
	// explotion //
	int massOfExplosiveMaterial;
	float e;								// ekvivalensfaktor
	float M_eq;								// massa ekvivalent i TNT
	float p;								// övertryck

	// the launch //
	float pipelength;						// längden av kanonröret
	float WidthOfBall;						// diametern på kulan
	float areaOfPipe;						// arean på öppningen av 
	float lengthOfExplosives = 0.1f;		// längden på krutet innan bollen

	// misc //
	float E_explo, E_final;			// energy from launch
	float finalVilocity;					// starthastigheten för kanonkulan
	float explotionConstant;				// constant value used to calculate energy released

	// set base values //
	massOfExplosiveMaterial = gunpowderAmount;
	e = 0.55f;								// 0.55 för krut. Varierar beroende av material
	M_eq = (float)massOfExplosiveMaterial * e * 0.001;	// transforming grams into kg
	pipelength = 1.5f;
	WidthOfBall = ballWidth;
	areaOfPipe = ((WidthOfBall + 0.01f) / 2.0f) * ((WidthOfBall + 0.01f) / 2.0f) * PI;

	// Energy released calculation //
	explotionConstant = 4184000.0f;			// Of 1kg TNT this much energy is released.
	E_explo = M_eq * explotionConstant;
	p = E_explo / (areaOfPipe * lengthOfExplosives);

	// calculate energy and velocity //
	E_final = p * areaOfPipe * lengthOfExplosives * log((lengthOfExplosives + pipelength) / (2.0f * lengthOfExplosives));
	finalVilocity = pow((2.0f * E_final) / ballMass, 0.5f);

	/// NEW CALCULATION ///
	float L0 = lengthOfExplosives;
	float L100 = L0 * (p / 100000.0f);

	float eFinal = E_explo * (log(pipelength / L0) / log(L100 / L0));
	finalVilocity = pow((2.0f * eFinal) / ballMass, 0.5f);

	return finalVilocity;
}

void Cannonball::updateVelocity(float dt) {
	// Readability
	float t = dt;
	sf::Vector2f v = m_velocity;


	float forceDrag = calculateDragforce();
	sf::Vector2f a = getUnitVector(m_velocity) * -forceDrag;
	a = a / ballMass;
	a.y -= g;

	// Change in position
	float x = v.x * t + a.x * t * t * 0.5f;
	float y = v.y * t + a.y * t * t * 0.5f;

	m_velocity += a * t;

	// move 
	float pixelInMeters = pixelSize;
	m_Sprite.move(x * pixelInMeters, -y * pixelInMeters);

	/*
	COLLISION
	*/
		// Ground
	if (m_Sprite.getPosition().y > 700.0f - WIDTH_OF_TEXTURE / 2.0f) {

		m_Sprite.setPosition(m_Sprite.getPosition().x, 700.0f - 8.01f);
		m_velocity.y = m_velocity.y * -1 * krockKoefGround;
		m_velocity.x = m_velocity.x * 0.9f;
	}

		// Castle
	if (m_Sprite.getPosition().x > 1400 - 8 && m_Sprite.getPosition().x < 1410 && m_Sprite.getPosition().y < 708.0f && m_Sprite.getPosition().y > 580.0f) {
		m_Sprite.setPosition(1400 - 8, m_Sprite.getPosition().y);
		m_velocity.x = (m_velocity.x * -1 * krockKoefWall);
	}

	if (m_Sprite.getPosition().x > 1400 && m_Sprite.getPosition().x < 1552.0f && m_Sprite.getPosition().y > 580.0f) {
		m_velocity.x = 0.0f;
		m_velocity.y = 0.0001f;
	}

	/*
	Stop
	*/
	if (abs(m_velocity.y) < 0.001f && abs(m_velocity.x) < 0.001f && m_Sprite.getPosition().y > 690.0f) {
		m_velocity.x = 0.0f;
		m_velocity.y = 0.0f;
		m_isAirbourne = false;
	}

	if ((getPos().y - (ballWidth / 2)) == 700) // On the ground, take pos - r else we would be in the origo on the ball.
	{
		calculateFriction();
	}
}

void Cannonball::calculateFriction()
{
	m_velocity.x *= grassIronFriction; 
}

float Cannonball::calculateDragforce() {
	sf::Vector2f u = m_velocity; // wind
	float re = (rho * ballWidth * getLengthOfVector(m_velocity)) / mu;
	float area = PI * pow(ballWidth / 2, 2);
	float Cd = 0;
	float FD = 0;
	
	if (re < 100)
		Cd = 1.20f;
	else if (re < 1000)
		Cd = 0.58f;
	else if (re < 100000)
		Cd = 0.5f;
	else if (re < 1000000)
		Cd = 0.19;
	else if (re < 10000000)
		Cd = 0.39;

	// Calculate the air resistance force
	float lengthOfU = getLengthOfVector(u);
	FD = 0.5 * Cd * area * rho * pow(lengthOfU, 2);

	return FD;
}

float Cannonball::getLengthOfVector(sf::Vector2f v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f Cannonball::getUnitVector(sf::Vector2f v) {
	float length = getLengthOfVector(v);
	sf::Vector2f u;
	u.x = v.x / length;
	u.y = v.y / length;
	return u;
}

sf::Vector2f Cannonball::getPos() {
	return m_Sprite.getPosition();
}

sf::Vector2f Cannonball::getVelocity() {
	return m_velocity;
}

void Cannonball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_Sprite, states);
}
