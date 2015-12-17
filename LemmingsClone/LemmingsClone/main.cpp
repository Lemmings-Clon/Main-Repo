#include <SFML/Graphics.hpp>
#include <iostream>
#include "SFMLtilex.hpp"
#include "Player.hpp"
#include "windows.h"
#include "AnimatedSprite.hpp"

using namespace std;

bool showConsole = false;

int main(int argc, const char * argv[])
{
	SFMLtilex* f = new SFMLtilex("prototype.tmx");

	//f->showMapContent();
	
	sf::Texture texture;
	if (!texture.loadFromFile("char3.png"))
	{
		std::cout << "Failed to load player spritesheet!" << std::endl;
		return 1;
	}

	Player* player = new Player(f);
	player->setTexture(&texture);

	sf::ContextSettings setting;
	setting.antialiasingLevel = 2;
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	int w, h;
	sf::RenderWindow window(sf::VideoMode( w = f->getTotalWidth(), h = f->getTotalHeight()), "PortalGuy", sf::Style::Default, setting);
	
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed){
				delete player;
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
				case sf::Keyboard::M:
					player->consoleMap();
					break;
				case sf::Keyboard::D:
					player->dig();
					break;
				case sf::Keyboard::E:
					player->setClimb();
					break;
				case sf::Keyboard::R:
					delete player;
					player = new Player(f);
					player->setTexture(&texture);
					break;
				case sf::Keyboard::S:
					player->toogleSpeedText();
					break;
				case sf::Keyboard::I:
					player->showIntersectCounter();
					break;
				case sf::Keyboard::C:
					showConsole = !showConsole;
					if(showConsole)
					ShowWindow(GetConsoleWindow(), SW_SHOW);
					else
					ShowWindow(GetConsoleWindow(), SW_HIDE);

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
					break;
				case sf::Keyboard::F1:
					player->toogleControlText();
					break;
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

