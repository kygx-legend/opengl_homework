/*  
    for computer graphics assignment03
    rasterization
    author: 李冠贤 10389043
    date: 2013-04-08 
*/
#include "GL/freeglut.h"
#include <iostream>
#include <cmath>
#include <algorithm>

#include <vector>
#include "grids.h"
using namespace std;

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
static GLfloat PI=acos(-1);

//window parameters
static GLsizei width=200;
static GLsizei height=200;

//view parameters
static GLdouble oleft=0.0f,oright=200.0f,obottom=0.0f,otop=200.0f; 

//grids
Grids *grids;
Triangle *triangle;

void grids_init(){
    grids=new Grids(width/10,height/10,10);
    triangle=new Triangle();
}

void view_init(){
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D(oleft,oright,obottom,otop);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void){
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    view_init();
    grids_init();
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    grids->draw();
	glutSwapBuffers();
}

void reshape(int w,int h){
    width=(GLsizei)w;
    height=(GLsizei)h;
    oright=width;
    otop=height;
    grids->resize(w/10,h/10,10);
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D(oleft,oright,obottom,otop);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glutPostRedisplay();
}

void mouse_click(int button,int state,int x,int y){
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            //cout << x/10 << (height-y)/10 << endl;
            Vertex v(x/10,(height-y)/10);
            if(triangle->count==0){
                triangle->v1=v;
                triangle->count++;
            }
            else if(triangle->count==1){
                triangle->v2=v;
                triangle->count++;
            }
            else if(triangle->count==2){
                triangle->v3=v;
                triangle->count++;
            }
            triangle->set_color(grids);
            glutPostRedisplay();
        }
        else if(state == GLUT_UP){}
    }
    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            if(triangle->count==3){
                triangle->rasterization(grids);
            }
            glutPostRedisplay();
        }
        else if(state == GLUT_UP){}
    }
    if (state == GLUT_UP && button == GLUT_WHEEL_UP){ 
    }
    if (state == GLUT_UP && button == GLUT_WHEEL_DOWN){
    }
}

void mouse_motion(int x, int y){
}

void keyboard(unsigned char key,int x,int y){
	switch(key){
        case 's':
            break;
        case 'w':
            break;
        case 'a':
            break;
        case 'd':
            break;
		default:
			break;
	}
}

void idle(void){}

void exit(){
    delete triangle;
    delete grids;
}

int main(int argc,char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(width,height);
	glutInitWindowPosition(0,0);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutMouseFunc(mouse_click);
    glutMotionFunc(mouse_motion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
	init();
	glutMainLoop();
    exit();
	return 0;
}
