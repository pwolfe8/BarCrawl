#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class Boundary
{
public:
// constructor/destructors
	Boundary();
    Boundary (const Boundary &copy); // copy constructor
	~Boundary();


	// Overloaded assignment operator
	Boundary& operator= (const Boundary &rhs);


// member functions
    void init(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
	void move(GLfloat amount, Direction dir);
    void setPosition(GLfloat x, GLfloat y);

// member variables

    // position of upper left corner
    GLfloat x;
    GLfloat y;

	// size of boundary
	GLfloat width;
	GLfloat height;

	// top, bottom, left, right values of boundary
	GLfloat left;
	GLfloat right;
	GLfloat top;
	GLfloat bottom;
};

