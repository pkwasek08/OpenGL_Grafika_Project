#include "swiatlo.h"


swiatlo::swiatlo()
{
}

swiatlo::~swiatlo()
{
}

void swiatlo::ustaw(menu *anttweak)
{
	// Enable lighting
	if (anttweak->swiatlo)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	ambientLight[0] = anttweak->jasnosc;
	ambientLight[1] = anttweak->jasnosc;
	ambientLight[2] = anttweak->jasnosc;

	// Setup and enable light 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
}