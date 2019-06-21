#pragma once
#include <windows.h>            // Window defines
#include "AntTweakBar.h"
#include <GL/glut.h>
#include <iostream>
#include <string>
class menu
{

	TwBar *myBar;
	static bool obiekty[]; //obiekty wyœwietlane
	bool tryb_lotu;
	

public:
	float mat[4 * 4]; // rotation matrix
	char model = 's';
	float v[4];
	float g_LightDirection[3] = { -0.57735f, -0.57735f, -0.57735f };
	float g_LightMultiplier = 1.0f;
	float xRot = 25, yRot = 35;
	bool swiatlo = 0;
	float jasnosc = 0.5;
	// Shapes scale
	typedef enum { SHAPE_TEAPOT=1, SHAPE_TORUS, SHAPE_CONE } Shape;
#define NUM_SHAPES 3
	Shape g_CurrentShape = SHAPE_TORUS;
	float g_Zoom = 1.0f;
	// Shape orientation (stored as a quaternion)
	GLfloat g_Rotation[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	void ConvertQuaternionToMatrix(const float *quat, float *mat);
	void SetQuaternionFromAxisAngle(const float *axis, float angle, float *quat);
	float axis[3] = { 0.7f, 0.7f, 0.0f }; // initial model rotation
	float angle = 0.8f;
	GLfloat params[7] = { 10.0f,10.0f,10.0f,10.0f,60.0f, 1.0f, 0.9f };
	int points = 15;
	bool start = false;
	bool reset = false;
	// Auto rotate
	int g_AutoRotate = 0;
	int g_RotateTime = 0;
	float g_RotateStart[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	menu();
	~menu();
	void TW_CALL SetAutoRotateCB(const void *value, void *clientData);
	void rysuj();
	static bool* obiekty_wyswieltane();
};