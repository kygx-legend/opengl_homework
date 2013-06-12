/*  robot named suzaku
    for computer graphics assignment03
    author: 李冠贤 10389043
    date: 2013-03-28 */

#include "GL/freeglut.h"
#include <iostream>
#include <cmath>
using namespace std;


#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
static GLfloat PI=acos(-1);

//window parameters
static GLsizei width=500;
static GLsizei height=500;

//view parameters
static GLfloat eye_x=0.0,eye_y=0.0,eye_z=0.0; 
static GLfloat at_x=0.0,at_y=0.0,at_z=0.0; 
static GLfloat up_x=0.0,up_y=0.0,up_z=0.0; 
static GLfloat radius=4.0; 
static GLfloat theta=0.0; 
static GLfloat rotate=1.0/48*PI; 
static GLfloat zoom=0.2;

//robot rotate parameters
static GLint head=0;
static GLint l_shoulder=0;
static GLint l_elbow=0;
static GLint r_shoulder=0;
static GLint r_elbow=0;
static GLint l_leg=0;
static GLint r_leg=0;

static GLint head_speed=1;
static GLint l_shoulder_speed=1;
static GLint l_elbow_speed=1;
static GLint r_shoulder_speed=1;
static GLint r_elbow_speed=1;
static GLint l_leg_speed=1;
static GLint r_leg_speed=-1;

void view_reload(){
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(50.0,width/height,1.0,20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    gluLookAt(eye_x,eye_y,eye_z,at_x,at_y,at_z,up_x,up_y,up_z);
}

void view_zoom(int select){
    if(select==1)
        radius-=zoom;
    else if(select==2)
        radius+=zoom;
    eye_x=radius*sin(theta);
    eye_y=0.0;
    eye_z=radius*cos(theta);
    view_reload();
}

void view_move(int select){
    if(select==1)
        theta-=rotate;
    else if(select==2)
        theta+=rotate;
    eye_x=radius*sin(theta);
    eye_y=0.0;
    eye_z=radius*cos(theta);
    cout << theta << endl;
    cout << eye_x << ' ' << eye_y << ' ' << eye_z << endl;
    view_reload();
}

void view_init(){
    //x=r*cos(0);
    //y=0;
    //z=r*sin(0);
    radius=9.0;
    view_zoom(0);
    up_x=0.0;
    up_y=1.0;
    up_z=0.0;
}

void movehead(){
    if((head+head_speed)==60||(head+head_speed)==-60)
        head_speed=0-head_speed;
    head+=head_speed;
}

void movehand(){
    if((l_shoulder+l_shoulder_speed)==25||(l_shoulder+l_shoulder_speed)==-25)
        l_shoulder_speed=0-l_shoulder_speed;
    if((l_elbow+l_elbow_speed)==25||(l_elbow+l_elbow_speed)==-25)
        l_elbow_speed=0-l_elbow_speed;
    if((r_shoulder+r_shoulder_speed)==30||(r_shoulder+r_shoulder_speed)==-30)
        r_shoulder_speed=0-r_shoulder_speed;
    if((r_elbow+r_elbow_speed)==30||(r_elbow+r_elbow_speed)==-30)
        r_elbow_speed=0-r_elbow_speed;
    l_shoulder+=l_shoulder_speed;
    l_elbow+=l_elbow_speed;
    r_shoulder+=r_shoulder_speed;
    r_elbow+=r_elbow_speed;
}

void moveleg(){
    if((l_leg+l_leg_speed)==15||(l_leg+l_leg_speed)==-15)
        l_leg_speed=0-l_leg_speed;
    if((r_leg+r_leg_speed)==15||(r_leg+r_leg_speed)==-15)
        r_leg_speed=0-r_leg_speed;
    l_leg+=l_leg_speed;
    r_leg+=r_leg_speed;
}

void motion(void){
    //move head
    movehead();
    //move hand
    movehand();
    //move leg
    moveleg();
    glutPostRedisplay();
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
    glLineWidth(1.5);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    view_init();
}

void idle(void){}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
    //robot whole ------1
	glPushMatrix();

        //robot head -------2
        glPushMatrix();
            glTranslatef(0.0,2.0,0.0);
            glRotatef(head,0.0,1.0,0.0);
            //robot head: brain
            glPushMatrix();
                glScalef(3.0,2.0,1.5);
                glutWireCube(1.0);
            glPopMatrix();
            //robot head: left ear
            glPushMatrix();
                glTranslatef(-1.75,0.0,0.0);
                glScalef(0.5,0.6,1.5);
                glutWireCube(1.0);
            glPopMatrix();
            //robot head: right ear
            glPushMatrix();
                glTranslatef(1.75,0.0,0.0);
                glScalef(0.5,0.6,1.5);
                glutWireCube(1.0);
            glPopMatrix();
        glPopMatrix();
        //------------------2

        //robot body -------3
        glPushMatrix();
        //robot body: body
            glPushMatrix();
                glScalef(1.5,2.0,1.5);
                glutWireCube(1.0);
            glPopMatrix();
            //robot body: left hand
            glPushMatrix();
                glTranslatef(-0.75,0.5,0.0);
                glRotatef(l_shoulder,0.0,0.0,1.0);
                glTranslatef(-1.0,0.0,0.0);
                //robot body: left shoulder
                glPushMatrix();
                    glScalef(2.0,0.5,1.0);
                    glutWireCube(1.0);
                glPopMatrix();
                //robot body: left elbow
                glPushMatrix();
                    glTranslatef(-1.0,0.0,0.0);
                    glRotatef(l_elbow,0.0,0.0,1.0);
                    glTranslatef(-1.0,0.0,0.0);
                    glScalef(2.0,0.5,1.0);
                    glutWireCube(1.0);
                glPopMatrix();
            glPopMatrix();
            //robot body: left hand
            glPushMatrix();
                glTranslatef(0.75,0.5,0.0);
                glRotatef(r_shoulder,0.0,0.0,1.0);
                glTranslatef(1.0,0.0,0.0);
                //robot body: right shoulder
                glPushMatrix();
                    glScalef(2.0,0.5,1.0);
                    glutWireCube(1.0);
                glPopMatrix();
                //robot body: right elbow
                glPushMatrix();
                    glTranslatef(1.0,0.0,0.0);
                    glRotatef(r_elbow,0.0,0.0,1.0);
                    glTranslatef(1.0,0.0,0.0);
                    glScalef(2.0,0.5,1.0);
                    glutWireCube(1.0);
                glPopMatrix();
            glPopMatrix();
            //robot body: leg
            glPushMatrix();
                glRotatef(l_leg,1.0,0.0,0.0);
                glTranslatef(-0.3,-2.0,0.0);
                glScalef(0.5,2.0,1.0);
                glutWireCube(1.0);
            glPopMatrix();
            glPushMatrix();
                glRotatef(r_leg,1.0,0.0,0.0);
                glTranslatef(0.3,-2.0,0.0);
                glScalef(0.5,2.0,1.0);
                glutWireCube(1.0);
            glPopMatrix();
        glPopMatrix();
        //------------------3

	glPopMatrix();
    //------------------1

    //coordinate
    //x
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(1.0,0.0,0.0);
    glEnd();
    //y
    glBegin(GL_LINES);
        glColor3f(0.0,1.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,1.0,0.0);
    glEnd();
    //z
    glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.0,1.0);
    glEnd();

    glColor3f(1.0,1.0,1.0);

	glutSwapBuffers();
}

void reshape(int w,int h){
    width=(GLsizei)w;
    height=(GLsizei)h;
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(50.0,(GLfloat)w/(GLfloat)h,1.0,20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    gluLookAt(eye_x,eye_y,eye_z,at_x,at_y,at_z,up_x,up_y,up_z);
}


void mouse(int button,int state,int x,int y){
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
        }
        else if(state == GLUT_UP){}
    }
    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
        }
        else if(state == GLUT_UP){}
    }
    if (state == GLUT_UP && button == GLUT_WHEEL_UP){ 
        view_zoom(1);
        glutPostRedisplay();
    }
    if (state == GLUT_UP && button == GLUT_WHEEL_DOWN){
        view_zoom(2);
        glutPostRedisplay();
    }
}

void mouseMotion(int x, int y){
}

void keyboard(unsigned char key,int x,int y){
	switch(key){
        case 'r':
            glutIdleFunc(motion);
            break;
        case 'R':
            glutIdleFunc(idle);
            break;
        case 's':
            break;
        case 'w':
            break;
        case 'a':
            view_move(1);
            glutPostRedisplay();
            break;
        case 'd':
            view_move(2);
            glutPostRedisplay();
            break;
		default:
			break;
	}
}

int main(int argc,char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(width,height);
	glutInitWindowPosition(0,0);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
	init();
	glutMainLoop();
	return 0;
}
