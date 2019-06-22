#include "menu.h"

bool menu::obiekty[] = { 0, 0 }; 


void menu::ConvertQuaternionToMatrix(const float * quat, float * mat)
{
	float yy2 = 2.0f * quat[1] * quat[1];
	float xy2 = 2.0f * quat[0] * quat[1];
	float xz2 = 2.0f * quat[0] * quat[2];
	float yz2 = 2.0f * quat[1] * quat[2];
	float zz2 = 2.0f * quat[2] * quat[2];
	float wz2 = 2.0f * quat[3] * quat[2];
	float wy2 = 2.0f * quat[3] * quat[1];
	float wx2 = 2.0f * quat[3] * quat[0];
	float xx2 = 2.0f * quat[0] * quat[0];
	mat[0 * 4 + 0] = -yy2 - zz2 + 1.0f;
	mat[0 * 4 + 1] = xy2 + wz2;
	mat[0 * 4 + 2] = xz2 - wy2;
	mat[0 * 4 + 3] = 0;
	mat[1 * 4 + 0] = xy2 - wz2;
	mat[1 * 4 + 1] = -xx2 - zz2 + 1.0f;
	mat[1 * 4 + 2] = yz2 + wx2;
	mat[1 * 4 + 3] = 0;
	mat[2 * 4 + 0] = xz2 + wy2;
	mat[2 * 4 + 1] = yz2 - wx2;
	mat[2 * 4 + 2] = -xx2 - yy2 + 1.0f;
	mat[2 * 4 + 3] = 0;
	mat[3 * 4 + 0] = mat[3 * 4 + 1] = mat[3 * 4 + 2] = 0;
	mat[3 * 4 + 3] = 1;
}

void menu::SetQuaternionFromAxisAngle(const float * axis, float angle, float * quat)
{
		float sina2, norm;
		sina2 = (float)sin(0.5f * angle);
		norm = (float)sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
		quat[0] = sina2 * axis[0] / norm;
		quat[1] = sina2 * axis[1] / norm;
		quat[2] = sina2 * axis[2] / norm;
		quat[3] = (float)cos(0.5f * angle);
}

menu::menu()
{
	TwInit(TW_OPENGL, NULL);
	
	myBar = TwNewBar("PARAMETERS");
	TwDefine(" TweakBar size='200 400' color='96 216 224' "); 
	TwWindowSize(1000, 1000);
	
	TwAddSeparator(myBar, NULL, "label = '' ");
    TwAddVarRW(myBar, "Zoom", TW_TYPE_FLOAT, &g_Zoom,
               " min=0.01 max=2.5 step=0.01 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' ");

	TwAddSeparator(myBar, NULL, "label = '' ");
    TwAddVarRW(myBar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation,
               " label='Object rotation' opened=true help='Change the object orientation.' ");

	TwAddSeparator(myBar, NULL, "label = '' ");
	TwAddButton(myBar, "ANIMATION", NULL, NULL, NULL);
	TwAddVarRW(myBar, "Start", TW_TYPE_BOOL32, &start,
		" label='START' ");
	TwAddVarRW(myBar, "Reset", TW_TYPE_BOOL32, &reset,
		" label='RESET'");

		TwAddSeparator(myBar, NULL, "label = '' ");
		TwAddButton(myBar, "PARAMETERS OF MODEL", NULL, NULL, NULL);
		TwAddVarRW(myBar, "Points", TW_TYPE_INT32, &points, "min=3 max=500 step=1");// keyIncr=w keyDecr=s");
		TwAddVarRW(myBar, "Width", TW_TYPE_FLOAT, &params[1], "min=-34.0 max=34.0 step=1");
		TwAddVarRW(myBar, "Hight", TW_TYPE_FLOAT, &params[2], "min=-100.0 max=100.0 step=1");

		TwAddVarRW(myBar, "Frequence", TW_TYPE_FLOAT, &params[4], "min=-180.0 max=180.0 step=1");
		TwAddVarRW(myBar, "Width of line", TW_TYPE_FLOAT, &params[5], "min=0 max=5.0 step=0.1");
	
	TwAddSeparator(myBar, NULL, "label = '' ");
	TwAddButton(myBar, "LIGHTING", NULL, NULL, NULL);
	TwAddVarRW(myBar, "LightDir", TW_TYPE_DIR3F, &g_LightDirection,
		" label='Light direction' opened=true help='Change the light direction.' ");
	TwAddVarRW(myBar, "Multiplier", TW_TYPE_FLOAT, &g_LightMultiplier,
		" label='Light booster' min=0.1 max=4 step=0.02 keyIncr='+' keyDecr='-' help='Increase/decrease the light power.' ");
	
	TwAddSeparator(myBar, NULL, "label = '' ");

	SetQuaternionFromAxisAngle(axis, angle, g_Rotation);
}


menu::~menu()
{
}

void menu::rysuj()
{
	TwDraw();  // draw the tweak bar
}
