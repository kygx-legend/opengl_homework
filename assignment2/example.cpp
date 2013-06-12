#include "GL/freeglut.h"

//颜色向量
GLfloat cl_white[]={1.0,1.0,1.0};
GLfloat cl_black[]={0.0,0.0,0.0};
GLfloat cl_red[]={1.0,0.0,0.0};
GLfloat cl_green[]={0.0,1.0,0.0};
GLfloat cl_blue[]={0.0,0.0,1.0};
GLfloat cl_yellow[]={1.0,1.0,0.0};
GLfloat cl_purple[]={1.0,0.0,1.0};

void myDisplay(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
	//clear
	//there are different displays below
}

void myPoint(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glBegin(GL_POINTS);
	glColor3fv(cl_red);
	glVertex3f(0.0f,0.0f,0.0f);
	glEnd();
	glFlush();
}
void myLine(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glBegin(GL_LINES);
	glColor3fv(cl_yellow);
	glVertex3f(1.0f,0.0f,0.0f);
	glVertex3f(8.0f,5.0f,0.0f);
	glVertex3f(-1.0f,-2.0f,0.0f);
	glVertex3f(8.0f,-5.0f,0.0f);
	glEnd();
	glFlush();
}
void myLineStrip(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glBegin(GL_LINE_STRIP);
	glColor3fv(cl_yellow);
	glVertex3f(1.0f,0.0f,0.0f);
	glVertex3f(8.0f,5.0f,0.0f);
	glVertex3f(-1.0f,-2.0f,0.0f);
	glVertex3f(8.0f,-5.0f,0.0f);
	glEnd();
	glFlush();
}
void myLineLoop(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glBegin(GL_LINE_LOOP);
	glColor3fv(cl_yellow);
	glVertex3f(1.0f,0.0f,0.0f);
	glVertex3f(8.0f,5.0f,0.0f);
	glVertex3f(-1.0f,-2.0f,0.0f);
	glVertex3f(8.0f,-5.0f,0.0f);
	glEnd();
	glFlush();
}
void myTriangle(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glBegin(GL_TRIANGLES);
	glColor3fv(cl_white);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(8.0f,0.0f,0.0f);
	glVertex3f(5.0f,5.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(-10.0f,6.0f,0.0f);
	glVertex3f(-10.0f,-10.0f,0.0f);
	glEnd();
	glFlush();
}
void myTriangleStrip(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glBegin(GL_TRIANGLE_STRIP);
	glColor3fv(cl_blue);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(8.0f,0.0f,0.0f);
	glVertex3f(5.0f,5.0f,0.0f);
	glVertex3f(2.0f,2.0f,0.0f);
	glVertex3f(-10.0f,6.0f,0.0f);
	glEnd();
	glFlush();
}
void myTriangleFan(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glBegin(GL_TRIANGLE_FAN);
	glColor3fv(cl_purple);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(8.0f,0.0f,0.0f);
	glVertex3f(5.0f,5.0f,0.0f);
	glVertex3f(2.0f,2.0f,0.0f);
	glVertex3f(-10.0f,6.0f,0.0f);
	glEnd();
	glFlush();
}
void myQuad(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glBegin(GL_QUADS);
	glColor3fv(cl_green);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,8.0f,0.0f);
	glVertex3f(8.0f,8.0f,0.0f);
	glVertex3f(8.0f,0.0f,0.0f);
	glEnd();
	glFlush();
}
void myQuadStrip(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glBegin(GL_QUAD_STRIP);
	glColor3fv(cl_green);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(8.0f,8.0f,0.0f);
	glVertex3f(8.0f,0.0f,0.0f);
	glVertex3f(0.0f,8.0f,0.0f);
	glVertex3f(10.0f,10.0f,0.0f);
	glVertex3f(10.0f,0.0f,0.0f);
	glEnd();
	glFlush();
}
void myPolygon(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glBegin(GL_POLYGON);
	glColor3fv(cl_red);
	glVertex3f(-3.0f,0.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,5.0f,0.0f);
	glVertex3f(2.0f,2.0f,0.0f);
	glVertex3f(-1.0f,2.0f,0.0f);
	glEnd();
	glFlush();
}

void myClear(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	glFlush();
}

void ReshapeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
		case '1':
			myPoint();
			break;
		case '2':
			myLine();
			break;
		case '3':
			myLineStrip();
			break;
		case '4':
			myLineLoop();
			break;
		case '5':
			myTriangle();
			break;
		case '6':
			myTriangleStrip();
			break;
		case '7':
			myTriangleFan();
			break;
		case '8':
			myQuad();
			break;
		case '9':
			myQuadStrip();
			break;
		case '0':
			myPolygon();
			break;
		case 'c':
			myClear();
			break;
	}
}

void IdleFunc(void)
{
	//do nothing;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize(500,400);
    glutInitWindowPosition(400,400);
    glutCreateWindow("assignment2");
    glutDisplayFunc(myDisplay);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutIdleFunc(IdleFunc);
    glutMainLoop();    
    return 0;
}
