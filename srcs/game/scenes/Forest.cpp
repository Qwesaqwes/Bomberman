#include "game/scenes/Forest.hpp"
#include "engine/GameEngine.hpp"

Forest::Forest(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	:  // Camera(pos, eulerAngles),
	  SceneTools(17, 17, pos, eulerAngles) {}

Forest::~Forest(void) {}

void Forest::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImage(_neededImages);

	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
	_refreshAI = false;
	_pauseMenu = false;
}

void Forest::drawGUI(GUI *graphicUI) {
	if (_pauseMenu || _gameEngine->isKeyPressed("E"))
		_pauseMenu = _displayPauseMenu(graphicUI, &_newSceneIdx, 1, 0);

	// static int searchWord = 0;
	// static int lastWord = 0;
	// static int startStrIdx = 0;
	// std::string str =
	// 	"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
	// 	"officia deserunt mollit anim id est laborum.";
	// _displayDialogue(graphicUI, &searchWord, &lastWord, &startStrIdx,
	// 				 "Bomberman", "", str, false, 1000, 1000,
	// 				 NK_TEXT_LEFT, "12_BOMBERMAN", "18_BOMBERMAN");
}

void Forest::tellPosition(Entity *entity) { _savePositions(entity); }

void Forest::update(void) {
	Camera::update();
	_refreshAI = false;
	static float _cooldown = 0.5f;
	if (_cooldown <= 0.0f) {
		_cooldown = 0.5f;
		_startBuildingGrapheForPathFinding();
		_refreshAI = true;
	}
	_cooldown -= _gameEngine->getDeltaTime();
}
