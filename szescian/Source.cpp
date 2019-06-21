#include "Header.h"



point::point()
{
}

void point::set_tie_a(GLfloat x, GLfloat y, GLfloat z)
{
	tie_a_position[0] = x;
	tie_a_position[1] = y;
	tie_a_position[2] = z;
}

void point::set_tie_a(GLfloat * vector)
{
	set_tie_a(vector[0], vector[1], vector[2]);
}

void point::set_tie_b(GLfloat x, GLfloat y, GLfloat z)
{
	tie_b_position[0] = x;
	tie_b_position[1] = y;
	tie_b_position[2] = z;
}

void point::set_tie_b(GLfloat * vector)
{
	set_tie_b(vector[0], vector[1], vector[2]);
}

void point::set_position(GLfloat x, GLfloat y, GLfloat z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

void point::set_prev_position(GLfloat x, GLfloat y, GLfloat z)
{
	prev_position[0] = x;
	prev_position[1] = y;
	prev_position[2] = z;
}

void point::set_start_position(GLfloat x, GLfloat y, GLfloat z)
{
	set_position(x, y, z);
	set_prev_position(x, y, z);
}

void point::set_weight(GLfloat w)
{
	weight = w;
}

GLfloat * point::update_position(void)
{

	GLfloat *prev_move = get_vector(prev_position, position);
	GLfloat *pa;
	GLfloat *pb;
	GLfloat fg[3] = { 0.0f, -10.0f, 0.0f };
	GLfloat fw[3];

	if (position[1] < -48.0f) return position;

	pa = get_vector_A(position, tie_a_position);
	pb = get_vector_B(position, tie_b_position);
	prev_position[0] = position[0];
	prev_position[1] = position[1];
	prev_position[2] = position[2];

	//Fa
	pa[0] *= ratio_1;
	pa[1] *= ratio_1;
	pa[2] *= ratio_1;

	//Fb
	pb[0] *= ratio_1;
	pb[1] *= ratio_1;
	pb[2] *= ratio_1;

	//Fg
	fg[1] *= weight * ratio_2;

	//Fm
	prev_move[0] *= weight * ratio_3;
	prev_move[1] *= weight * ratio_3;
	prev_move[2] *= weight * ratio_3;

	//Fw
	fw[0] = pa[0] + pb[0] + fg[0] + prev_move[0];
	fw[1] = pa[1] + pb[1] + fg[1] + prev_move[1];
	fw[2] = pa[2] + pb[2] + fg[2] + prev_move[2];
	fw[0] *= ratio_4;
	fw[1] *= ratio_4;
	fw[2] *= ratio_4;

	//new pos
	position[0] += fw[0];
	position[1] += fw[1];
	position[2] += fw[2];

	return position;
}

GLfloat * point::get_vector(const GLfloat * pos_a, const GLfloat * pos_b)
{
	vector[0] = pos_b[0] - pos_a[0];
	vector[1] = pos_b[1] - pos_a[1];
	vector[2] = pos_b[2] - pos_a[2];
	return vector;
}
GLfloat * point::get_vector_A(const GLfloat * pos_a, const GLfloat * pos_b)
{
	vector_A[0] = pos_b[0] - pos_a[0];
	vector_A[1] = pos_b[1] - pos_a[1];
	vector_A[2] = pos_b[2] - pos_a[2];
	return vector_A;
}
GLfloat * point::get_vector_B(const GLfloat * pos_a, const GLfloat * pos_b)
{
	vector_B[0] = pos_b[0] - pos_a[0];
	vector_B[1] = pos_b[1] - pos_a[1];
	vector_B[2] = pos_b[2] - pos_a[2];
	return vector_B;
}
GLfloat * point::add_vector(const GLfloat * vec_a, const GLfloat * vec_b)
{
	vector[0] = vec_a[0] + vec_b[0];
	vector[1] = vec_a[1] + vec_b[1];
	vector[2] = vec_a[2] + vec_b[2];
	return vector;
}

GLfloat point::vector_len(const GLfloat * vector)
{
	GLfloat length;
	length = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
	return length;
}

GLfloat * point::get_position()
{
	return position;
}

GLfloat * point::get_tie_a_position()
{
	return tie_a_position;
}

GLfloat * point::get_tie_b_position()
{
	return tie_b_position;
}

point::~point()
{
	delete vector;
	delete vector_A;
	delete vector_B;
	/*delete pa;
	delete pb;
	delete prev_move;*/
}
