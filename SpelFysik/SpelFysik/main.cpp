#include "Gamestate.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	sf::RenderWindow window(sf::VideoMode(1600, 800), "Fysik projekt Emma David");	// window
	sf::Clock gameTime;																// timer
	Game game;

	game;
	game.initialize(); // set start angle, 

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// game code in here //
		game.Update(gameTime.restart().asSeconds());

		window.clear();
		window.draw(game);
		window.display();
	}

	window.close();

	return 0;
}
