#include "Player.hpp"
using namespace std;

Player::Player(SFMLtilex* map) {

	pMap = map;
	cDig = false;
	cClimb = false;
	showSpeedText = false;
	showControlText = false;
	jumpImp = 0;
	sf::Vector2f tempStartPos(pMap->getStart());
	tempStartPos.y = pMap->getStart().y + pMap->getTileHeight() - playerHeight;
	setPos(tempStartPos);	
	setTilePos(pMap->getTilePos());
	setBounds(pMap->getBounds());
	pMap->loadTexture();
	setupPlayer();
}


void Player::setupPlayer(){

	playerShape.setSize(sf::Vector2f(playerWidth, playerHeight));
	//playerShape.setFillColor(sf::Color::Green);

	turn = true;
	start = false;
	win = false;
	cClimb = false;
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

	helpText.setFont(font);
	helpText.setCharacterSize(25);
	helpText.setColor(sf::Color::Black);
	helpText.setPosition(10, 10);
	helpText.setString("Press F1 for controls");
	
	speedText.setFont(font);
	speedText.setCharacterSize(22);
	speedText.setColor(sf::Color::Black);
	speedText.setPosition(bounds.x - 50, 10);

	controlText.setFont(font);
	controlText.setCharacterSize(35);
	controlText.setColor(sf::Color::Black);
	controlText.setPosition(bounds.x / 2 - 100, 15);
		
	string s = "Escape:\t Exit\n";
		  s += "Arrows: \tMove\n";
			s+="Space: \t Jump\n";
			s+= "D:  \t\tdig\n";
			s += "E:  \t\tclimbUp\n";
			s += "R:  \t\trestart\n\n";
			s+="C:  \t\tshow developer Console\n";
			s+="M:  \t\tshow Map in Console\n";
			s+="A:  \t\ttoogle Collision Map\n";
			s+="S:  \t\ttoogle Speed Text\n";
			s+="I:  \t\tshow Intersect Counter\n";
			s+="F1: \t\tshow controls\n";

	controlText.setString(s);

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

		if (walk) {
			moveX();
		}
		if (cClimb) {
			climb();
		}
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
	if((pos.x+playerWidth)>bounds.x)
		setX(bounds.x-playerWidth);


	for(int counter=0; counter < cTileColMap.size(); ++counter){
		tilePos tCurrent = cTileColMap[counter];
		if(playerShape.getGlobalBounds().intersects(tCurrent.tilepos.getGlobalBounds())){
			switch(cTileColMap[counter].type)
			{
			case tileshape::UPPERPORTAL:
			case tileshape::LOWERPORTAL:
				cout << " WIN!! " << endl;
				win = true;
				
				return;
	
			case tileshape::MIDDLEBLOCK:
			case tileshape::MIDDLEBREAK_1:
			case tileshape::MIDDLEBREAK_2:
			case tileshape::LEFTBLOCK:
			case tileshape::RIGHTBLOCK:
				//	cout << " STOP " << endl;
				cClimb = false;
				setX(xOld);
				if(isFalling||isJumping)
					move(direction::STOP);
				break;
		/*	case tileshape::DIGBLOCK:
				if (walk == false) {
					isFalling = true;
				}
				else {
					setX(xOld);
					if (isFalling || isJumping)
						move(direction::STOP);
				}
				break;*/
			case tileshape::LADDER:
				if (!walk) {
					cClimb = false;
				}
				if (cClimb && !isJumping && !isFalling) {
					climb();
				}
				break;
			case tileshape::LAVA:
			case tileshape::SPIKE_UP:
			case tileshape::SPIKE_DOWN:
				dead = true;
				return;
			default: break;
			}

		}
	}

	//cDig = false;

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
	bool die = false;
	if (v > maxFallSpeed) {
		v = maxFallSpeed;
		speedText.setColor(sf::Color::Red);
		cDig = true;
		die = true;
	}
	speedText.setString(std::to_string(v));
	setY(pos.y+v);
	bool mayFall = true;

	if((pos.y+playerHeight)>bounds.y){  //wenn der Player den Boden berührt, gibt es keine Y-Bewegung
		setY(bounds.y-playerHeight);
		isJumping=isFalling=mayFall=false;
		jumpImp=0;
		if (die)
			dead = true;

	}else{
		++testcounter;
		float newPosition = pos.y;
		for(int counter=0; counter < cTileColMap.size(); ++counter){
			tilePos tCurrent = cTileColMap[counter];
			if(playerShape.getGlobalBounds().intersects(tCurrent.tilepos.getGlobalBounds())){
				if (showIntersect) {
					cout << testcounter << ", intersect." << endl;
					cout << "X: " << tCurrent.tilepos.getPosition().x << " Y: " << tCurrent.tilepos.getPosition().y << endl;
					}switch (cTileColMap[counter].type) {
				case tileshape::UPPERPORTAL:
				case tileshape::LOWERPORTAL:
					cout << " WIN!! " << endl;
					win = true;
					return;
				case tileshape::MIDDLEBLOCK:
					if (cDig && 
						playerShape.getPosition().x+playerWidth/2-1<=tCurrent.tilepos.getPosition().x+tCurrent.tilepos.getGlobalBounds().width &&
						playerShape.getPosition().x + playerWidth / 2+1 > tCurrent.tilepos.getPosition().x) {
						
						//std::cout << "Dig1" << endl;
						pMap->digBlock((int) cTileColMap[counter].tilepos.getPosition().x / pMap->getTileWidth(),
							(int) cTileColMap[counter].tilepos.getPosition().y / pMap->getTileHeight(),
							tileshape::MIDDLEBREAK_1);
						cDig = false;
						pMap->calcStart();
						setTilePos(pMap->getTilePos());
					}
				case tileshape::MIDDLEBREAK_1:
					if (cDig && 
						playerShape.getPosition().x + playerWidth / 2-1 <= tCurrent.tilepos.getPosition().x + tCurrent.tilepos.getGlobalBounds().width &&
						playerShape.getPosition().x + playerWidth / 2+1 > tCurrent.tilepos.getPosition().x) {
						//std::cout << "Dig2!" << endl;
						pMap->digBlock((int)cTileColMap[counter].tilepos.getPosition().x / pMap->getTileWidth(),
							(int)cTileColMap[counter].tilepos.getPosition().y / pMap->getTileHeight(),
							tileshape::MIDDLEBREAK_2);
						cDig = false;
						pMap->calcStart();
						setTilePos(pMap->getTilePos());
					}
				case tileshape::MIDDLEBREAK_2:
					if (cDig && 
						playerShape.getPosition().x + playerWidth / 2 -1<= tCurrent.tilepos.getPosition().x + tCurrent.tilepos.getGlobalBounds().width &&
						playerShape.getPosition().x + playerWidth / 2 +1> tCurrent.tilepos.getPosition().x) {
						//std::cout << "Dig3!" << endl;
						pMap->digBlock((int)cTileColMap[counter].tilepos.getPosition().x / pMap->getTileWidth(),
							(int)cTileColMap[counter].tilepos.getPosition().y / pMap->getTileHeight(),
							tileshape::SKY);
						cDig = false;
						pMap->calcStart();
						setTilePos(pMap->getTilePos());
					}
				case tileshape::LEFTBLOCK:
				case tileshape::RIGHTBLOCK:
					isJumping=isFalling=mayFall = false;
					cClimb = false;
					jumpImp=0;
					if(v<=0){ //wenn er springt
						newPosition= tCurrent.tilepos.getGlobalBounds().top+tCurrent.tilepos.getGlobalBounds().height;
					}else{ //wenn er fällt
						newPosition = tCurrent.tilepos.getGlobalBounds().top-playerHeight;
					}
					if (die)
						dead = true;
					break;
				case tileshape::LADDER:
					if (!walk) {
						cClimb = false;
					}
					if (cClimb && !isJumping && !isFalling) {
						climb();
					}
					break;
			/*	cased tileshape::DIGBLOCK:
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
					break;*/
				case tileshape::LAVA:
					dead = true;
					return;
				case tileshape::SPIKE_UP:
				case tileshape::SPIKE_DOWN:
					dead = true;
					return;
				}
			}
		}
		setY(newPosition);
		showIntersect = false;
	}
	if(mayFall)
		isFalling = true;
	cDig = false;
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

	window.draw(helpText);

	if (showSpeedText)
		window.draw(speedText);

	if (showControlText)
		window.draw(controlText);
}

void Player::setupJumpConfig(){
	oldHeight = -1;
	//jump variables
	maxFallSpeed = 10;
	configJumpImp = 4.2;
	g = 0.981f;
	v = 0;
	t = 0;
	isJumping = isFalling = false;
	starttimeFall = 0;
}

bool Player::isDead() {
	return dead;
}

void Player::setDead(bool rev) {
	dead = rev;
	win = rev;
}

void Player::setClimb() {
	cClimb = true;
}

void Player::climb() {

	float yOld = pos.y;
	float moveStep = step*(0.002f);
	
	setY(pos.y - moveStep);
	//cout << "MOVE" << endl;
	/* check left */
	if (pos.y<0)
		setY(0);

	/* check right */
	if ((pos.y + playerHeight)>bounds.y)
		setY(bounds.y - playerHeight);


	for (int counter = 0; counter < cTileColMap.size(); ++counter) {
		tilePos tCurrent = cTileColMap[counter];
		if (playerShape.getGlobalBounds().intersects(tCurrent.tilepos.getGlobalBounds())) {
			switch (cTileColMap[counter].type)
			{
			case tileshape::UPPERPORTAL:
			case tileshape::LOWERPORTAL:
				cout << " WIN!! " << endl;
				win = true;

				return;

			
			case tileshape::LADDER:
				if (!walk) {
					cClimb = false;
				}
				if (cClimb && !isJumping && !isFalling) {
					climb();
				}
				break;
			case tileshape::LAVA:
			case tileshape::SPIKE_UP:
			case tileshape::SPIKE_DOWN:
				dead = true;
				return;
			default: break;
			}

		}
	}

	//cDig = false;

	if (step<1000) {
		if (acc<30)
			++acc;
		step += (acc*speed);
	}

	if (step>1000) {
		step = 1000;
	}

}