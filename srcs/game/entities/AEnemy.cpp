#include "game/entities/AEnemy.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"

AEnemy::AEnemy(glm::vec3 position, glm::vec3 eulerAngles, std::string name,
	LayerTag tag, Entity *sceneManager)
	: Damageable(
		  glm::vec3(position.x, position.y + 0.4f, position.z), eulerAngles,
		  new Collider(Collider::Circle, LayerTag::EnemyLayer, 0.45f, 0.45f),
		  "Enemy", name, "Enemy", 1, tag, EnemySpecialLayer, 2.0f,
		  sceneManager) {}

AEnemy::~AEnemy(void) {}

void AEnemy::findBestWay(SceneTools *cam, bool runAway) {
	if (cam->getRefreshAI()) {
		size_t mapWidth = cam->getMapWidth();
		size_t mapHeight = cam->getMapHeight();
		float x = this->getPosition().x + (static_cast<float>(mapWidth) / 2);
		float z = this->getPosition().z + (static_cast<float>(mapHeight) / 2);
		size_t bestDist = std::numeric_limits<std::size_t>::max();
		_way.clear();
		if (!runAway) {
			size_t xPlayer = cam->getPlayerPos() % mapWidth;
			size_t zPlayer = cam->getPlayerPos() / mapHeight;
			size_t pos = static_cast<int>(z) * mapHeight + static_cast<int>(x);
			if (cam->getGraphe().find(pos) != cam->getGraphe().end()) {
				Node currentPos = *cam->getGraphe().at(pos);
				while (1) {
					if (currentPos.prevNodesByDist.size() == 0) return ;
					if (xPlayer == currentPos.x && zPlayer == currentPos.z)
						break;
					for (const auto &n : currentPos.prevNodesByDist) {
						if (bestDist > n.first)
							bestDist = n.first;
					}
					if (bestDist != std::numeric_limits<std::size_t>::max())
						currentPos = *currentPos.prevNodesByDist[bestDist][0];
					_way.push_back(currentPos.z * mapHeight + currentPos.x);
				}
			}
		} else {
			size_t xPlayer = cam->getRunAwayPos() % mapWidth;
			size_t zPlayer = cam->getRunAwayPos() / mapHeight;
			size_t pos = static_cast<int>(z) * mapHeight + static_cast<int>(x);
			if (cam->getGraphe().find(pos) != cam->getGraphe().end()) {
				Node currentPos = *cam->getGraphe().at(pos);
				while (1) {
					if (currentPos.runAwayNodesByDist.size() == 0) return ;
					if (xPlayer == currentPos.x && zPlayer == currentPos.z)
						break;
					for (const auto &n : currentPos.runAwayNodesByDist) {
						if (bestDist > n.first)
							bestDist = n.first;
					}
					if (bestDist != std::numeric_limits<std::size_t>::max()) {
						size_t playerDist = 0;
						size_t idx = 0;
						size_t saveIdx = 0;
						for (const auto &v : currentPos.runAwayNodesByDist[bestDist]) {
							if (v->dist > playerDist) {
								playerDist = v->dist;
								saveIdx = idx;
							}
							idx++;
						}
						if (idx == 0)
							break ;
						currentPos = *currentPos.runAwayNodesByDist[bestDist][saveIdx];
					}
					_way.push_back(currentPos.z * mapHeight + currentPos.x);
				}
			}
		}
	}
}

void AEnemy::walk(SceneTools *cam) {
	if (_way.size() != 0) {
		size_t mapWidth = cam->getMapWidth();
		size_t mapHeight = cam->getMapHeight();
		float x = this->getPosition().x + (static_cast<float>(mapWidth) / 2);
		float z = this->getPosition().z + (static_cast<float>(mapHeight) / 2);
		float targetX = (static_cast<int>(_way[0]) % mapWidth) + 0.5f;
		float targetZ = static_cast<int>(_way[0] / mapHeight) + 0.5f;
		int xSign = 0;
		int zSign = 0;
		if (targetX - x < -0.05f)
			xSign = -1;
		else if (targetX - x > 0.05f)
			xSign = 1;
		if (targetZ - z < -0.05f)
			zSign = -1;
		else if (targetZ - z > 0.05f)
			zSign = 1;
		if (x - targetX <= 0.05 && z - targetZ <= 0.05 &&
			x - targetX >= -0.05 && z - targetZ >= -0.05) {
			_targetMovement *= 0;
			_way.erase(_way.begin());
		} else if (xSign != 0 || zSign != 0) {
			float xDirection = static_cast<float>(xSign);
			float zDirection = static_cast<float>(zSign);
			xSign = abs(xSign);
			zSign = abs(zSign);
			float totalMagnitude = xSign + zSign;
			xDirection *= sqrt(xSign / totalMagnitude);
			zDirection *= sqrt(zSign / totalMagnitude);
			float deltaTime = _gameEngine->getDeltaTime();
			_targetMovement.x = xDirection * _speed * deltaTime;
			_targetMovement.z = zDirection * _speed * deltaTime;
		}
	}
}
