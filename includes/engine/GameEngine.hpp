#pragma once

#include "engine/AGame.hpp"
#include "engine/AudioManager.hpp"
#include "engine/Camera.hpp"
#include "engine/Collider.hpp"

#define KEY_W "W"
#define KEY_A "A"
#define KEY_S "S"
#define KEY_D "D"
#define KEY_C "C"
#define KEY_SPACE "SPACE"
#define KEY_LEFT "LEFT"
#define KEY_UP "UP"
#define KEY_RIGHT "RIGHT"
#define KEY_DOWN "DOWN"
#define KEY_ESCAPE "ESCAPE"

#define EPSILON 0.01f

typedef std::chrono::high_resolution_clock Clock;

struct KeyState {
	bool currFrame;
	bool prevFrame;
};

class GameRenderer;

class GameEngine {
   public:
	GameEngine(AGame *game);
	~GameEngine(void);

	void run();

	// Functions needed by Renderer
	GameRenderer const *getGameRenderer(void) const;
	// Entity *getFirstEntityWithName(std::string entityName);
	// std::vector<Entity *> getEntitiesWithName(std::string entityName);
	// Entity *getFirstEntityWithLabel(std::string entityLabel);
	// std::vector<Entity *> getEntitiesWithLabel(std::string entityLabel);
	void buttonStateChanged(std::string button, bool isPressed);

	// Functions needed by entities
	bool isKeyPressed(std::string keyName);
	bool isKeyJustPressed(std::string keyName);
	float getDeltaTime();
	void addNewEntity(Entity *entity);

   private:
	struct LineInfo {  // Equation of a line: z = mx + q
	   public:
		LineInfo(void);
		LineInfo(float startX, float startZ, float endX, float endZ);

		float m;
		float q;
		bool isVertical;  // if line is vertical then m = inf
		float startX;
		float startZ;
		float endX;
		float endZ;
	};

	struct RectanglePoints {
	   public:
		float top;
		float bot;
		float left;
		float right;
		RectanglePoints(Entity *entity, glm::vec3 movement = glm::vec3(0.0f));
	};
	static std::map<std::string, KeyState> keyboardMap;

	GameEngine(void);
	GameEngine(GameEngine const &src);

	GameEngine &operator=(GameEngine const &rhs);

	bool initScene(size_t newSceneIdx);
	void moveEntities(void);
	size_t checkCollision(Entity *entity, glm::vec3 &futureMovement,
						  std::vector<Entity *> &collidedEntities,
						  std::vector<Entity *> &collidedTriggers);
	void getMovementLines(Entity *entity, glm::vec3 &targetMovement,
						  LineInfo *lineA, LineInfo *lineB);
	bool hasCollisionCourse(LineInfo &lineA, LineInfo &lineB, int layerTag,
							Entity *entityB);
	bool isLineLineCollision(LineInfo &lineA, LineInfo &lineB);
	bool isLineCircleCollision(LineInfo &lineA, float &xSquareCoeff,
							   float &xCoeff, float &zSquareCoeff,
							   float &zCoeff, float &cCoeff);
	bool doCollide(const Collider *colliderA, const glm::vec3 &posA,
				   Entity *entityB) const;
	bool collisionCircleRectangle(const Collider *circleCollider,
								  const glm::vec3 &circlePos,
								  const Collider *rectangleCollider,
								  const glm::vec3 &rectanglePos) const;

	// Graphic libraries vars
	GameRenderer *_gameRenderer;
	Clock::time_point _frameTs;
	Clock::time_point _lastFrameTs;
	double _deltaTime;
	AudioManager *_audioManager;

	// Game model vars
	bool _running;
	bool restartRequest;

	// Scene management vars
	int _sceneIdx;
	AGame *_game;
	Camera *_camera;
	std::vector<Entity *> _allEntities;
	std::vector<Entity *> _newEntities;
	std::map<size_t, std::vector<size_t>> _initialCollisionMap;
	std::vector<std::vector<bool>> const &_collisionTable;
};