/*  
    structure 
    vertex, edge, polygon
    author: 李冠贤 10389043
    date: 2013-04-08 
*/

#include "GL/freeglut.h"
#include <iostream>
#include <vector>

using namespace std;

//颜色向量
GLfloat cl_white[]={1.0,1.0,1.0};
GLfloat cl_black[]={0.0,0.0,0.0};
GLfloat cl_red[]={1.0,0.0,0.0};
GLfloat cl_green[]={0.0,1.0,0.0};
GLfloat cl_blue[]={0.0,0.0,1.0};
GLfloat cl_yellow[]={1.0,1.0,0.0};
GLfloat cl_purple[]={1.0,0.0,1.0};

class Vertex{
    public:
        GLfloat x;
        GLfloat y;
        Vertex(){x=0;y=0;}
        Vertex(int xx,int yy){x=xx;y=yy;}
};

class Edge{
    public:
        Vertex start;
        Vertex end;
        Edge(Vertex v1,Vertex v2){start=v1;end=v2;}
};

class Polygon{
    public:
        vector<Vertex> vertexs;
        vector<Edge> edges;
        int type,width,height;
        bool state;
        Polygon(int w,int h,int t){width=w;height=h;type=t;state=false;vertexs.clear();edges.clear();}
        void addVertex(int x,int y){
            if(!state){
                if(vertexs.empty()){
                    Vertex v(x,y);
                    vertexs.push_back(v);
                }
                else{
                    Vertex v1(x,y);
                    Vertex v2=vertexs.back();
                    Edge e(v2,v1);
                    edges.push_back(e);
                    vertexs.push_back(v1);
                }
            }
        }
        void finish(){
            Edge e(vertexs.back(),vertexs.front());
            edges.push_back(e);
            state=true;
        }
        void draw(){
            glPointSize(9.0);
            for(int i=0;i<vertexs.size();i++){
                glBegin(GL_POINTS);
                if(type==0) glColor3fv(cl_yellow);
                else if(type==1) glColor3fv(cl_purple);
                else if(type==2) glColor3fv(cl_green);
                glVertex2f(vertexs[i].x,vertexs[i].y);
                glEnd();
            }
            glLineWidth(2.0);
            for(int i=0;i<edges.size();i++){
                glBegin(GL_LINES);
                if(type==0) glColor3fv(cl_white);
                else if(type==1) glColor3fv(cl_red);
                else if(type==2) glColor3fv(cl_blue);
                glVertex2f(edges[i].start.x,edges[i].start.y);
                glVertex2f(edges[i].end.x,edges[i].end.y);
                glEnd();
            }
        }
        void resize(int w,int h){
            for(int i=0;i<vertexs.size();i++){
                vertexs[i].x=vertexs[i].x/(GLfloat)width*w;
                vertexs[i].y=vertexs[i].y/(GLfloat)height*h;
            }
            for(int i=0;i<edges.size();i++){
                edges[i].start.x=edges[i].start.x/(GLfloat)width*w;
                edges[i].start.y=edges[i].start.y/(GLfloat)height*h;
                edges[i].end.x=edges[i].end.x/(GLfloat)width*w;
                edges[i].end.y=edges[i].end.y/(GLfloat)height*h;
            }
            width=w;
            height=h;
        }
        void rectangle_change(int x,int y){
            vertexs[1].x=x; 
            vertexs[2].x=x; 
            vertexs[2].y=y; 
            vertexs[3].y=y; 
            edges[0].end.x=x;
            edges[1].start.x=x;
            edges[1].end.x=x;
            edges[1].end.y=y;
            edges[2].start.x=x;
            edges[2].start.y=y;
            edges[2].end.y=y;
            edges[3].start.y=y;
        }
};
