// Gl_template.c
//Wyłšczanie błędów przed "fopen"
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
#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14
#define _WIN32_WINNT 0x0500

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
static int points = 15;
static float size = 3;
static int stop = 0;
static int s1;
static int s2;
static int s3;
static bool start = false;
GLfloat params[7] = { 20.0f,60.0f,0.0f,-20.0f,60.0f, 0.0f, 0.9f};
int iterations = 1;
// Opis tekstury
BITMAPINFOHEADER	bitmapInfoHeader;	// nagłówek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury

float color0[] = { 1.0f, 0.5f, 0.0f };

float color1[] = { 0.5f, 1.0f, 0.0f };
// Declaration for Window procedure
LRESULT CALLBACK WndProc(   HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam);

// Dialog procedure for about box
BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam);

// Set Pixel Format function - forward declaration
void SetDCPixelFormat(HDC hDC);

// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
int keyPressed(int key) {
	return (GetAsyncKeyState(key) & 0x8000 != 0);
}
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {

	PKBDLLHOOKSTRUCT k = (PKBDLLHOOKSTRUCT)(lParam);
	POINT p;


	if (wParam == WM_RBUTTONDOWN)
	{
		// right button clicked 
		GetCursorPos(&p);
	}
	return (0L);
}

void ReduceToUnit(float vector[3])
	{
	float length;
	
	// Calculate the length of the vector		
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
	}


// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
	{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
	}



// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(GLsizei w, GLsizei h)
	{
	GLfloat nRange = 100.0f;
	GLfloat fAspect;
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	lastWidth = w;
	lastHeight = h;
		
	fAspect=(GLfloat)w/(GLfloat)h;
	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	if (ButtonL) glTranslated(0, -0.5, 0);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h) 
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else 
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

	// Establish perspective: 
	/*
	gluPerspective(60.0f,fAspect,1.0,400);
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	}



// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
	{
	// Light values and coordinates
	//GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	//GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	//GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	//GLfloat	 lightPos[] = { 0.0f, 150.0f, 150.0f, 1.0f };
	//GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	//glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Enable lighting
	//glEnable(GL_LIGHTING);

	// Setup and enable light 0
	//glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	//glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	//glEnable(GL_LIGHT0);

	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	//glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	//glMateriali(GL_FRONT,GL_SHININESS,128);


	// White background
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f );
	// Black brush
	glColor3f(0.0,0.0,0.0);
	}

// LoadBitmapFile
// opis: ładuje mapę bitową z pliku i zwraca jej adres.
//       Wypełnia strukturę nagłówka.
//	 Nie obsługuje map 8-bitowych.
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// wskaźnik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nagłówek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	int					imageIdx = 0;		// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany składowych

	// otwiera plik w trybie "read binary"
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// wczytuje nagłówek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// sprawdza, czy jest to plik formatu BMP
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// wczytuje nagłówek obrazu
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// ustawia wskaźnik pozycji pliku na początku danych obrazu
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// przydziela pamięć buforowi obrazu
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// sprawdza, czy udało się przydzielić pamięć
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// wczytuje dane obrazu
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// sprawdza, czy dane zostały wczytane
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// zamienia miejscami składowe R i B 
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// zamyka plik i zwraca wskaźnik bufora zawierającego wczytany obraz
	fclose(filePtr);
	return bitmapImage;
}
// Callback function called by GLUT when window size changes
void OnReshape(int width, int height)
{
	// Set OpenGL viewport
	glViewport(0, 0, width, height);

	// Send the new window size to AntTweakBar
	TwWindowSize(width, height);
}

// Function called at exit
void OnTerminate(void)
{
	// terminate AntTweakBar
	TwTerminate();
}

// Event callbacks
void OnMouseButton(int glutButton, int glutState, int mouseX, int mouseY)
{
	// send event to AntTweakBar
	if (TwEventMouseButtonGLUT(glutButton, glutState, mouseX, mouseY))
		glutPostRedisplay(); // request redraw if event has been handled
}

void OnMouseMotion(int mouseX, int mouseY)
{
	// send event to AntTweakBar
	if (TwEventMouseMotionGLUT(mouseX, mouseY))
		glutPostRedisplay(); // request redraw if event has been handled
}

void OnKeyboard(unsigned char glutKey, int mouseX, int mouseY)
{
	// send event to AntTweakBar
	if (TwEventKeyboardGLUT(glutKey, mouseX, mouseY))
		glutPostRedisplay(); // request redraw if event has been handled
}

void OnSpecial(int glutKey, int mouseX, int mouseY)
{
	// send event to AntTweakBar
	if (TwEventSpecialGLUT(glutKey, mouseX, mouseY))
		glutPostRedisplay(); // request redraw if event has been handled
}
void spiral(int a, int b, int c, int d, int k) {
#define AM points//iloœæ punktów
#define M_PI 3.141592
	float clr = 0.5f;
	//AM points;
	int i = 0;
	//int t=AM;
	GLfloat vertices[500][3];

	float t;
	float step = 1.0f / (float)AM;
	for (t = 0.0f; t < 1.0f && i < AM; t += step, i++)
	{
		//po³o¿enie punktu wzglêdem osi x
		vertices[i][0] = cos(2 * M_PI * k * t) * sqrt(d * t) * b;
		//po³o¿enie punktu wzglêdem osi y
		vertices[i][1] = sin(2 * M_PI * k * t) * sqrt(d * t) * b;
		//po³o¿enie punktu wzglêdem osi z
		vertices[i][2] = (float)(t * c) - 60.0f;
	}

	glLineWidth(size);

	glBegin(GL_LINE_STRIP);
	for (i = 0; i < AM; i++) {
		glVertex3fv(vertices[i]);
		glColor3f(clr, clr / 40.0f, clr / 0.5f);
		//glColor3f(clr, clr / 2.0f, clr / 2.0f);
		clr += step;
	}
	glEnd();
}
void floor() {

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		// Parametry wierzcholkow
		GLfloat sa[3] = { -80.0f, -90.0f, 50.0f };
		GLfloat sb[3] = { 80.0f,-90.0f, 50.0f };
		GLfloat sc[3] = { 80.0f,-80.0f,50.0f };
		GLfloat sd[3] = { -80.0f, -80.0f,50.0f };
		GLfloat se[3] = { -80.0f, -90.0f, -50.0f };
		GLfloat sf[3] = { 80.0f,-90.0f, -50.0f };
		GLfloat sg[3] = { 80.0f,-80.0f,-50.0f };
		GLfloat sh[3] = { -80.0f, -80.0f,-50.0f };

		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sa);
		glVertex3fv(sb);
		glVertex3fv(sc);
		glVertex3fv(sd);
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sb);
		glVertex3fv(sf);
		glVertex3fv(sg);
		glVertex3fv(sc);
		glEnd();

		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sf);
		glVertex3fv(se);
		glVertex3fv(sh);
		glVertex3fv(sg);
		glEnd();

		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(se);
		glVertex3fv(sa);
		glVertex3fv(sd);
		glVertex3fv(sh);
		glEnd();

		glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sd);
		glVertex3fv(sc);
		glVertex3fv(sg);
		glVertex3fv(sh);
		glEnd();

		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sa);
		glVertex3fv(sb);
		glVertex3fv(sf);
		glVertex3fv(se);
		glEnd();
	}
}
void gumka()
{
	#define AMOUNT 15
	GLfloat col_r = 0.0f;

		s1 = float(rand() % 50 - 0);
		s2 = float(rand() % 50 - 0);
		s3 = float(rand() % 50 - 0);
	
	GLfloat s[3] = {s1, s2, s3};
	//czerwony
	GLfloat sa[3] = { params[0], params[1], params[2] };
	//niebieski
	GLfloat sb[3] = { params[3], params[4], params[5] };

	point points[AMOUNT];
	points[0].set_tie_a(params[0], params[1], params[2]);
	points[0].set_start_position(s[0], s[1], s[2]);
	points[AMOUNT-1].set_tie_b(params[3], params[4], params[5]);
	points[AMOUNT-1].set_start_position(s[0], s[1], s[2]);
	for (int i = 1; i < AMOUNT-1; i++) {
		s1 = float(rand() % 50 - 0);
		s2 = float(rand() % 50 - 0);
		s3 = float(rand() % 50 - 0);
		points[i].set_start_position(s1, s2, s3);
	}
	glLineWidth(5.0f);
	glPointSize(1	);


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
void OnDisplay(void)
{
	// Clear frame buffer
	glClearColor(0.5f, 0.5f, 0.6f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// App drawing here
	// ...

	// Draw tweak bars
	TwDraw();

	// Present frame buffer
	glutSwapBuffers();
}

void rysuj()
{
	int argc = 1;
	char *argv[1] = { (char*)"Something" };
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutCreateWindow("AntTweakBar string example");
	glutCreateMenu(NULL);

	// Set GLUT callbacks
	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(OnReshape);
	atexit(OnTerminate);  // Called after glutMainLoop ends

	// Initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);

	// Set GLUT event callbacks
	// - Directly redirect GLUT mouse button events to AntTweakBar
	glutMouseFunc(OnMouseButton);
	// - Directly redirect GLUT mouse motion events to AntTweakBar
	glutMotionFunc(OnMouseMotion);
	// - Directly redirect GLUT mouse "passive" motion events to AntTweakBar (same as MouseMotion)
	glutPassiveMotionFunc(OnMouseMotion);
	// - Directly redirect GLUT key events to AntTweakBar
	glutKeyboardFunc(OnKeyboard);
	// - Directly redirect GLUT special key events to AntTweakBar
	glutSpecialFunc(OnSpecial);
	// - Send 'glutGetModifers' function pointer to AntTweakBar;
	//   required because the GLUT key event functions do not report key modifiers states.
	TwGLUTModifiersFunc(glutGetModifiers);
}
void RenderScene2(void)
{
	//float normal[3];	// Storeage for calculated surface normal

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	/////////////////////////////////////////////////////////////////
	// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
	/////////////////////////////////////////////////////////////////
	//szescian();

	//Sposób na odróźnienie "przedniej" i "tylniej" ściany wielokąta:
	glPolygonMode(GL_BACK, GL_LINE);
	//walec(40, 40);

	srand(5);
	gumka();
	//iterations++;

/*if (start == true)
{
	for (int i = 0; i < 300; i++)
	{
		srand(5);
		gumka();
		iterations++;
	}
	start = false;
}*/
	floor();
	//spiral(2, width_model, hight_model, 2, freq);
	/*glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Flush drawing commands
	glFlush();*/
	TwDraw();
	//glutMainLoop();//loops the current event
}

// Called to draw scene
void RenderScene(void)
	{
	//float normal[3];	// Storeage for calculated surface normal

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	/////////////////////////////////////////////////////////////////
	// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
	/////////////////////////////////////////////////////////////////
	//szescian();
	
	//Sposób na odróźnienie "przedniej" i "tylniej" ściany wielokąta:
	glPolygonMode(GL_BACK,GL_LINE);
	//walec(40, 40);

		srand(5);
		gumka();


	floor();
	//spiral(2, width_model, hight_model, 2, freq);
	/*glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Flush drawing commands
	glFlush();*/
	TwDraw();
	//glutMainLoop();//loops the current event
	}



// Select the pixel format for a given device context
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



// If necessary, creates a 3-3-2 palette for the device context listed.
HPALETTE GetOpenGLPalette(HDC hDC)
	{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange,GreenRange,BlueRange;
								// Range for each color entry (7,7,and 3)


	// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if(!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;	

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +nColors*sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) -1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) -1;

	// Loop through all the palette entries
	for(i = 0; i < nColors; i++)
		{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char) NULL;
		}
		

	// Create the palette
	hRetPal = CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(hDC,hRetPal,FALSE);
	RealizePalette(hDC);

	// Free the memory used for the logical palette structure
	free(pPal);

	// Return the handle to the new palette
	return hRetPal;
	}
float r, g, b, x, y;
bool check = true;
void mouse(int button, int state, int mousex, int mousey)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		check = true;

		x = mousex;
		y = 480 - mousey;

		r = (rand() % 9) / 8;
		g = (rand() % 9) / 8;
		b = (rand() % 9) / 8;
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//undo(clear)the drawing
	{
		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		check = false;
	}
	glutPostRedisplay();
}
void display(void)
{
	glColor3f(r, g, b); // sets the current drawing (foreground) color to blue 
	glPointSize(50); // sets the size of points to be drawn (in pixels)

	glMatrixMode(GL_PROJECTION);// sets the current matrix to projection
	glLoadIdentity();//multiply the current matrix by identity matrix
	gluOrtho2D(0.0, 640, 0.0, 480);//sets the parallel(orthographic) projection of the full frame buffer 

	if (check)
	{
		glBegin(GL_POINTS); // writes pixels on the frame buffer with the current drawing color
		glVertex2i(x, y);   // sets vertex
		
		glEnd();
	}
	gumka();
	glFlush();     // flushes the frame buffer to the screen
}

// Entry point of all Windows programs
int APIENTRY WinMain(   HINSTANCE       hInst,
						HINSTANCE       hPrevInstance,
						LPSTR           lpCmdLine,
						int                     nCmdShow)
	{
	static TCHAR lpszAppName[] = TEXT("Connect");
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle

	hInstance = hInst;

	// Register Window style
	wc.style                        = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc          = (WNDPROC) WndProc;
	wc.cbClsExtra           = 0;
	wc.cbWndExtra           = 0;
	wc.hInstance            = hInstance;
	wc.hIcon                        = NULL;
	wc.hCursor                      = LoadCursor(NULL, IDC_ARROW);
	
	// No need for background brush for OpenGL window
	wc.hbrBackground        = NULL;         
	
	wc.lpszMenuName         = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName        = lpszAppName;

	// Register the window class
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, TEXT("Program requires Windows NT!"), lpszAppName, MB_ICONERROR);
		return FALSE;
	}


	// Create the main application window
	hWnd = CreateWindow(
				lpszAppName,
				lpszAppName,
				
				// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
	
				// Window position and size
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
				NULL,
				NULL,
				hInstance,
				NULL);
	/*int argc = 1;
	char *argv[1] = { (char*)"Something" };
		glutInit(&argc, argv);
		glutInitWindowSize(640, 480);   //sets the width and height of the window in pixels
		glutInitWindowPosition(10, 10);//sets the position of the window in pixels from top left corner 
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//creates a single frame buffer of RGB color capacity.
		glutCreateWindow("DDA Line Drawing");//creates the window as specified by the user as above.

		glClearColor(1, 1, 1, 0); // sets the backgraound color to white light
		glClear(GL_COLOR_BUFFER_BIT); // clears the frame buffer and set values defined in glClearColor() function call 

		glutDisplayFunc(display);//links the display event with the display event handler(display)
		glutMouseFunc(mouse);//keyboard event handler
		//glutMainLoop();//loops the current event
	 TwInit(TW_OPENGL, NULL);
	 */
	/*glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); // use default light diffuse and position
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);*/
	// If window was not created, quit
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(600, 400);
	TwBar *myBar;
	myBar = TwNewBar("Parameters");
	TwAddVarRW(myBar, "Params[0]", TW_TYPE_FLOAT, &params[0], "");
	TwAddVarRW(myBar, "Params[1]", TW_TYPE_FLOAT, &params[1], "");
	TwAddVarRW(myBar, "Params[2]", TW_TYPE_FLOAT, &params[2], "");
	TwAddVarRW(myBar, "Params[3]", TW_TYPE_FLOAT, &params[3], "");
	TwAddVarRW(myBar, "Params[4]", TW_TYPE_FLOAT, &params[4], "");
	TwAddVarRW(myBar, "Params[5]", TW_TYPE_FLOAT, &params[5], "");
	TwAddVarRW(myBar, "Params[6]", TW_TYPE_FLOAT, &params[6], "");
	TwAddVarRW(myBar, "Iterations", TW_TYPE_INT32, &iterations, "");
	TwAddVarRW(myBar, "width_model", TW_TYPE_FLOAT, &iterations,
		" label='X path coeff' keyIncr=1 keyDecr=CTRL+1 min=-10 max=100 step=1 ");

	//TwAddButton(myBar, "Run", RunCB, NULL, "label = 'Pokemon'");

	MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hInstance, 0);
	iterations++;

	UnhookWindowsHookEx(MouseHook);
	// Process application messages until the application closes
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while( GetMessage(&msg, NULL, 0, 0))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

	//glutMainLoop();//loops the current event
	return msg.wParam;

	}

// In the Windows MessageProc callback
LRESULT CALLBACK MessageProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (TwEventWin(wnd, msg, wParam, lParam)) // send event message to AntTweakBar
		return 0; // event has been handled by AntTweakBar
	// else process the event message
	// ...
}



// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(       HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam)
	{
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context

	switch (message)
	{
		// Window creation, setup for OpenGL
	case WM_CREATE:
		// Store the device context
		hDC = GetDC(hWnd);
		//TwInit(TW_OPENGL_CORE, NULL);
		//TwInit(TW_OPENGL, NULL);
		TwWindowSize(400, 600);
		// Select the pixel format
		SetDCPixelFormat(hDC);

		// Create palette if needed
		hPalette = GetOpenGLPalette(hDC);

		// Create the rendering context and make it current
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		SetupRC();
		glGenTextures(2, &texture[0]);                  // tworzy obiekt tekstury			

		// ładuje pierwszy obraz tekstury:
		bitmapData = LoadBitmapFile("Bitmapy\\checker.bmp", &bitmapInfoHeader);

		glBindTexture(GL_TEXTURE_2D, texture[0]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

		if (bitmapData)
			free(bitmapData);

		// ładuje drugi obraz tekstury:
		bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
		glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);



		if (bitmapData)
			free(bitmapData);

		// ustalenie sposobu mieszania tekstury z tłem
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		break;

		// Window is being destroyed, cleanup
	case WM_DESTROY:
		// Deselect the current rendering context and delete it
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);

		// Delete the palette if it was created
		if (hPalette != NULL)
			DeleteObject(hPalette);
		//TwTerminate();
		// Tell the application to terminate after the window
		// is gone.
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
	{
		// Call OpenGL drawing code
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
		if (wParam == VK_UP)
			xRot -= 5.0f;

		if (wParam == VK_DOWN)
			xRot += 5.0f;

		if (wParam == VK_LEFT)
			yRot -= 5.0f;

		if (wParam == VK_RIGHT)
			yRot += 5.0f;

		if (wParam == 'Q')
			params[0] += 1.0f;
		if (wParam == 'A')
			params[0] -= 1.0f;
		if (wParam == 'W')
			params[1] += 1.0f;
		if (wParam == 'S')
			params[1] -= 1.0f;
		if (wParam == 'E')
			params[2] += 1.0f;
		if (wParam == 'D')
			params[2] -= 1.0f;
		if (wParam == 'R')
			params[3] += 1.0f;
		if (wParam == 'F')
			params[3] -= 1.0f;
		if (wParam == 'T')
			params[4] += 1.0f;
		if (wParam == 'G')
			params[4] -= 1.0f;
		if (wParam == 'Y')
			params[5] += 1.0f;
		if (wParam == 'H')
			params[5] -= 1.0f;
		if (wParam == 'U')
			params[6] += 0.1f;
		if (wParam == 'J')
			params[6] -= 0.1f;
		if (wParam == 'I')
			iterations++;
		if (wParam == 'K')
			iterations--;
		if (wParam == VK_LBUTTON)
			iterations = 0;
		if (wParam == GLUT_LEFT)
			iterations = 0;
		if (wParam == WM_LBUTTONDOWN)
			iterations = 0;
		if (wParam == TW_KEY_BACKSPACE)
			iterations = 0;
		if (wParam == VK_SPACE)
		//	start = true;
		if (wParam == TW_KEY_LEFT)
			iterations = 0;
		if (keyPressed(VK_LBUTTON)) {
			iterations = 0;
		}
		if (wParam == 0x201)
		{
			iterations = 0;
		}
		xRot = (const int)xRot % 360;
		yRot = (const int)yRot % 360;

		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;
	case WM_LBUTTONDOWN:
		if (keyPressed(VK_LBUTTON))
		{
			start = true;

		}

			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case WM_RBUTTONDOWN:
			if (keyPressed(VK_RBUTTON))
				iterations = 0;

			InvalidateRect(hWnd, NULL, FALSE);
			break;

	/*	case WM_LBUTTONUP:
			if (wParam == GLUT_LEFT)
				iterations++;
			if (keyPressed(VK_LBUTTON))
				iterations++;
			if (wParam == WM_LBUTTONDOWN)
				iterations = 0;
			if (wParam == GLUT_LEFT_BUTTON)
				iterations++;
			iterations++;
			break;*/
		// A menu command
		case WM_COMMAND:
			{
			switch(LOWORD(wParam))
				{
				// Exit the program
				case ID_FILE_EXIT:
					DestroyWindow(hWnd);
					break;

				}
			}
			break;


	default:   // Passes it on if unproccessed
	    return (DefWindowProc(hWnd, message, wParam, lParam));

	}

    return (0L);
	}


// Dialog procedure.
BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
	{
	
    switch (message)
	{
		// Initialize the dialog box
	    case WM_INITDIALOG:
			{
			int i;
			GLenum glError;


			return (TRUE);
			}
			break;

		// Process command messages
	    case WM_COMMAND:      
			{
			// Validate and Make the changes
			if(LOWORD(wParam) == IDOK)
				EndDialog(hDlg,TRUE);
		    }
			break;

		// Closed from sysbox
		case WM_CLOSE:
			EndDialog(hDlg,TRUE);
			break;
		}

	return FALSE;
	}
