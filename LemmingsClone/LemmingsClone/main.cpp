#include "SFML/Graphics.hpp"
#include <iostream>
#include "Menu.h"
#include "Game.hpp"
#include "Player.hpp"
#include "SFMLtilex.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "Portal Guy");
	Menu menu(window.getSize().x, window.getSize().y);

	Game* level = new Game();

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					menu.MoveUp();
					break;

				case sf::Keyboard::Down:
					menu.MoveDown();
					break;

				case sf::Keyboard::Q:
					window.close();
					break;
				
				case sf::Keyboard::Return:
					switch (menu.GetPressedItem())
					{
					case 0:
						level->setRunning(true);
						level->setLevel(1);
						level->Run();
						break;
					case 1:
						level->setRunning(true);
						level->setLevel(2);
						level->Run();
						break;
					case 2:
						level->setRunning(true);
						level->setLevel(3);
						level->Run();
						break;
					case 3:
						level->setRunning(true);
						level->setLevel(4);
						level->Run();
						break;
					case 4:
						window.close();
						break;
					}

					break;
				}

				break;
			case sf::Event::Closed:
				window.close();

				break;

			}
		}

		window.clear();

		menu.draw(window);

		window.display();
	}
}