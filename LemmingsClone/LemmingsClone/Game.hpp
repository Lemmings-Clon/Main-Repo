//Game.h

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Gamestate.hpp"
#include "PlayState.hpp"

class Game {
public:
	Game();
	~Game();

	enum class gameStates { MAINMENU, INTRO, PLAY, SETTINGS, GRAPHICSET, SOUNDSET, DIFFSET, HIGHSCORE, HIGHSCORELIST, SPLASHSTATE };

	void Run();
	void ChangeState(gameStates newState);
	void ChangeLevel(int level) { cLevel = level; }
	static int getLevel() {	return cLevel;}

	const bool isRunning() { return running; }
	const bool getIntroPlayed() { return playedIntro; }

	void setRunning(bool mRunning);
	void setIntroPlayed(bool played) { playedIntro = played; };

	sf::RenderWindow	window;
	
private:
	void Quit();
	void Update();
	void HandleEvents();
	void Render();

	void checkScreenSet();

	std::unique_ptr<GameState> CurrentState;

	bool running;
	bool playedIntro;
	static int cLevel;
};

#endif