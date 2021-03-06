#pragma once

#include "engine/Engine.hpp"
#include "engine/Entity.hpp"
#include "engine/GUI/GUI.hpp"

class Camera : public Entity {
   public:
	Camera(glm::vec3 const &pos, glm::vec3 const &eulerAngles);
	virtual ~Camera(void);

	glm::mat4 const &getViewMatrix(void) const;
	glm::mat4 const &getProjectionMatrix(void) const;
	int getNewSceneIdx(void) const;
	std::string getNewSceneName(void) const;
	std::vector<std::tuple<std::string, std::string, bool>> const &getNeededImages()
		const;
	bool isGameRunning(void) const;

	// Redefinitions of Entity func
	virtual void initEntity(GameEngine *gameEngine);
	virtual void update(void);
	virtual void translate(glm::vec3 translation);
	// Camera specific virtual func
	virtual bool isPause(void) const;
	virtual void drawGUI(GUI *graphicUI);
	virtual void updateDebugMode(void);
	virtual void configGUI(GUI *graphicUI);

	void mouseCallback(GLFWwindow *window, double xpos, double ypos);

   protected:
	int _newSceneIdx;
	bool _isRunning = false;
	bool _isPause = false;
	bool _debugMode = false;
	std::string _newSceneName;
	std::map<int, nk_color> defaultStyle;
	std::map<int, nk_color> activeStyle;
	std::vector<std::tuple<std::string, std::string, bool>> _neededImages;
	void _updateData(void);
	float _speed;
	float _aspectRatio;
	glm::mat4 _view;
	glm::mat4 _projection;

	glm::vec2 _lastMousePos;
	glm::vec3 _front;
	glm::vec3 _right;
	glm::vec3 _up;

   private:
	Camera(void);
	Camera(Camera const &src);

	Camera &operator=(Camera const &rhs);
};
