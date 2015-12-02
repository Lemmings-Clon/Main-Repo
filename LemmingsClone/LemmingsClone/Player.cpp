#include "Player.hpp"
using namespace std;

Player::Player(SFMLtilex* map) {

	pMap = map;
	setPos(pMap->getStart());
	setTilePos(pMap->getTilePos());
	setBounds(pMap->getBounds());
	pMap->loadTexture();
	setupPlayer();
}


Player::Player(SFMLtilex* map, const sf::Texture* texture){

	pMap = map;
	setPos(pMap->getStart());
	setTilePos(pMap->getTilePos());
	setBounds(pMap->getBounds());
	pMap->loadTexture();
	setupPlayer(texture);
}

void Player::setupPlayer(const sf::Texture* texture) {

	playerShape.setSize(sf::Vector2f(31, 31));
	//playerShape.setFillColor(sf::Color::Green);
	playerShape.setTexture(texture);


	turn = true;
	start = false;
	win = false;
	dead = false;
	speed = 1.85f;
	currentDir = direction::INITIAL;
	setupText();
	setupJumpConfig();

	//pMap->showMapContent();
}

void Player::setupPlayer(){

	playerShape.setSize(sf::Vector2f(31,31));
	playerShape.setFillColor(sf::Color::Green);

	turn = true;
	start = false;
	win = false;
	dead = false;
	speed = 1.85f;
	currentDir = direction::INITIAL;
	setupText();
	setupJumpConfig();

	//pMap->showMapContent();
}

void Player::setupText(){
	font.loadFromFile("C:/Windows/Fonts/CHILLER.TTF");
	text.setFont(font);
	text.setCharacterSize(95);
	text.setColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
}

void Player::move(direction dir){

	if(currentDir!=dir&&!win){
		currentDir = dir;
		turn = true;
		start = true;
		walk = true;
	}

	if(dir==direction::STOP)
		walk = false;

	if(dir==direction::INITIAL && win)
		start = false;

}

void Player::move(){
	if(!start)
		return;

	if(win){
		move(direction::INITIAL);
		text.setString("W I N ! !");
		text.setPosition(bounds.x/2-50, bounds.y/2-30);
		return;

	}

	if(dead){
		win=true;
		move(direction::INITIAL);
		text.setString("D E A D ! !");
		text.setPosition(bounds.x/2-60, bounds.y/2-40);
		playerShape.setFillColor(sf::Color::Black);
		return;

	}
	if(turn){
		
		step = 0;
		acc = 1;
		turn = false;
		starttime = clock.getElapsedTime().asMilliseconds()/10;
		steps = 0;
	}



	while(steps < clock.getElapsedTime().asMilliseconds()/10-starttime){
		++steps;
		// Bewegung in x richtung

		if(walk)
			moveX();

		//Bewegung in Y richtung
		moveY();

	}
}

void Player::moveX(){

	float xOld = pos.x;
	float moveStep = step*(0.002f);
	if(currentDir==direction::LEFT)
		moveStep*=-1;

	setX(pos.x+moveStep);
	//cout << "MOVE" << endl;
	/* check left */
	if(pos.x<0)
		setX(0);

	/* check right */
	if((pos.x+32)>bounds.x)
		setX(bounds.x-32);


	for(int counter=0; counter < cTilePos.size(); ++counter){
		tilePos tCurrent = cTilePos[counter];
		if(playerShape.getGlobalBounds().intersects(tCurrent.tilepos.getGlobalBounds())){
			switch(cTilePos[counter].type)
			{
			case tileshape::UPPERPORTAL:
			case tileshape::LOWERPORTAL:
				cout << " WIN!! " << endl;
				win = true;
				return;
			case tileshape::LEFTBLOCK:
			case tileshape::RIGHTBLOCK:
			case tileshape::MIDDLEBLOCK:
				//	cout << " STOP " << endl;
				setX(xOld);
				if(isFalling||isJumping)
					move(direction::STOP);
				break;
			case tileshape::DIGBLOCK:
				if (walk == false) {
					isFalling = true;
				}
				else {
					setX(xOld);
					if (isFalling || isJumping)
						move(direction::STOP);
				}
				break;
			case tileshape::LAVA:
				dead = true;
				return;
			default:break;
			}

		}
	}

	if(step<1000){
		if(acc<30)
			++acc;
		step+= (acc*speed);
	}

	if(step>1000){
		step = 1000;
	}

}

void Player::moveY(){

	float yOld = pos.y;

	/* Wenn der block nicht springt oder fällt, setzte neue startzeit */
	if(!isFalling&&!isJumping)
		starttimeFall = (float)clock.getElapsedTime().asMilliseconds()/100;

	t = (float)clock.getElapsedTime().asMilliseconds()/100;
	t-=starttimeFall; //t=die Zeit die seit Beginn des Sprungs/falls vergangen ist


	v = g * ++t - jumpImp;

	if(v>maxFallSpeed)
		v = maxFallSpeed;

	setY(pos.y+v);
	bool mayFall = true;

	if((pos.y+31)>bounds.y){  //wenn der Player den Boden berührt, gibt es keine Y-Bewegung
		setY(bounds.y-31);
		isJumping=isFalling=mayFall=false;
		jumpImp=0;

	}else{
		for(int counter=0; counter < cTilePos.size(); ++counter){
			tilePos tCurrent = cTilePos[counter];
			if(playerShape.getGlobalBounds().intersects(tCurrent.tilepos.getGlobalBounds())){

				switch(cTilePos[counter].type){
				case tileshape::UPPERPORTAL:
				case tileshape::LOWERPORTAL:
					cout << " WIN!! " << endl;
					win = true;
					return;
				case tileshape::LEFTBLOCK:
				case tileshape::RIGHTBLOCK:
				case tileshape::MIDDLEBLOCK:
					isJumping=isFalling=mayFall = false;
					jumpImp=0;
					if(v<=0){ //wenn er springt
						setY(tCurrent.tilepos.getGlobalBounds().top+tCurrent.tilepos.getGlobalBounds().height);
					}else{ //wenn er fällt
						setY(tCurrent.tilepos.getGlobalBounds().top-31);		
					}
					break;
				case tileshape::DIGBLOCK:
					if (walk == false) {
						isFalling = true;
					}
					else {
						isJumping = isFalling = mayFall = false;
						jumpImp = 0;
						if (v <= 0) { //wenn er springt
							setY(tCurrent.tilepos.getGlobalBounds().top + tCurrent.tilepos.getGlobalBounds().height);
						}
						else { //wenn er fällt
							setY(tCurrent.tilepos.getGlobalBounds().top - 31);
						}
					}
					break;
				case tileshape::LAVA:
					dead = true;
					return;
				}
			}
		}
	}
	if(mayFall)
		isFalling = true;
}

void Player::jump(){

	if(isJumping || isFalling)
		return;
	jumpImp = configJumpImp;
	isJumping = true;


}

void Player::drawPlayer(sf::RenderWindow& window){
	if(win)
		window.draw(text);

	window.draw(playerShape);
}

void Player::setupJumpConfig(){
	oldHeight = -1;
	//jump variables
	maxFallSpeed = 5;
	configJumpImp = 10;
	g = 0.981f;
	v = 0;
	t = 0;
	isJumping = isFalling = false;
	starttimeFall = 0;
}