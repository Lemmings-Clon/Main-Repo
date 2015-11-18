#include <SFML/Graphics.hpp>
#include <iostream>
#include "SFMLtilex.hpp"
#include "Player.hpp"
using namespace std;



int main(int argc, const char * argv[])
{
	SFMLtilex* f = new SFMLtilex("tiles.tmx");

	//f->showMapContent();
	

	Player* player = new Player(f);


	sf::ContextSettings setting;
	setting.antialiasingLevel = 2;

	sf::RenderWindow window(sf::VideoMode(f->getTotalWidth(),f->getTotalHeight()), "PortalGuy", sf::Style::Default, setting);
	
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed){
				delete f;
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::R:
					f->reloadMap();
					f->loadTexture();
					break;
				case sf::Keyboard::A:
					f->changeColMap();
					break;
				case sf::Keyboard::Left:
					player->move(Player::direction::LEFT);
					break;
				case sf::Keyboard::Right:
					player->move(Player::direction::RIGHT);
					break;
				case sf::Keyboard::Down:
					player->move(Player::direction::STOP);
					break;
				case sf::Keyboard::Space:
					player->jump();
				default:
					break;
				}
			}


		}

		window.clear();
		player->move();
		player->draw(window);
		window.display();
	}	//somechange

	return 0;
}

