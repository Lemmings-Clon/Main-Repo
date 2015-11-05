#include <SFML/Graphics.hpp>
#include <iostream>
#include "SFMLtilex.h"
using namespace std;


int main(int argc, const char * argv[])
{
	SFMLtilex* f = new SFMLtilex("Septest.tmx");
	f->showMapContent();

	sf::ContextSettings setting;
	setting.antialiasingLevel = 2;

	sf::RenderWindow window(sf::VideoMode(f->getTotalWidth(),f->getTotalHeight()), "My window", sf::Style::Default, setting);
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				delete f;
				window.close();
		}

		window.clear();
		f->loadTexture();
		f->draw(window);
		window.display();
	}	

	return 0;
}

