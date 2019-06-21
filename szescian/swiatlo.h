#pragma once
#include <iostream>
#include <gl\glut.h>             // GLU library
#include "menu.h"

class swiatlo
{
	// Light values and coordinates
	GLfloat  ambientLight[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat  diffuseLight[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat  specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat	 lightPos[4] = { 100.0f, 100.0f, 100.0f, 1.0f };
	GLfloat  specref[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	swiatlo();
	~swiatlo();

	void ustaw(menu *anttweak);
};

