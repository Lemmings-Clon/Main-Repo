#include "Game.hpp"
#include <Windows.h>


Game::Game() {
	running = true;
	playedIntro = false;

	//check settings for fullscreen
	checkScreenSet();

	//window stuff
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	//hide the console
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	//Sound and Music
}

Game::~Game() {
	CurrentState = nullptr;
}

void Game::Run() {
	while (running) {


		//trap mouse curser in window
		HWND hwnd;
		hwnd = FindWindow(0, LPCWSTR("Pew"));
		RECT r;
		//top left coords
		r.left = window.getPosition().x;
		r.top = window.getPosition().y;
		//bottom right coords
		r.right = window.getPosition().x + window.getSize().x;
		r.bottom = window.getPosition().y + window.getSize().y;

		//clip mouse to window
		GetWindowRect(hwnd, &r);
		ClipCursor(&r);

		//do the game stuff
		Update();
		HandleEvents();
		Render();
		Quit();
	}
}

void Game::ChangeState(gameStates newState) {
	switch (newState) {
	/*case gameStates::MAINMENU:
		CurrentState = std::move(std::unique_ptr<MainMenuState>(new MainMenuState));
		menMusic.PlayMusic("menu");
		ingMusic.Pause();
		break;
	case gameStates::INTRO:
		CurrentState = std::move(std::unique_ptr<Intro>(new Intro));
		playedIntro = true;
		menMusic.Pause("menu");
		menMusic.PlayMusic("intro");
		break;*/
	case gameStates::PLAY:
		CurrentState = std::move(std::unique_ptr<PlayState>(new PlayState));
		break;
	/*case gameStates::HIGHSCORE:
		CurrentState = std::move(std::unique_ptr<Highscore>(new Highscore));
		break;
	case gameStates::SETTINGS:
		CurrentState = std::move(std::unique_ptr<Settings>(new Settings));
		checkScreenSet();
		break;
	case gameStates::DIFFSET:
		CurrentState = std::move(std::unique_ptr<DiffSet>(new DiffSet));
		break;
	case gameStates::GRAPHICSET:
		CurrentState = std::move(std::unique_ptr<GraphicSet>(new GraphicSet));
		break;
	case gameStates::SOUNDSET:
		CurrentState = std::move(std::unique_ptr<SoundSet>(new SoundSet));
		break;
	case gameStates::HIGHSCORELIST:
		CurrentState = std::move(std::unique_ptr<HighscoreList>(new HighscoreList));
		break;
	case gameStates::SPLASHSTATE:
		CurrentState = std::move(std::unique_ptr<SplashState>(new SplashState));*/
	}
}

void Game::setRunning(bool mRunning) {
	running = mRunning;
}

void Game::Quit() {
	if (!running)
		window.close();
}
void Game::Update() {
	CurrentState->Update(*this);
}
void Game::HandleEvents() {
	CurrentState->HandleEvents(*this);
}
void Game::Render() {
	window.clear();
	CurrentState->Render(*this);
	window.display();
}



void Game::checkScreenSet() {
	//check settings for fullscreen

/*	if (ioscreen.getScreenSettings()) {
		window.create(sf::VideoMode(800, 600, 32), "Pew", sf::Style::Fullscreen);
		window.setMouseCursorVisible(true);
	}
	else
		window.create(sf::VideoMode(800, 600, 32), "Pew", sf::Style::Titlebar);*/
}