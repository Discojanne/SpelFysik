#include "Projectile.h"

Projectile::Projectile()
{
	sf::String fileName = "../Resources/player.png";

	m_Texture.loadFromFile(fileName);
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setTextureRect(sf::IntRect(0, 0, WIDTH_OF_TEXTURE, WIDTH_OF_TEXTURE));
	m_Sprite.setOrigin(WIDTH_OF_TEXTURE / 2.0f, WIDTH_OF_TEXTURE / 2.0f);	// 16 cus texture is 16*2 in length
	m_Sprite.setPosition(WIDTH_OF_TEXTURE / 2.0f, 700.0f - WIDTH_OF_TEXTURE / 2.0f);

	m_velocity = sf::Vector2f(0, 0);
	m_startSpeed = sf::Vector2f(0, 0);
	m_isAirbourne = false;
	m_width = ballWidth;
	m_mass = ballMass;
}

Projectile::~Projectile()
{
}

void Projectile::Update(float dt)
{
	if (m_isAirbourne)
		updateVelocity(dt);

	/// Handle input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (!m_isAirbourne)
			setVelocity(m_startSpeed);

		m_isAirbourne = true;
	}


}

sf::Vector2f Projectile::getPos()
{
	return m_Sprite.getPosition();
}

sf::Vector2f Projectile::getVelocity()
{
	return m_velocity;
}

float Projectile::getVelocityFloat()
{
	return pow(pow(m_velocity.x, 2) + pow(m_velocity.y, 2), 0.5f);
}

void Projectile::setVelocity(sf::Vector2f newV)
{
	m_velocity = newV;
}

void Projectile::setStartAngle(sf::Vector2f newV)
{
	m_startSpeed = newV;
}

float Projectile::calculateStartVelocity(int howMuchExplosives)
{
	// explotion //
	int massOfExplosiveMaterial;
	float e;								// ekvivalensfaktor
	float M_eq;								// massa ekvivalent i TNT
	float z, alpha, beta, gamma, horse;	// explotionsberäkningsvariabler
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
	massOfExplosiveMaterial = howMuchExplosives;
	e = 0.55f;								// 0.55 för krut. Varierar beroende av material
	M_eq = (float)massOfExplosiveMaterial * e * 0.001;	// transforming grams into kg
	pipelength = 1.5f;
	WidthOfBall = m_width;
	areaOfPipe = ((WidthOfBall + 0.01f) / 2.0f) * ((WidthOfBall + 0.01f) / 2.0f) * PI;

	// calculate the explotion //
	//z = 0.1f / pow(M_eq, (1.0f / 3.0f));
	//alpha = 1.0f + pow(z/4.5f,2.0f);
	//beta = 1.0f + pow(z / 0.048f, 2.0f);
	//gamma = 1.0f + pow(z / 0.32f, 2.0f);
	//horse = 1.0f + pow(z / 1.35f, 2.0f);
	//p = (8.08f * pow(10.0f, 7.0f) * alpha) / pow(beta * gamma * horse, 0.5f);

	// Energy released calculation //
	explotionConstant = 4184000.0f;			// Of 1kg TNT this much energy is released.
	E_explo = M_eq * explotionConstant;
	p = E_explo / (areaOfPipe * lengthOfExplosives);

	// calculate energy and velocity //
	E_final = p * areaOfPipe * lengthOfExplosives * log((lengthOfExplosives + pipelength) / (2.0f * lengthOfExplosives));
	finalVilocity = pow((2.0f * E_final) / m_mass, 0.5f);

	/// NEW CALCULATION ///
	float L0 = lengthOfExplosives;
	float L100 = L0 * (p / 100000.0f);

	float eFinal = E_explo * (log(pipelength / L0) / log(L100 / L0));
	finalVilocity = pow((2.0f * eFinal) / m_mass, 0.5f);

	return finalVilocity;
}

float Projectile::calculateDragforce()
{
	sf::Vector2f u = m_velocity - wind;
	float re = (rho * m_width * getVelocityFloat()) / mu;
	float area = PI * pow(m_width / 2, 2);
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

void Projectile::generateWind()
{
	srand(time(0)); 
	// Generate a random wind velocity [-15 -> 15]
	wind.x = rand() % 15 + -15;
	wind.y = rand() % 15 + -15;
}

sf::Vector2f Projectile::getUnitVector(sf::Vector2f v)
{
	float length = getLengthOfVector(v);
	sf::Vector2f u;
	u.x = v.x / length;
	u.y = v.y / length;
	return u;
}

float Projectile::getLengthOfVector(sf::Vector2f v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

void Projectile::updateVelocity(float dt)
{
	float forceDrag = calculateDragforce();
	sf::Vector2f a = getUnitVector(m_velocity) * -forceDrag;
	a = a / m_mass;
	a.y -= g;

	float x = m_velocity.x * dt + a.x * dt * dt * 0.5f;
	float y = m_velocity.y * dt + a.y * dt * dt * 0.5f;

	m_velocity += a * dt;

	// move 
	m_Sprite.move(x, y);



	/*
		Replace this with correct friction calculation
	*/
	//	/// Checks ground collision 
	//if (m_Sprite.getPosition().y > 700.0f - WIDTH_OF_TEXTURE / 2.0f) {
	//	m_Sprite.setPosition(m_Sprite.getPosition().x, m_Sprite.getPosition().y - 0.3f);
	//	m_velocity.y = (m_velocity.y * eKoef);
	//	if (m_velocity.y < 2.0f && m_velocity.y > -2.0f) {
	//		m_velocity.y = (m_velocity.x * 0.6f);
	//	}
	//}

	//	/// Checks wall collision 
	//if (m_Sprite.getPosition().x > 1200 - 8 && m_Sprite.getPosition().x < 1210 && m_Sprite.getPosition().y > 100) {
	//	m_Sprite.setPosition(1200 - 8, m_Sprite.getPosition().y);
	//	m_velocity.x = (m_velocity.x * -0.7f);
	//}

	//	/// Checks wall collision2 
	//if (m_Sprite.getPosition().x < 7) {
	//	m_Sprite.setPosition(7.0f, m_Sprite.getPosition().y);
	//	m_velocity.x = (m_velocity.x * -0.7f);
	//}

	//	/// checks if the ball has stopped 
	//if (m_velocity.x < 0.05f && m_velocity.x > -0.05f &&
	//	m_velocity.y < 0.05f && m_velocity.y > -0.05f &&
	//	m_Sprite.getPosition().y > 700.0f - WIDTH_OF_TEXTURE - 1.0f) {
	//	m_isAirbourne = false;
	//	m_velocity.x = 0;
	//	m_velocity.y = 0;
	//}
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite, states);
}