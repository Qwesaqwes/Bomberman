#pragma once

#include "header.hpp"

class Shape {
   public:
	Shape(std::string const &objPath);
	virtual ~Shape(void);

	size_t getSize(void) const;
	GLuint getVAO(void) const;
	GLuint getVBO(void) const;


   private:
	Shape(void);
	Shape(Shape const &src);

	Shape &operator=(Shape const &rhs);

	size_t _size;

	GLuint _VAO;
	GLuint _VBO;
	GLuint _EBO;
};
