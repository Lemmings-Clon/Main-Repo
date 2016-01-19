#include "Game.hpp"
#include "SFMLtilex.hpp"
#include "Player.hpp"
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


		SFMLtilex* f = new SFMLtilex("prototype.tmx");

		//f->showMapContent();

		sf::Texture texture;
		if (!texture.loadFromFile("char3.png"))
		{
			std::cout << "Failed to load player spritesheet!" << std::endl;
		}

		Player* player = new Player(f);
		player->setTexture(&texture);

		sf::ContextSettings setting;
		setting.antialiasingLevel = 2;
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		int w, h;
		sf::RenderWindow window(sf::VideoMode(w = f->getTotalWidth(), h = f->getTotalHeight()), "PortalGuy", sf::Style::Default, setting);

		while (running)
		{
			// check all the window's events that were triggered since the last iteration of the loop
			sf::Event event;
			while (window.pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed) {
					delete player;
					delete f;
					window.close();
				}
				if (event.type == sf::Event::KeyPressed)
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Escape:
						running = false;
						Quit();
						break;
					case sf::Keyboard::M:
						player->consoleMap();
						break;
					case sf::Keyboard::D:
						player->dig();
						break;
					case sf::Keyboard::E:
						player->setClimb();
						break;
					case sf::Keyboard::R:
						delete player;
						delete f;
						f = new SFMLtilex("prototype.tmx");
						player = new Player(f);
						player->setTexture(&texture);
						break;
					case sf::Keyboard::S:
						player->toogleSpeedText();
						break;
					case sf::Keyboard::I:
						player->showIntersectCounter();
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
						break;
					case sf::Keyboard::F1:
						player->toogleControlText();
						break;
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