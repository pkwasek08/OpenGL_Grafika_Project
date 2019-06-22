#pragma once
#pragma once
#include <windows.h>
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h> 
#include <math.h>	
class point
{
private:
	GLfloat position[3];
	GLfloat prev_position[3];
	GLfloat tie_a_position[3]= { 0.0f, 0.0f, 0.0f };
	GLfloat tie_b_position[3] = { 0.0f, 0.0f, 0.0f };
	GLfloat weight = 0.04f;
	GLfloat ratio_1 = 1.0f;
	GLfloat ratio_2 = 2.0f;
	GLfloat ratio_3 = 0.8f;
	GLfloat ratio_4 = 0.2f;
public:
	point();
	GLfloat *vector = new GLfloat[3];
	GLfloat *vector_A = new GLfloat[3];
	GLfloat *vector_B = new GLfloat[3];

	void set_tie_a(GLfloat x, GLfloat y, GLfloat z);
	void set_tie_a(GLfloat *vector);
	void set_tie_b(GLfloat x, GLfloat y, GLfloat z);
	void set_tie_b(GLfloat *vector);
	void set_position(GLfloat x, GLfloat y, GLfloat z);
	void set_prev_position(GLfloat x, GLfloat y, GLfloat z);
	void set_start_position(GLfloat x, GLfloat y, GLfloat z);
	void set_weight(GLfloat w);
	GLfloat * update_position(void);
	GLfloat * get_vector(const GLfloat *pos_a, const GLfloat *pos_b);
	GLfloat * get_vector_A(const GLfloat *pos_a, const GLfloat *pos_b);
	GLfloat * get_vector_B(const GLfloat *pos_a, const GLfloat *pos_b);
	GLfloat * add_vector(const GLfloat *vec_a, const GLfloat *vec_b);
	GLfloat vector_len(const GLfloat *vector);
	GLfloat * get_position();
	GLfloat * get_tie_a_position();
	GLfloat * get_tie_b_position();
	~point();
};

