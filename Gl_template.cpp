#define  _CRT_SECURE_NO_WARNINGS

#include <AntTweakBar.h>

// Ładowanie bibliotek:

#ifdef _MSC_VER                         // Check if MS Visual C compiler
#  pragma comment(lib, "opengl32.lib")  // Compiler-specific directive to avoid manually configuration
#  pragma comment(lib, "glu32.lib")     // Link libraries
#endif


bool ButtonL;

// Ustalanie trybu tekstowego:
#ifdef _MSC_VER        // Check if MS Visual C compiler
#   ifndef _MBCS
#      define _MBCS    // Uses Multi-byte character set
#   endif
#   ifdef _UNICODE     // Not using Unicode character set
#      undef _UNICODE 
#   endif
#   ifdef UNICODE
#      undef UNICODE 
#   endif
#endif
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>				// Define for sqrt
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include "resource.h"           // About box resource identifiers.
#include "szescian/Header.h"
#include <GL/gl.h> 
#include <GL/glut.h>
#include <GL/Glut.h>
#include "szescian/menu.h"
#include "szescian/swiatlo.h"
#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14
#define _WIN32_WINNT 0x0500
menu menu_anttweak;		//menu AntTweakBar
swiatlo swiatlo1;

HHOOK MouseHook;
// Color Palette handle
HPALETTE hPalette = NULL;

// Application name and instance storeage
static LPCTSTR lpszAppName = "GL Template";
static HINSTANCE hInstance;

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

static GLsizei lastHeight;
static GLsizei lastWidth;
static double hight_model = 10;
static double width_model = 10;
static float freq = 10;
int points_model = 15;
static float size = 3;
static int stop = 0;
static int s1;
static int s2;
static int s3;
static bool start = false;
static bool reset = false;
GLfloat params[7] = { 10.0f,10.0f,10.0f,10.0f,60.0f, 1.0f, 0.9f };
int iterations = 1;
// Opis tekstury
BITMAPINFOHEADER	bitmapInfoHeader;	// nagłówek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury

float color0[] = { 1.0f, 0.5f, 0.0f };

float color1[] = { 0.5f, 1.0f, 0.0f };
LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    message,
	WPARAM  wParam,
	LPARAM  lParam);
void RenderScene(void);
int keyPressed(int key) {
	return (GetAsyncKeyState(key) & 0x8000 != 0);
}
// Zmiana widoku, wywoływana przy zmianie rozmiaru okna
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 100.0f;
	GLfloat fAspect;
	//Zapobiega dzieleniu przez zero
	if (h == 0)		h = 1;

	lastWidth = w;
	lastHeight = h;

	fAspect = (GLfloat)w / (GLfloat)h;

	// Obszar renderingu
	glViewport(0, 0, w, h);
	// Resetuje układ współrzędnych
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Ustawia wilkość wyświetlanej przestrzeni
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange * h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange * w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	//perspektywa
	//gluPerspective(60.0f, fAspect, 1.0, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(20, 30, 20, 00, 00, -0, 0, 1, 0);
}
GLfloat **spiral(int a, int b, int c, int d, int k) {
#define AM 50
#define M_PI 3.141592
	float clr = 0.5f;
	//AM points;
	int i = 0;
	//int t=AM;
	GLfloat **vertices = new GLfloat*[AM];

	float t;
	float step = 1.0f / (float)AM;
	for (t = 0.0f; t < 1.0f && i < AM; t += step, i++)
	{
		vertices[i] = new GLfloat[3];
		//polozenie punktu wzgledem osi x
		vertices[i][0] = cos(2 * M_PI * k * t) * sqrt(d * t) * b;
		//polozenie punktu wzgledem osi y
		vertices[i][1] = sin(2 * M_PI * k * t) * sqrt(d * t) * b;
		//polozenie punktu wzgledem osi z
		vertices[i][2] = (float)(t * c) - 60.0f;
	}
	/*
	glLineWidth(size);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < AM; i++) {
		glVertex3fv(vertices[i]);
		glColor3f(clr, clr / 40.0f, clr / 0.5f);
		//glColor3f(clr, clr / 2.0f, clr / 2.0f);
		clr += step;
	}
	glEnd();
	*/
	return vertices;
}
void spiral_model()
{
	#define AMOUNT  points_model
	GLfloat col_r = 0.0f;
	GLfloat **st_pnts = spiral(params[0], params[1], params[2], params[3], params[4]);
	//czerwony
	GLfloat sa[3] = { st_pnts[0][0], st_pnts[0][1], st_pnts[0][2] };
	GLfloat s[3];
	//niebieski
	GLfloat sb[3] = { st_pnts[19][0], st_pnts[19][1], st_pnts[19][2] };
	point *points = new point[AMOUNT];

	points[0].set_tie_a(st_pnts[0][0], st_pnts[0][1], st_pnts[0][2]);
	points[0].set_start_position(st_pnts[1][0], st_pnts[1][1], st_pnts[1][2]);
	points[AMOUNT-1].set_start_position(st_pnts[18][0], st_pnts[18][1], st_pnts[18][2]);
	points[AMOUNT-1].set_tie_b(st_pnts[19][0], st_pnts[19][1], st_pnts[19][2]);

	for (int i = 1; i < AMOUNT-1; i++) {
		points[i].set_start_position(st_pnts[i][0], st_pnts[i][1], st_pnts[i][2]);
	}
	glLineWidth(4.0f);

	for (int i = 0; i < AMOUNT; i++) {
		delete[] st_pnts[i];
	}
	delete[] st_pnts;

	int iter = iterations;
	for (int i = 0; i < iter; i++) {
		for (int i = 0; i < AMOUNT; i++) {
			if (i < AMOUNT - 1) {
				points[i].set_tie_b(points[i + 1].get_position());
			}
			if (i > 0) {
				points[i].set_tie_a(points[i - 1].get_position());
			}
		}
		for (int i = 0; i < AMOUNT; i++) {
			points[i].update_position();
		}
	}

	float clr = 0.5f;
	float step = 1.0f / (float)AMOUNT;
	glBegin(GL_LINE_STRIP);
	glColor3f(0.8f, 0.2f, 0.1f);
//	glVertex3fv(sa);
	glColor3f(0.9f, 0.9f, 0.9f);
	for (int i = 0; i < AMOUNT; i++) {
		s[0] = *points[i].get_position();
		s[1] = *(points[i].get_position() + 1);
		s[2] = *(points[i].get_position() + 2);
		glColor3f(clr, clr / 30.0f, clr / 0.5f);
		
//		glColor3f(clr, clr / 2.0f, clr / 2.0f);
		glVertex3fv(s);

		clr += step;
	}
	glColor3f(0.2f, 0.1f, 0.8f);
	glVertex3fv(sb);

	glEnd();
	/*
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(0.2f, 0.9f, 0.2f);
	for (int i = 0; i < AMOUNT; i++) {
		s[0] = *points[i].get_position();
		s[1] = *(points[i].get_position() + 1);
		s[2] = *(points[i].get_position() + 2);
		glVertex3fv(s);
	}
	glEnd();
	*/
}
void rope()
{

#define AMOUNT points_model
	GLfloat col_r = 0.0f;

	s1 = float(rand() % 50 - 0);
	s2 = float(rand() % 50 - 0);
	s3 = float(rand() % 50 - 0);

	GLfloat s[3] = { s1, s2, s3 };
	//czerwony
	GLfloat sa[3] = { params[0], params[1], params[2] };
	//niebieski
	GLfloat sb[3] = { params[3], params[4], params[5] };

	point *points = new point[AMOUNT];
	points[0].set_tie_a(params[0], params[1], params[2]);
	points[0].set_start_position(s[0], s[1], s[2]);
	points[AMOUNT - 1].set_tie_b(params[3], params[4], params[5]);
	points[AMOUNT - 1].set_start_position(s[0], s[1], s[2]);
	for (int i = 1; i < AMOUNT - 1; i++) {
		s1 = float(rand() % 50 - 0);
		s2 = float(rand() % 50 - 0);
		s3 = float(rand() % 50 - 0);
		points[i].set_start_position(s1, s2, s3);
	}
	glLineWidth(5.0f);
	glPointSize(1);


	int iter = iterations;
	for (int i = 0; i < iter; i++) {
		for (int j = 0; j < AMOUNT; j++) {
			if (j < AMOUNT - 1) {
				points[j].set_tie_b(points[j + 1].get_position());
			}
			if (j > 0) {
				points[j].set_tie_a(points[j - 1].get_position());
			}
		}
		for (int j = 0; j < AMOUNT; j++) {
			points[j].update_position();
		}
	}

	float clr = 0.5f;
	float step = 1.0f / (float)AMOUNT;
	glBegin(GL_LINE_STRIP);
	glColor3f(0.8f, 0.2f, 0.1f);
	glVertex3fv(sa);
	glColor3f(0.9f, 0.9f, 0.9f);
	for (int i = 0; i < AMOUNT; i++) {
		s[0] = *points[i].get_position();
		s[1] = *(points[i].get_position() + 1);
		s[2] = *(points[i].get_position() + 2);
		glColor3f(clr, clr / 30.0f, clr / 0.5f);
		//glColor3f(clr, clr / 2.0f, clr / 2.0f);
		glVertex3fv(s);

		clr += step;
	}
	glColor3f(0.2f, 0.1f, 0.8f);
	glVertex3fv(sb);

	glEnd();

	glBegin(GL_POINTS);
	glColor3f(0.2f, 0.9f, 0.2f);
	for (int i = 0; i < AMOUNT; i++) {
		s[0] = *points[i].get_position();
		s[1] = *(points[i].get_position() + 1);
		s[2] = *(points[i].get_position() + 2);
		glVertex3fv(s);
	}
	glEnd();
}
void SetupRC()
{
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal

	// wielokaty o kolejnosci krawedzi przeciwnej do ruchu wskazówek zegara sa widoczne od frontu 
	glFrontFace(GL_CCW);		
	
	//Kolor tła
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

}
// Wybiera format pikseli dla danego kontekstu urządzenia
void SetDCPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		1,                                                              // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered
		PFD_TYPE_RGBA,                          // RGBA Color mode
		24,                                     // Want 24bit color 
		0,0,0,0,0,0,                            // Not used to select mode
		0,0,                                    // Not used to select mode
		0,0,0,0,0,                              // Not used to select mode
		32,                                     // Size of depth buffer
		0,                                      // Not used to select mode
		0,                                      // Not used to select mode
		PFD_MAIN_PLANE,                         // Draw in main plane
		0,                                      // Not used to select mode
		0,0,0 };                                // Not used to select mode

												// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}


int APIENTRY WinMain(HINSTANCE       hInst,
	HINSTANCE       hPrevInstance,
	LPSTR           lpCmdLine,
	int                     nCmdShow)
{
	MSG             msg;            // Struktura komunikatu Windows
	WNDCLASS        wc;             // Struktutra klasy Windows
	HWND            hWnd;           // Uchwyt okna

	hInstance = hInst;

	// Register Window style
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	// No need for background brush for OpenGL window
	wc.hbrBackground = NULL;

	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = lpszAppName;

	// Rejestruje klasę okna
	if (RegisterClass(&wc) == 0)
		return FALSE;


	// Tworzy główne okno aplikacji
	hWnd = CreateWindow(
		lpszAppName,
		lpszAppName,

		// OpenGL wymaga WS_CLIPCHILDREN i WS_CLIPSIBLINGS
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,

		// Pozycja i rozmiar okna
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Jeśli okno nie utworzone, zakończ
	if (hWnd == NULL)
		return FALSE;

	////Timer odpowiedzialny za ruch!!!//////
	//SetTimer(hWnd, 1, ruch::delta_t, NULL);
	/////////////////////////////////////////

	// Wyświetla okno
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// Odbiera komunikaty od aplikacji
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    message,
	WPARAM  wParam,
	LPARAM  lParam)
{
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context

	if (TwEventWin(hWnd, message, wParam, lParam)) // send event message to AntTweakBar
		return 0; // event has been handled by AntTweakBar

	switch (message)
	{
		// Window creation, setup for OpenGL
	case WM_CREATE:
		// Store the device context
		hDC = GetDC(hWnd);

		// Select the pixel format
		SetDCPixelFormat(hDC);

		// Create palette if needed
		//hPalette = GetOpenGLPalette(hDC);

		// Create the rendering context and make it current
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		SetupRC();
		
		break;

		// Window is being destroyed, cleanup
	case WM_DESTROY:
		// Deselect the current rendering context and delete it
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);

		// Delete the palette if it was created
		if (hPalette != NULL)
			DeleteObject(hPalette);

		// Tell the application to terminate after the window
		// is gone.
		KillTimer(hWnd, 1);            //-----------------
		PostQuitMessage(0);
		break;

		// Window is resized.
	case WM_SIZE:
		// Call our function which modifies the clipping
		// volume and viewport
		ChangeSize(LOWORD(lParam), HIWORD(lParam));
		break;


		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
	case WM_PAINT:
		if (start == true)
		{
			for (int i = 0; i < 1000; i++)
			{
				RenderScene();
				//rysuj();
				//TwDraw();
				SwapBuffers(hDC);

				// Validate the newly painted client area
				ValidateRect(hWnd, NULL);
				iterations++;
			}
			start = false;
		}
		else
		{
			RenderScene();
			//rysuj();
			//TwDraw();
			SwapBuffers(hDC);

			// Validate the newly painted client area
			ValidateRect(hWnd, NULL);
		}
		break;

		// Windows is telling the application that it may modify
		// the system palette.  This message in essance asks the 
		// application for a new palette.
	case WM_QUERYNEWPALETTE:
		// If the palette was created.
		if (hPalette)
		{
			int nRet;

			// Selects the palette into the current device context
			SelectPalette(hDC, hPalette, FALSE);

			// Map entries from the currently selected palette to
			// the system palette.  The return value is the number 
			// of palette entries modified.
			nRet = RealizePalette(hDC);

			// Repaint, forces remap of palette in current window
			InvalidateRect(hWnd, NULL, FALSE);

			return nRet;
		}
		break;


		// This window may set the palette, even though it is not the 
		// currently active window.
	case WM_PALETTECHANGED:
		// Don't do anything if the palette does not exist, or if
		// this is the window that changed the palette.
		if ((hPalette != NULL) && ((HWND)wParam != hWnd))
		{
			// Select the palette into the device context
			SelectPalette(hDC, hPalette, FALSE);

			// Map entries to system palette
			RealizePalette(hDC);

			// Remap the current colors to the newly realized palette
			UpdateColors(hDC);
			return 0;
		}
		break;

		// Key press, check for arrow keys to do cube rotation.
	case WM_KEYDOWN:
	{
		/*xRot = (const int)xRot % 360;
		yRot = (const int)yRot % 360;*/	
		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;

	/////////obsługa timera, wywolanie funkcji renderujacej////////
	case WM_TIMER:
	//	czas++;
		//czas %= ruch::czas_max;

		// Call OpenGL drawing code
		RenderScene();
		
		SwapBuffers(hDC);

		// Validate the newly painted client area
		ValidateRect(hWnd, NULL);
		break;
	////////////////////////////////////////////////////////////////

		// A menu command
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			// Exit the program
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		}
	}
	break;
	case WM_LBUTTONDOWN:
		/*if (keyPressed(VK_LBUTTON))
		{
			start = true;
		}*/
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_RBUTTONDOWN:
		/*if (keyPressed(VK_RBUTTON))
			iterations = 0;
			*/
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:   // Passes it on if unproccessed

		return (DefWindowProc(hWnd, message, wParam, lParam));
	}

	return (0L);
}
void floor() {
	glLineWidth(2);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	for (GLfloat i = -60.0; i <= 60.0; i += 6.0) {
		glVertex3f(i, -50.0, 60.0); glVertex3f(i, -50.0, -60.0);
		glVertex3f(60.0, -50.0, i); glVertex3f(-60.0, -50.0, i);
	}
	glEnd();
}
void ConvertQuaternionToMatrix(const float * quat, float * mat)
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
void parameters()
{
	params[0] = menu_anttweak.params[0];
	params[1] = menu_anttweak.params[1];
	params[2] = menu_anttweak.params[2];
	params[3] = menu_anttweak.params[3];
	params[4] = menu_anttweak.params[4];
	params[5] = menu_anttweak.params[5];
	if (menu_anttweak.start == true)
	{
		start = menu_anttweak.start;
		menu_anttweak.start = false;
	}
	
	if (menu_anttweak.reset == true)
	{
		reset = menu_anttweak.reset;
		iterations = 0;
		menu_anttweak.reset = false;
	}
	points_model = menu_anttweak.points;

}
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	swiatlo1.ustaw(&menu_anttweak);

	parameters();
	// Save the matrix state and do the rotations
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(menu_anttweak.xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(menu_anttweak.yRot, 0.0f, 1.0f, 0.0f);

	glPopMatrix();

	/////////////////////////////////////////////////////////////////
	// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
	/////////////////////////////////////////////////////////////////

	glPushMatrix();
	ConvertQuaternionToMatrix(menu_anttweak.g_Rotation, menu_anttweak.mat);
	glMultMatrixf(menu_anttweak.mat);
	glScalef(menu_anttweak.g_Zoom, menu_anttweak.g_Zoom, menu_anttweak.g_Zoom);
	glCallList(menu_anttweak.g_CurrentShape);
	srand(5);
	//spiral_model();
	rope();

	floor();
	menu_anttweak.rysuj();
	
	glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);

	// Flush drawing commands
	glFlush();
}

