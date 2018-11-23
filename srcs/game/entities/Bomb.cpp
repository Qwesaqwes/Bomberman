#include "game/entities/Bomb.hpp"
#include "engine/GameEngine.hpp"
#include "game/cams/Tools.hpp"

Bomb::Bomb(glm::vec3 position, float timer, size_t range, Entity *sceneManager)
	: Entity(glm::vec3(position.x, position.y + 0.3f, position.z), glm::vec3(0),
			 nullptr, "Bomb", "Bomb", "Bomb", sceneManager),
	  _timer(timer),
	  _range(range) {}

Bomb::~Bomb(void) {}

void Bomb::update(void) {
	float deltaTime = _gameEngine->getDeltaTime();

	_timer -= deltaTime;
	(void)_range;

	if (_timer <= 0.0f) {
		// Tools *cam = dynamic_cast<Tools *>(_sceneManager);

		// if (cam == nullptr) {
		// 	cam->putExplosion(getPosition().x, getPosition().z, _range);
		// }
		_needToBeDestroyed = true;
	}
}
