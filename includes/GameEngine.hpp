#pragma once

#include "AGameScene.hpp"
#include "AudioManager.hpp"
#include "Camera.hpp"
#include "Collider.hpp"

#define MAP_SIZE 40

#define WINDOW_MIN_X_OFFSET 50
#define WINDOW_MIN_Y_OFFSET 100
#define MIN_SQUARE_SIZE 10
#define MAX_SQUARE_SIZE 25
#define OUTLINE_TICKNESS 2

#define KEY_W "W"
#define KEY_A "A"
#define KEY_S "S"
#define KEY_D "D"
#define KEY_LEFT "LEFT"
#define KEY_UP "UP"
#define KEY_RIGHT "RIGHT"
#define KEY_DOWN "DOWN"
#define KEY_ESCAPE "ESCAPE"

typedef std::chrono::high_resolution_clock Clock;

class GameRenderer;

class GameEngine {
   public:
	GameEngine(std::vector<AGameScene *> gameScenes);
	~GameEngine(void);

	void run();

	// Functions needed by Renderer
	int getSquareSize(void);
	int getXOffset(void);
	int getYOffset(void);
	int getMapW(void);
	int getMapH(void);
	GameRenderer const *getGameRenderer(void) const;
	Entity *getFirstEntityWithName(std::string entityName);
	// std::vector<Entity *> getEntitiesWithName(std::string entityName);
	// Entity *getFirstEntityWithLabel(std::string entityLabel);
	// std::vector<Entity *> getEntitiesWithLabel(std::string entityLabel);
	void buttonStateChanged(std::string button, bool isPressed);

	// Functions needed by entities
	bool isKeyPressed(std::string keyName);
	float getDeltaTime();

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
	static std::map<std::string, bool> keyboardMap;

	GameEngine(void);
	GameEngine(GameEngine const &src);

	GameEngine &operator=(GameEngine const &rhs);

	bool initScene(int newSceneIdx);
	void _clearTmpEntities(void);
	void moveEntities(void);
	size_t checkCollision(Entity *entity, glm::vec3 &futureMovement,
						  std::vector<Entity *> &collidedEntities,
						  std::vector<Entity *> &collidedTriggers);
	void getMovementLines(Entity *entity, glm::vec3 &targetMovement,
						  LineInfo *lineA, LineInfo *lineB);
	bool hasCollisionCourse(LineInfo &lineA, LineInfo &lineB, Entity *entityB);
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
	int squareSize;
	int xOffset;
	int yOffset;
	Clock::time_point _frameTs;
	Clock::time_point _lastFrameTs;
	double _deltaTime;
	AudioManager *_audioManager;

	// Game model vars
	bool _running;
	int mapH;
	int mapW;
	bool restartRequest;

	// Scene management vars
	int _sceneIdx;
	std::vector<AGameScene *> _gameScenes;
	std::vector<Entity *> _allEntities;
	Camera *_camera;
};
