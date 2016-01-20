#ifndef PLAYER_H
#define PLAYER_H

#include "SFML\Graphics.hpp"
#include <iostream>
#include <string>
#include "SFMLtilex.hpp"

class Player {
public: enum class direction { LEFT, RIGHT, STOP, INITIAL = -1 };
		void move();
		void moveX();
		void moveY();
		void move(direction dir);
		std::vector<tilePos> cTileColMap;
		void showIntersectCounter() { showIntersect = true; }
		bool isDead();
		void setDead(bool rev);
		void setupPlayer();
		void setupPlayer(const sf::Texture* texture);

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
	sf::Text helpText;
	sf::Text speedText;
	bool showSpeedText;
	sf::Text controlText;
	bool showControlText;

	bool showIntersect = false;
	unsigned long testcounter = 0;


	sf::Font font;
	void setupText();
	sf::Clock clock;
	int starttime;

	//other
	SFMLtilex* pMap;
	const int playerSize = 28;
	const int playerHeight = 46;
	const int playerWidth = 28;
	sf::RectangleShape playerShape;
	sf::Vector2f bounds;
	sf::Vector2f pos;
	bool win;
	bool dead;
	bool cDig;
	bool cClimb;

	void refreshPos() { playerShape.setPosition(pos); };


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
	void inline
		setTexture(const sf::Texture* texture) { playerShape.setFillColor(sf::Color::White); playerShape.setTexture(texture); };
	//void setMap(SFMLtilex* map){ pMap= map;};
	void inline setTilePos(std::vector<tilePos> pos) { cTileColMap = pos; };
	void inline setBounds(sf::Vector2f bnd) { bounds = bnd; };
	sf::RectangleShape inline getPlayer() { return playerShape; };
	void jump();
	void inline setPos(sf::Vector2f v) { pos = v; refreshPos(); };
	void inline setX(float x) { pos.x = x; refreshPos(); };
	void inline setY(float y) { pos.y = y; refreshPos(); };
	void drawPlayer(sf::RenderWindow &window);
	void inline drawMap(sf::RenderWindow &window) { pMap->draw(window); };
	void inline draw(sf::RenderWindow& window) { drawMap(window); drawPlayer(window); };
	void consoleMap() { pMap->showMapContent(); }
	void dig() { if (cDig == false && start == true){ cDig = true;}}
	void setClimb();
	void climb();
	void inline toogleSpeedText() { showSpeedText = !showSpeedText; }
	void inline toogleControlText() { showControlText = !showControlText; }
};

#endif