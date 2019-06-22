#pragma once
#include <windows.h>            // Window defines
#include "AntTweakBar.h"
#include <GL/glut.h>
#include <iostream>
#include <string>
class menu
{


	TwBar *myBar;
	bool tryb_lotu;
	static bool obiekty[]; //obiekty wyœwietlane

public:
	bool startRotate = false;
	float mat[4 * 4]; // rotation matrix
	float v[4];
	float xRot, yRot;
	float g_LightDirection[3] = { -0.57735f, -0.57735f, -0.57735f };
	float g_LightMultiplier = 1.0f;
	bool swiatlo = 0;
	float jasnosc = 0.5;
	float g_Zoom = 1.0f;
	// Shape orientation (stored as a quaternion)
	GLfloat g_Rotation[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	void ConvertQuaternionToMatrix(const float *quat, float *mat);
	void SetQuaternionFromAxisAngle(const float *axis, float angle, float *quat);
	float axis[3] = { 0.7f, 0.7f, 0.0f }; // initial model rotation
	float angle = 0.8f;
	GLfloat params[7] = { 10.0f,34.0f,90.0f,10.0f,47.0f, 1.5f, 0.9f };
	int points = 70;
	bool start = false;
	bool reset = false;
	menu();
	~menu();
	void rysuj();
};