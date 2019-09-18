#include "gamestate.h"

int main()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	sf::RenderWindow window(sf::VideoMode(1600, 800), "Window");	// window
	sf::Clock gameTime;												// timerr

	Gamestate gameState;
	gameState.initialize();

	// fåglöar
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	int teste2 = 0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		gameState.Update(gameTime.restart().asSeconds());
		
		window.clear();
		window.draw(gameState);
		window.display();
	}

	window.close();

	return 0;
}