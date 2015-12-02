#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "GameState.hpp"
#include "Game.hpp"

#include <string>

#define MAX_LEVEL 3

class PlayState : public GameState {


	std::string levelnames[MAX_LEVEL] = { "level1.tmx", "level2.tmx", "level3.tmx" };
	

public:
	PlayState();
	~PlayState() {};
	

	void HandleEvents(Game &game) {};
	 void Update(Game &game) {};
	 void Render(Game &game) {};








};



#endif