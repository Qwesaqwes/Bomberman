#include "GameLogic.hpp"
#include "GameRenderer.hpp"

#include "Player.hpp" // TODO: this should be done by a "game initializer" and not by the game engine!

GameLogic::GameLogic()
{
	canRun = false;
	running = false;

	// Map size parsing
	mapW = MAP_SIZE;
	mapH = MAP_SIZE;
	int maxSizeW = (WINDOW_W - 2 * WINDOW_MIN_X_OFFSET) / mapW;
	int maxSizeH = (WINDOW_H - 2 * WINDOW_MIN_Y_OFFSET) / mapH;
	squareSize = (maxSizeH < maxSizeW) ? maxSizeH : maxSizeW;
	xOffset = (WINDOW_W - squareSize * mapW) / 2;
	yOffset = (WINDOW_H - squareSize * mapH) / 2;

	// Create interface class
	graphicLib = new GameRenderer(this);
	if (!graphicLib)
	{
		std::cerr << "GameRenderer couldn't load !" << std::endl;
		return;
	}

	// Create audio manager
	audioManager = new AudioManager();
	if (!audioManager)
	{
		std::cerr << "AudioManager couldn't load !" << std::endl;
		return;
	}

	// TODO: this should be done by a "game initializer" and not by the game engine!
	entities.push_back(new Player());

	// Everything good
	canRun = true;
}

GameLogic::GameLogic(GameLogic const &src)
{
	*this = src;
	return;
}

GameLogic::~GameLogic(void)
{
	for (auto entity : entities)
	{
		delete entity;
	}
	delete (audioManager);
	delete (graphicLib);
	return;
}

int GameLogic::getSquareSize(void)
{
	return squareSize;
}

int GameLogic::getXOffset(void)
{
	return xOffset;
}

int GameLogic::getYOffset(void)
{
	return yOffset;
}

int GameLogic::getMapW(void)
{
	return mapW;
}

int GameLogic::getMapH(void)
{
	return mapH;
}

int GameLogic::getPlayerDirection(void)
{
	return playerDirection;
}

bool GameLogic::getIsPlayerAlive(void)
{
	return isPlayerAlive;
}

std::tuple<int, int> &GameLogic::getPlayerPos(void)
{
	return playerPos;
}

GameLogic &GameLogic::operator=(GameLogic const &rhs)
{
	this->canRun = rhs.canRun;
	return *this;
}

void GameLogic::printUsage(void)
{
	std::cerr << "Usage: ./bomberman" << std::endl;
}

void GameLogic::changeLibraryRequest(std::string keyCode)
{
	int requestedIndex = std::stoi(keyCode);

	// std::cout << "Change index of library to: " << requestedIndex << std::endl;
	if (requestedIndex >= 0 && requestedIndex <= 0)
	{
		dlIndex = requestedIndex;
	}
}

void GameLogic::updateGameState(void)
{
	// Get all pool events in library
	graphicLib->getUserInput();

	// Check if we want to close window, in this case no need to do further calculations
	if (dlIndex == 0)
	{
		running = false;
	}

	if (isPlayerAlive && running)
	{
		if (playerDirectionRequested > 0)
		{
			playerDirection = playerDirectionRequested;
			playerDirectionRequested = -1;

			if (playerCanMove())
			{
				movePlayer(playerPos, playerDirection);
			}

			//player actual moving
			if (!isPlayerAlive)
			{
				std::cerr << "Game Over! (Press 'R' to restart)" << std::endl;
				audioManager->playDeathSound();
			}
		}
	}
}

int GameLogic::renderGame(void)
{
	if (dlIndex < 0 || dlIndex > 1)
	{
		std::cerr << "Wrong number given.." << std::endl;
		return EXIT_FAILURE;
	}

	// Draw window with game infos
	if (graphicLib && !hasShownDeath)
	{
		graphicLib->refreshWindow();
		if (!isPlayerAlive)
			hasShownDeath = true;
	}

	return EXIT_SUCCESS;
}

void GameLogic::initPlayer(void)
{
	isPlayerAlive = true;
	hasShownDeath = false;

	playerPos = std::tuple<int, int>();

	playerPos = std::make_tuple(mapW / 2, mapH / 2);
	playerDirection = (mapW > mapH) ? LEFT : DOWN;
	playerDirectionRequested = -1;
}

bool GameLogic::playerCanMove(void)
{
	// Check for player
	int headX = std::get<0>(playerPos);
	int headY = std::get<1>(playerPos);
	if (playerDirection == UP)
		headY -= 1;
	else if (playerDirection == DOWN)
		headY += 1;
	else if (playerDirection == LEFT)
		headX -= 1;
	else if (playerDirection == RIGHT)
		headX += 1;

	return true;
}

void GameLogic::movePlayer(std::tuple<int, int> &playerPos, int &playerDir)
{
	// Advance based on direction
	if (playerDir == UP)
		std::get<1>(playerPos) = std::get<1>(playerPos) - 1;
	else if (playerDir == DOWN)
		std::get<1>(playerPos) = std::get<1>(playerPos) + 1;
	else if (playerDir == LEFT)
		std::get<0>(playerPos) = std::get<0>(playerPos) - 1;
	else if (playerDir == RIGHT)
		std::get<0>(playerPos) = std::get<0>(playerPos) + 1;
}

void GameLogic::changeDirectionTo(int &playerDirection, int &playerDirectionRequested, int newDir)
{
	playerDirectionRequested = newDir;
	if ((newDir == UP || newDir == DOWN) && (playerDirection == LEFT || playerDirection == RIGHT))
	{
		playerDirectionRequested = newDir;
	}
	else if ((newDir == LEFT || newDir == RIGHT) && (playerDirection == UP || playerDirection == DOWN))
	{
		playerDirectionRequested = newDir;
	}
}

int GameLogic::run(void)
{
	if (!canRun)
		return EXIT_FAILURE;
	int guiRet;

	// init vars
	running = true;
	restartRequest = false;
	dlIndex = 1;
	dlPastIndex = -1;
	timer = time(NULL);

	initPlayer();

	audioManager->playStartSound();

	// Start game loop
	while (running)
	{
		// Get delta time in order to synch entities positions
		pastFrameLength = difftime(timer, time(NULL));
		timer = time(NULL);

		// Update inputs
		graphicLib->getUserInput();

		// TODO: Update game engine statuses (ex. when to quit)

		// Update game entities states
		// for (auto entity : entities)
		// {
		// 	entity->Update();
		// }

		// TODO: set position of entities back to prev frame when they collide

		updateGameState();

		guiRet = renderGame();
		if (guiRet != EXIT_SUCCESS || !running)
			break;
	}
	if (restartRequest)
	{
		std::cout << "Starting new game!" << std::endl;
		return run();
	}
	return guiRet;
}

void GameLogic::buttonStateChanged(std::string buttonName, bool isPressed)
{
	// std::map<std::string, bool> mapKey;
	// mapKey[buttonName] = isPressed;
	// std::string key = !button ? "NULL" : std::string(button); // GLFW sends NULL pointer for Escape key..
	std::cout << "key '" << buttonName << "' new status: " << isPressed << std::endl;
}

void GameLogic::buttonPressed(const char *button)
{
	std::string key = !button ? KEY_ESCAPE : std::string(button); // GLFW sends NULL pointer for Escape key..

	// std::cout << "key '" << key << "' was pressed" << std::endl;
	std::list<std::string>::const_iterator iter = std::find(changeLibraryKeys.begin(), changeLibraryKeys.end(), key);
	if (iter != changeLibraryKeys.end())
	{
		changeLibraryRequest(key);
	}
	else
	{
		for (const std::tuple<std::string, int> &changeDirectionPair : changeDirectionKeys) // access by reference to avoid copying
		{
			if (std::get<0>(changeDirectionPair).compare(key) == 0)
			{
				changeDirectionTo(playerDirection, playerDirectionRequested, std::get<1>(changeDirectionPair));
				return;
			}
		}
		if (!isPlayerAlive)
		{
			if (key.compare("R") == 0 || key.compare("r") == 0)
			{
				changeLibraryRequest("0");
				restartRequest = true;
			}
		}
		// std::cout << "value not useful.." << std::endl;
	}
}

static std::list<std::string> generateLibraryKeys()
{ // static here is "internal linkage"
	std::list<std::string> p;
	p.push_front(KEY_0);
	return p;
}
const std::list<std::string> GameLogic::changeLibraryKeys = generateLibraryKeys();

static std::vector<std::tuple<std::string, int>> generateDirectionKeys()
{ // static here is "internal linkage"
	std::vector<std::tuple<std::string, int>> p;
	p.push_back(std::make_tuple(KEY_W, UP));
	p.push_back(std::make_tuple(KEY_A, LEFT));
	p.push_back(std::make_tuple(KEY_S, DOWN));
	p.push_back(std::make_tuple(KEY_D, RIGHT));
	p.push_back(std::make_tuple(KEY_W_LOWER, UP));
	p.push_back(std::make_tuple(KEY_A_LOWER, LEFT));
	p.push_back(std::make_tuple(KEY_S_LOWER, DOWN));
	p.push_back(std::make_tuple(KEY_D_LOWER, RIGHT));
	return p;
}
const std::vector<std::tuple<std::string, int>> GameLogic::changeDirectionKeys = generateDirectionKeys();