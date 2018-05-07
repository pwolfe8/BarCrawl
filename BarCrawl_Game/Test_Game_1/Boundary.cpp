#include "Boundary.h"


// constructor/destructor
Boundary::Boundary() {}

Boundary::Boundary (const Boundary &copy) {
    this->init(copy.x, copy.y, copy.width, copy.height);
};

Boundary::~Boundary() {
    
}



// assignment operator
Boundary& Boundary::operator= (const Boundary &rhs) {
    this->init(rhs.x, rhs.y, rhs.width, rhs.height);
	return *this;
}


// member functions
void Boundary::init( GLfloat x, GLfloat y, GLfloat width, GLfloat height ) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    
    // calculate bounding box based off position & width/height
	top = y;
	left = x;
	bottom = top + height;
	right = left + width;
}

void Boundary::move(GLfloat amount, Direction dir) {
    // move by an amount in a direction
	switch (dir) {
        case UP:
            y -= amount;
            break;    
        case DOWN:
            y += amount;
            break;
        case LEFT:
            x -= amount;
            break;
        case RIGHT:
            x += amount;
            break;
	}

    // recalculate bounding box based off new position
    top = y;
	left = x;
	bottom = top + height;
	right = left + width;
}

void Boundary::setPosition(GLfloat x, GLfloat y) {
    x = x;
    y = y;
    
    // recalculate bounding box based off new position
    top = y;
	left = x;
	bottom = top + height;
	right = left + width;
}