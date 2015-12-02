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
	SFMLtilex* f = new SFMLtilex("tiles.tmx");

	//f->showMapContent();
	
	sf::Texture texture;
	if (!texture.loadFromFile("char2.png"))
	{
		std::cout << "Failed to load player spritesheet!" << std::endl;
		return 1;
	}

	Player* player = new Player(f, &texture);
	
	Animation walkingAnimationDown;
	walkingAnimationDown.setSpriteSheet(texture);
	walkingAnimationDown.addFrame(sf::IntRect(32, 0, 32, 32));
	walkingAnimationDown.addFrame(sf::IntRect(64, 0, 32, 32));
	walkingAnimationDown.addFrame(sf::IntRect(32, 0, 32, 32));
	walkingAnimationDown.addFrame(sf::IntRect(0, 0, 32, 32));

	Animation walkingAnimationLeft;
	walkingAnimationLeft.setSpriteSheet(texture);
	walkingAnimationLeft.addFrame(sf::IntRect(32, 32, 32, 32));
	walkingAnimationLeft.addFrame(sf::IntRect(64, 32, 32, 32));
	walkingAnimationLeft.addFrame(sf::IntRect(32, 32, 32, 32));
	walkingAnimationLeft.addFrame(sf::IntRect(0, 32, 32, 32));

	Animation walkingAnimationRight;
	walkingAnimationRight.setSpriteSheet(texture);
	walkingAnimationRight.addFrame(sf::IntRect(32, 64, 32, 32));
	walkingAnimationRight.addFrame(sf::IntRect(64, 64, 32, 32));
	walkingAnimationRight.addFrame(sf::IntRect(32, 64, 32, 32));
	walkingAnimationRight.addFrame(sf::IntRect(0, 64, 32, 32));

	Animation walkingAnimationUp;
	walkingAnimationUp.setSpriteSheet(texture);
	walkingAnimationUp.addFrame(sf::IntRect(32, 96, 32, 32));
	walkingAnimationUp.addFrame(sf::IntRect(64, 96, 32, 32));
	walkingAnimationUp.addFrame(sf::IntRect(32, 96, 32, 32));
	walkingAnimationUp.addFrame(sf::IntRect(0, 96, 32, 32));


	sf::ContextSettings setting;
	setting.antialiasingLevel = 2;
	ShowWindow(GetConsoleWindow(), SW_HIDE);
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

