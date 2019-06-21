#include "menu.h"

bool menu::obiekty[] = { 1, 1, 1, 1 }; //drzewa, dom, gleba, droga;



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
	
	myBar = TwNewBar("Zmiana parametr�w");

	TwWindowSize(400, 700);
	
	TwAddSeparator(myBar, NULL, "label = '' ");
	 // Add 'g_Zoom' to 'bar': this is a modifable (RW) variable of type TW_TYPE_FLOAT. Its key shortcuts are [z] and [Z].
    TwAddVarRW(myBar, "Zoom", TW_TYPE_FLOAT, &g_Zoom,
               " min=0.01 max=2.5 step=0.01 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' ");

	TwAddSeparator(myBar, NULL, "label = '' ");
    // Add 'g_Rotation' to 'bar': this is a variable of type TW_TYPE_QUAT4F which defines the object's orientation
    TwAddVarRW(myBar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation,
               " label='Object rotation' opened=true help='Change the object orientation.' ");

	//if (model == 's')
	//{
		TwAddSeparator(myBar, NULL, "label = '' ");
		TwAddButton(myBar, "Punkt czerwony", NULL, NULL, NULL);
		TwAddVarRW(myBar, "Lewo/Prawo 1", TW_TYPE_FLOAT, &params[0], "min=-100.0 max=100.0 step=1");
		TwAddVarRW(myBar, "G�ra/d�l 1", TW_TYPE_FLOAT, &params[1], "min=-48.0 max=100.0 step=1");
		TwAddVarRW(myBar, "Prz�d/tyl 1", TW_TYPE_FLOAT, &params[2], "min=-100.0 max=100.0 step=1");

		TwAddSeparator(myBar, NULL, "label = '' ");
		TwAddButton(myBar, "Punkt niebieski", NULL, NULL, NULL);
		TwAddVarRW(myBar, "Lewo/Prawo 2", TW_TYPE_FLOAT, &params[3], "min=-100.0 max=100.0 step=1");
		TwAddVarRW(myBar, "G�ra/d�l 2", TW_TYPE_FLOAT, &params[4], "min=-48.0 max=100.0 step=1");
		TwAddVarRW(myBar, "Prz�d/tyl 2", TW_TYPE_FLOAT, &params[5], "min=-100.0 max=100.0 step=1");
	//}
	//else {

	//}

	TwAddSeparator(myBar, NULL, "label = '' ");
	TwAddButton(myBar, "OSWIETLENIE", NULL, NULL, NULL);
	/*TwAddVarRW(myBar, "Wlaczone", TW_TYPE_BOOLCPP, &swiatlo, "");
	TwAddVarRW(myBar, "Jasnosc", TW_TYPE_FLOAT, &jasnosc, "min=0.0 max=1.0 step=0.1");// keyIncr=w keyDecr=s");
	TwAddSeparator(myBar, NULL, "label = '' ");*/
	TwAddVarRW(myBar, "LightDir", TW_TYPE_DIR3F, &g_LightDirection,
		" label='Light direction' opened=true help='Change the light direction.' ");
	// Add 'g_LightMultiplier' to 'bar': this is a variable of type TW_TYPE_FLOAT. Its key shortcuts are [+] and [-].
	TwAddVarRW(myBar, "Multiplier", TW_TYPE_FLOAT, &g_LightMultiplier,
		" label='Light booster' min=0.1 max=4 step=0.02 keyIncr='+' keyDecr='-' help='Increase/decrease the light power.' ");

	TwAddSeparator(myBar, NULL, "label = '' ");
	TwAddButton(myBar, "Ilosc punkt�w", NULL, NULL, NULL);
	TwAddVarRW(myBar, "Punkty", TW_TYPE_INT32, &points, "min=1 max=70 step=1");// keyIncr=w keyDecr=s");

	TwAddSeparator(myBar, NULL, "label = '' ");
	TwAddVarRW(myBar, "Start", TW_TYPE_BOOL32, &start,
		" label='Start?' true='+' false='-' help='Start animation.' ");
	TwAddSeparator(myBar, NULL, "label = '' ");
	TwAddVarRW(myBar, "Reset", TW_TYPE_BOOL32, &reset,
		" label='Reset?' true='+' false='-' help='Reset animation.' ");

	SetQuaternionFromAxisAngle(axis, angle, g_Rotation);

}


menu::~menu()
{
}

void menu::rysuj()
{
	TwDraw();  // draw the tweak bar
}

bool* menu::obiekty_wyswieltane()
{
	return obiekty;
}