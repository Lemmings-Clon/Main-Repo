#ifndef PLAYER_H
#define PLAYER_H

#include "SFML\Graphics.hpp"
#include <iostream>
#include "SFMLtilex.hpp"

class Player {
public: enum class direction {LEFT, RIGHT, STOP,INITIAL=-1};
		void move();
		void moveX();
		void moveY();
		void move(direction dir);
		std::vector<tilePos> cTilePos;

private:
	//walk variables
	int steps; //zählt wie viele schritte in der vergangenen Zeit geangen wurden
	bool turn;  //schaut nach ob die richtung gewechselt wurde
	float acc;  //beschleunigung
	bool start;
	bool walk; //schaut ob der Player gerade läuft
	direction currentDir; //die richtung in die der player läuft
	float step; //ist die größe der schritte
	float speed; //die geschwindigkeit (geschwindigkeit * beschleunigung = eigentliche geschwindigketigeit)

	// Temp variables
	sf::Text text;	
	sf::Font font;
	void setupText();
	sf::Clock clock;
	int starttime;

	//other
	SFMLtilex* pMap;
	sf::RectangleShape playerShape;
	sf::Vector2f bounds;
	sf::Vector2f pos;
	void setupPlayer();
	bool win;
	bool dead;

	void refreshPos(){playerShape.setPosition(pos);};	
	
	
	double oldHeight; //debug variable

	//jump variables
	short maxFallSpeed;
	float jumpImp;
	float configJumpImp;
	float v;
	float t;
	float g;
	bool isJumping;
	bool isFalling;
	float startY;
	float starttimeFall;
	void setupJumpConfig();


public:
	
	Player(SFMLtilex* map);
	//void setMap(SFMLtilex* map){ pMap= map;};
	void setTilePos(std::vector<tilePos> pos){cTilePos = pos;};
	void setBounds(sf::Vector2f bnd){bounds = bnd;};
	sf::RectangleShape getPlayer(){return playerShape;};
	void jump();
	void setPos(sf::Vector2f v){pos = v; refreshPos();};
	void setX(float x){pos.x=x;refreshPos();};
	void setY(float y){pos.y=y;refreshPos();};
	void drawPlayer (sf::RenderWindow &window);
	void drawMap(sf::RenderWindow &window){pMap->draw(window);};
	void draw(sf::RenderWindow& window){drawMap(window);drawPlayer(window);};

};

#endif