#pragma once

#include "engine/Engine.hpp"
#include "engine/ShaderProgram.hpp"

struct Material {
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	bool isTextured;
	float shininess;
};

class Mesh {
   public:
	Mesh(std::vector<Vertex> const &vertices, Material const &material,
		 GLuint diffuseTexture);
	virtual ~Mesh(void);

	size_t getSize(void) const;
	void draw(ShaderProgram const &shaderProgram) const;

	GLuint VAO;
	GLuint VBO;

   private:
	size_t _size;
	Material const _material;
	GLuint const _diffuseTexture;

	Mesh(void);
	Mesh(Mesh const &src);

	Mesh &operator=(Mesh const &rhs);
};