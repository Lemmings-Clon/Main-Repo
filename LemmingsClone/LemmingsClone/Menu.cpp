#include "Menu.h"


Menu::Menu(float width, float height)
{
	font.loadFromFile("C:/Windows/Fonts/CHILLER.TTF");

	headline[0].setFont(font);
	headline[0].setColor(sf::Color::Red);
	headline[0].setString("Portal-Guy");
	headline[0].setCharacterSize(58);
	headline[0].setPosition(sf::Vector2f((width / 2)-80, height / (MAX_NUMBER_OF_ITEMS + 20)));

	menu[0].setFont(font);
	menu[0].setColor(sf::Color::Red);
	menu[0].setString("Level1");
	menu[0].setPosition(sf::Vector2f((width / 2) - 20, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

	menu[1].setFont(font);
	menu[1].setColor(sf::Color::White);
	menu[1].setString("Level2");
	menu[1].setPosition(sf::Vector2f((width / 2) - 20, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

	menu[2].setFont(font);
	menu[2].setColor(sf::Color::White);
	menu[2].setString("Level3");
	menu[2].setPosition(sf::Vector2f((width / 2) - 20, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

	menu[3].setFont(font);
	menu[3].setColor(sf::Color::White);
	menu[3].setString("Level4");
	menu[3].setPosition(sf::Vector2f((width / 2) - 20, height / (MAX_NUMBER_OF_ITEMS + 1) *4));

	menu[4].setFont(font);
	menu[4].setColor(sf::Color::White);
	menu[4].setString("Exit");
	menu[4].setPosition(sf::Vector2f((width / 2) - 20, height / (MAX_NUMBER_OF_ITEMS + 1) * 5));

	selectedItemIndex = 0;
}


Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow &window)
{
	window.draw(headline[0]);
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window.draw(menu[i]);
	}
}

void Menu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
}