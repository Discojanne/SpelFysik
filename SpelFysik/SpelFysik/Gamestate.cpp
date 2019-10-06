#include "Gamestate.h"

Game::Game() :m_Projectile()
{
	m_BackgroundTexture.loadFromFile("../Resources/background2.jpg");
	m_BackgroundSprite.setTexture(m_BackgroundTexture);

	this->stream << 0;
	this->text.setString(stream.str()); //texten i spelet
	this->font.loadFromFile("../Resources/Arcon-Regular.otf");
	this->text.setFont(font);
}

Game::~Game()
{
}

void Game::Update(float dt)
{
	m_Projectile.Update(dt);

	if (m_Projectile.getPos().x != 8 && m_Projectile.getPos().y != 8)
	{
		this->stream.str("");					//tömmer streamen
		this->stream
			<< "x: " << this->m_Projectile.getPos().x << std::endl
			<< "Vx: " << m_Projectile.getVelocity().x << std::endl
			<< "Vy: " << m_Projectile.getVelocity().y << std::endl
			<< "Resistans: " << m_Projectile.calculateDragforce();
	}
	this->text.setString(stream.str());	/* Update text with new stream */

	//cout << m_Projectile.getVelocity().getY() << endl;
	//cout << "x position: " << m_Projectile.getPos().getX() << ", y position: " << m_Projectile.getPos().getY() << endl;
}

bool Game::initialize()
{

	float i, j, velocity;
	std::cout << "Angle: ";
	std::cin >> i;

	std::cout << "Ammout of gumpowder: ";
	std::cin >> j;
	

	velocity = m_Projectile.calculateStartVelocity((int)j);

	float test = cosf(i * PI / 180) * velocity;

	m_Projectile.setStartAngle(sf::Vector2f(cosf(i * PI / 180) * velocity, sinf(i * PI / 180) * velocity));
	m_Projectile.generateWind();


	return true;
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Make sure everything in the game is drawn.
	target.draw(m_BackgroundSprite, states);
	target.draw(m_Projectile, states);
	target.draw(this->text);
}