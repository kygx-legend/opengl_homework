/*  
    for computer graphics assignment04
    clipping
    author: 李冠贤 10389043
    date: 2013-04-08 
*/

#include "GL/freeglut.h"
#include <iostream>
#include <cmath>
#include <algorithm>

#include <vector>
#include "structure.h"
using namespace std;

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
static GLfloat PI=acos(-1);

//window parameters
static GLsizei width=200;
static GLsizei height=200;

//view parameters
static GLdouble oleft=0.0f,oright=200.0f,obottom=0.0f,otop=200.0f; 

//顶点和边
Polygon *in_polygon;
Polygon *rectangle;
Polygon *clip_polygon;

//切割
bool to_clipping;
GLint xmin,ymin,xmax,ymax;
struct V_order{
    Vertex v;
    GLfloat theta;
};
struct less_than_key{
    inline bool operator()(const V_order& struct1,const V_order& struct2){
        return (struct1.theta < struct2.theta);
    }
};
struct more_than_key{
    inline bool operator()(const V_order& struct1,const V_order& struct2){
        return (struct1.theta > struct2.theta);
    }
};
vector<V_order> vs;

void structure_init(){
    in_polygon=new Polygon(width,height,1);
    rectangle=new Polygon(width,height,0);
    clip_polygon=new Polygon(width,height,2);
    to_clipping=false;
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
    structure_init();
}

void get_min(){
    if(rectangle->vertexs[0].x<rectangle->vertexs[2].x){
        xmin=rectangle->vertexs[0].x;
        xmax=rectangle->vertexs[2].x;
    }
    else{
        xmin=rectangle->vertexs[2].x;
        xmax=rectangle->vertexs[0].x;
    }
    if(rectangle->vertexs[0].y<rectangle->vertexs[2].y){
        ymin=rectangle->vertexs[0].y;
        ymax=rectangle->vertexs[2].y;
    }
    else{
        ymin=rectangle->vertexs[2].y;
        ymax=rectangle->vertexs[0].y;
    }
}

bool inside_rectangle(Vertex v){
    return v.x>=xmin && v.x<=xmax && v.y>=ymin && v.y<=ymax;
}

bool intersect(Edge e1,Edge e2,GLfloat &x,GLfloat &y){
    GLfloat Ax=e1.start.x,Ay=e1.start.y;
    GLfloat Bx=e1.end.x,By=e1.end.y;
    GLfloat Cx=e2.start.x,Cy=e2.start.y;
    GLfloat Dx=e2.end.x,Dy=e2.end.y;
    GLfloat  distAB,theCos,theSin,newX,ABpos;
    //  (1) Translate the system so that point A is on the origin.
    Bx-=Ax; By-=Ay;
    Cx-=Ax; Cy-=Ay;
    Dx-=Ax; Dy-=Ay;
    //  Discover the length of segment A-B.
    distAB=sqrt(Bx*Bx+By*By);
    //  (2) Rotate the system so that point B is on the positive X axis.
    theCos=Bx/distAB;
    theSin=By/distAB;
    newX=Cx*theCos+Cy*theSin;
    Cy  =Cy*theCos-Cx*theSin; Cx=newX;
    newX=Dx*theCos+Dy*theSin;
    Dy  =Dy*theCos-Dx*theSin; Dx=newX;
    //  Fail if segment C-D doesn't cross line A-B.
    if (Cy<0&&Dy<0||Cy>=0&&Dy>=0) 
        return false;
    //  (3) Discover the position of the intersection point along line A-B.
    ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);
    //  Fail if segment C-D crosses line A-B outside of segment A-B.
    if (ABpos<0||ABpos>distAB) 
        return false;
    //  (4) Apply the discovered position to line A-B in the original coordinate system.
    x=Ax+ABpos*theCos;
    y=Ay+ABpos*theSin;
    //  Success.
    return true;
}

bool inside_polygon(Vertex v){
    int count=0;
    GLfloat x,y;
    for(int j=0;j<in_polygon->edges.size();j++){
        if((in_polygon->edges[j].start.y-in_polygon->edges[j].end.y)!=0){
            Vertex vv(0,v.y);
            Edge e(v,vv);
            if(intersect(in_polygon->edges[j],e,x,y))
                count+=1;
        }
    }
    if(count%2==1)
        return true;
    return false;
}


bool is_here(Vertex v){
    for(int i=0;i<vs.size();i++){
        if(vs[i].v.x==v.x&&vs[i].v.y==v.y)
            return true;
    }
    return false;
}

GLfloat cal_theta(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2){
    GLfloat angle=acos((x2-x1)/sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
    return angle * 180.0 / PI;
}

void construct(){
    //GLfloat sumx=0,sumy=0;
    //for(int i=0;i<vs.size();i++){
    //    sumx+=vs[i].v.x;
    //    sumy+=vs[i].v.y;
    //}
    //GLfloat centerx=sumx/vs.size();
    //GLfloat centery=sumy/vs.size();
    //cout << "c" << centerx << ":" << centery << endl;
    //sort(vs.begin(),vs.end(),less_than_key());
    vs[0].theta=0;
    vector<V_order> vtop;
    vector<V_order> vbottom;
    for(int i=1,j;i<vs.size();i++){
        vs[i].theta=cal_theta(vs[i].v.x,vs[i].v.y,vs[0].v.x,vs[0].v.y); 
        //cout << i << ":" << vs[i].theta << endl;
        if(vs[i].v.y-vs[0].v.y>=0)
            vtop.push_back(vs[i]);
        else{
            vbottom.push_back(vs[i]);
        }
    }
    stable_sort(vtop.begin(),vtop.end(),less_than_key());
    stable_sort(vbottom.begin(),vbottom.end(),more_than_key());
    clip_polygon->addVertex(vs[0].v.x,vs[0].v.y);
    for(int i=0;i<vbottom.size();i++){
        clip_polygon->addVertex(vbottom[i].v.x,vbottom[i].v.y);
        //cout << i << ":" << vs[i].theta << endl;
    }
    for(int i=0;i<vtop.size();i++){
        clip_polygon->addVertex(vtop[i].v.x,vtop[i].v.y);
        //cout << i << ":" << vs[i].theta << endl;
    }
    clip_polygon->finish();
}

void clipping(){
    if(to_clipping){
        vs.clear();
        for(int j=0;j<in_polygon->edges.size();j++){
            if(inside_rectangle(in_polygon->edges[j].start)&&inside_rectangle(in_polygon->edges[j].end)){
                //clip_polygon->addVertex(in_polygon->edges[j].start.x,in_polygon->edges[j].start.y);
                //clip_polygon->addVertex(in_polygon->edges[j].end.x,in_polygon->edges[j].end.y);
                Vertex v11(in_polygon->edges[j].start.x,in_polygon->edges[j].start.y);
                Vertex v22(in_polygon->edges[j].end.x,in_polygon->edges[j].end.y);
                V_order v1,v2;
                if(!is_here(v11)){
                    v1.v=v11;
                    vs.push_back(v1);
                }
                if(!is_here(v22)){
                    v2.v=v22;
                    vs.push_back(v2);
                }
            }
            else{
                GLfloat x,y;
                for(int i=0;i<rectangle->edges.size();i++){
                    if(intersect(rectangle->edges[i],in_polygon->edges[j],x,y)){
                        //clip_polygon->addVertex(x,y);
                        Vertex vv(x,y);
                        V_order v;
                        v.v=vv;
                        vs.push_back(v);
                        if(inside_rectangle(in_polygon->edges[j].start)){
                            //clip_polygon->addVertex(in_polygon->edges[j].start.x,in_polygon->edges[j].start.y);
                            Vertex v1(in_polygon->edges[j].start.x,in_polygon->edges[j].start.y);
                            if(!is_here(v1)){
                                V_order v;
                                v.v=v1;
                                vs.push_back(v);
                            }
                        }
                        if(inside_rectangle(in_polygon->edges[j].end)){
                            //clip_polygon->addVertex(in_polygon->edges[j].end.x,in_polygon->edges[j].end.y);
                            Vertex v2(in_polygon->edges[j].end.x,in_polygon->edges[j].end.y);
                            if(!is_here(v2)){
                                V_order v;
                                v.v=v2;
                                vs.push_back(v);
                            }
                        }
                    }
                }
            }
        }
        bool flag=false;
        for(int i=0;i<rectangle->vertexs.size();i++){
            if(inside_polygon(rectangle->vertexs[i])){
                V_order v;
                v.v=rectangle->vertexs[i];
                vs.push_back(v);
                flag=true;
            }
            else
                flag=false;
        }
        if(!flag)
            construct();
        else{
            for(int i=0;i<rectangle->vertexs.size();i++){
                clip_polygon->addVertex(rectangle->vertexs[i].x,rectangle->vertexs[i].y);
            }
            clip_polygon->finish();
        }
        //clip_polygon->finish();
    }
}


void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    in_polygon->draw();
    rectangle->draw();
    clipping();
    clip_polygon->draw();
	glutSwapBuffers();
}

void reshape(int w,int h){
    width=(GLsizei)w;
    height=(GLsizei)h;
    oright=width;
    otop=height;
    in_polygon->resize(w,h);
    rectangle->resize(w,h);
    clip_polygon->resize(w,h);
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
            //cout << "clicked to " << in_polygon->vertexs.size() << endl
            //     << "width:" << width << "height:" << height << endl
            //     << "oright:" << oright << "otop:" << otop << endl
            //     << "x:" << x << "y:" << height-y << endl;
            in_polygon->addVertex(x,height-y);
            glutPostRedisplay();
        }
        else if(state == GLUT_UP){}
    }
    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            if(in_polygon->state&&rectangle->state&&to_clipping){
                in_polygon=new Polygon(width,height,1);
                rectangle=new Polygon(width,height,0);
                clip_polygon=new Polygon(width,height,2);
                to_clipping=false;
            }
            else{
                in_polygon->finish();
                cout << "finished!" << endl;
            }
            glutPostRedisplay();
        }
        else if(state == GLUT_UP){}
    }
    if (state == GLUT_UP && button == GLUT_WHEEL_UP){ 
        glutPostRedisplay();
    }
    if (state == GLUT_UP && button == GLUT_WHEEL_DOWN){
        glutPostRedisplay();
    }
}

void mouse_motion(int x, int y){
    if(in_polygon->state){
        if(!rectangle->state){
            rectangle->addVertex(x,height-y);
            rectangle->addVertex(x+1,height-y);
            rectangle->addVertex(x+1,height-y+1);
            rectangle->addVertex(x,height-y+1);
            rectangle->finish();
            get_min();
        }
        else{
            if(!to_clipping){
                rectangle->rectangle_change(x,height-y);
                get_min();
            }
        }
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key,int x,int y){
	switch(key){
        case 'c':
            to_clipping=true;
            glutPostRedisplay();
            break;
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
    delete in_polygon;
    delete rectangle;
    delete clip_polygon;
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
