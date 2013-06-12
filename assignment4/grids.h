/*  
    for computer graphics assignment03
    grids
    author: 李冠贤 10389043
    date: 2013-04-08 
*/

#include "GL/freeglut.h"
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

const int MAX=2000;

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
        Edge(){}
        Edge(Vertex v1,Vertex v2){start=v1;end=v2;}
        void draw(){
            glLineWidth(2.0);
            glBegin(GL_LINES);
            glColor3fv(cl_blue);
            glVertex2f(start.x,start.y);
            glVertex2f(end.x,end.y);
            glEnd();
        }
};

class Grid{
    public:
        Vertex v1,v2,v3,v4;
        Edge e1,e2,e3,e4;
        int u,color; 
        Grid(){u=0;color=0;}
        Grid(const Grid &other){
            v1=other.v1;v2=other.v2;v3=other.v3;v4=other.v4;
            e1=other.e1;e2=other.e2;e3=other.e3;e4=other.e4;
            u=other.u;color=other.color;
        }
        void setup(Vertex a1,Vertex a2,Vertex a3,Vertex a4,int uu,int c){
            v1=a1;v2=a2;v3=a3;v4=a4;u=uu;color=c;
            e1.start=v1;e1.end=v2;
            e2.start=v2;e2.end=v3;
            e3.start=v3;e3.end=v4;
            e4.start=v4;e4.end=v1;
        }
        void draw(){
            e1.draw();e2.draw();e3.draw();e4.draw();
            glBegin(GL_QUADS);
            if(color)
                glColor3fv(cl_red);
            else
                glColor3fv(cl_white);
            glVertex2f(v1.x,v1.y);
            glVertex2f(v2.x,v2.y);
            glVertex2f(v3.x,v3.y);
            glVertex2f(v4.x,v4.y);
            glEnd();
        }
};

class Grids{
    public:
        Grid grids[MAX][MAX];
        int width,height,uwidth,uheight;
        Grids(int w,int h,int u){
            width=w;
            height=h;
            generate(w,h,u);
        }
        void generate(int w,int h,int u){
            int x1,y1,x2,y2,x3,y3,x4,y4;
            x1=0;y1=0;
            x2=0;y2=u;
            x3=u;y3=u;
            x4=u;y4=0;
            for(int i=0;i<w;i++){
                for(int j=0;j<h;j++){
                    Vertex v1(x1+i*u,y1+j*u);
                    Vertex v2(x2+i*u,y2+j*u);
                    Vertex v3(x3+i*u,y3+j*u);
                    Vertex v4(x4+i*u,y4+j*u);
                    grids[i][j].setup(v1,v2,v3,v4,u,0);
                }
            }
        }
        void draw(){
            for(int i=0;i<width;i++){
                for(int j=0;j<height;j++){
                    grids[i][j].draw();
                }
            }
        }
        void resize(int w,int h,int u){
            width=w;
            height=h;
            generate(w,h,u);
        }
};

struct V_order{
    Vertex v;
    GLfloat d;
};

struct less_than_key{
    inline bool operator()(const V_order& struct1,const V_order& struct2){
        return (struct1.d < struct2.d);
    }
};


class Triangle{
    public:
        Vertex v1,v2,v3;
        vector<V_order> v;
        int count;
        Triangle(){count=0;}
        void set_color(Grids *grids){
            if(count==1){
                grids->grids[(int)v1.x][(int)v1.y].color=1;
            }
            else if(count==2){
                grids->grids[(int)v1.x][(int)v1.y].color=1;
                grids->grids[(int)v2.x][(int)v2.y].color=1;
            }
            else if(count==3){
                grids->grids[(int)v1.x][(int)v1.y].color=1;
                grids->grids[(int)v2.x][(int)v2.y].color=1;
                grids->grids[(int)v3.x][(int)v3.y].color=1;
            }
        }
        void self_sort(){
            if(count==3){
                v.clear();
                V_order vv1;
                vv1.v=v1;vv1.d=v1.y;
                V_order vv2;
                vv2.v=v2;vv2.d=v2.y;
                V_order vv3;
                vv3.v=v3;vv3.d=v3.y;
                v.push_back(vv1);
                v.push_back(vv2);
                v.push_back(vv3);
            }
            sort(v.begin(),v.end(),less_than_key());
        }
        GLfloat distance(int x1,int y1,int x2,int y2){
            return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
        }
        void rasterization(Grids *grids){
            self_sort();
            int bottomx=(int)v[0].v.x;
            int bottomy=(int)v[0].v.y;
            int midx=(int)v[1].v.x;
            int midy=(int)v[1].v.y;
            int topx=(int)v[2].v.x;
            int topy=(int)v[2].v.y;
            cout << "=====" << endl;
            cout << bottomx << ":" << bottomy << endl;
            cout << midx << ":" << midy << endl;
            cout << topx << ":" << topy << endl;
            cout << "=====" << endl;
            if(bottomy!=midy){
                /*
                   1.-----o------o-----------------------
                   ------*-*-----*-**--------------------
                   ------*--*-----*--**------------------
                   -----*----o----*-----o----------------
                   -----*---*------*--*------------------
                   -----*-*--------*-*-------------------
                   -----o-----------o--------------------
                   --------------------------------------
                */
                if(bottomx<midx){
                    GLfloat m1=acos((topy-bottomy)/distance(topx,topy,bottomx,bottomy));
                    GLfloat m2=acos((midy-bottomy)/distance(midx,midy,bottomx,bottomy));
                    GLfloat xl=bottomx;
                    GLfloat xr=bottomx;
                    for(int y=bottomy;y<=midy;y++){
                        scan(grids,xl,xr,y);
                        if(topx>bottomx)
                            xl+=tan(m1);
                        else
                            xl-=tan(m1);
                        xr+=tan(m2);
                    }
                    m2=acos((topy-midy)/distance(topx,topy,midx,midy));
                    xr=midx;
                    for(int y=midy;y<=topy;y++){
                        scan(grids,xl,xr,y);
                        if(topx>bottomx)
                            xl+=tan(m1);
                        else
                            xl-=tan(m1);
                        xr-=tan(m2);
                    }
                }
                /*
                   2.-----o----------o-------------------
                   ------*-*-------*-*-------------------
                   ------*-*------*---*------------------
                   -----0---*----o*---*------------------
                   ------*--*------*--*------------------
                   -------**-------*-*-------------------
                   ---------o--------o-------------------
                   --------------------------------------
                */
                else if(bottomx>midx){
                    GLfloat m1=acos((midy-bottomy)/distance(midx,midy,bottomx,bottomy));
                    GLfloat m2=acos((topy-bottomy)/distance(topx,topy,bottomx,bottomy));
                    GLfloat xl=bottomx;
                    GLfloat xr=bottomx;
                    for(int y=bottomy;y<=midy;y++){
                        scan(grids,xl,xr,y);
                        xl-=tan(m1);
                        if(topx>bottomx)
                            xr+=tan(m2);
                        else
                            xr-=tan(m2);
                    }
                    m1=acos((topy-midy)/distance(topx,topy,midx,midy));
                    xl=midx;
                    for(int y=midy;y<=topy;y++){
                        scan(grids,xl,xr,y);
                        xl+=tan(m1);
                        if(topx>bottomx)
                            xr+=tan(m2);
                        else
                            xr-=tan(m2);
                    }
                }
                else{
                /*
                   3.--o--------------o------------------
                   -----*-----------**-------------------
                   ------**-------*----------------------
                   ---------o----o*----------------------
                   ---------*----*-----------------------
                   ---------*----*-----------------------
                   ---------o----o-----------------------
                   --------------------------------------
                */
                    if(topx>bottomx){
                        GLfloat m1=acos((midy-bottomy)/distance(midx,midy,bottomx,bottomy));
                        GLfloat m2=acos((topy-bottomy)/distance(topx,topy,bottomx,bottomy));
                        GLfloat xl=bottomx;
                        GLfloat xr=bottomx;
                        for(int y=bottomy;y<=midy;y++){
                            scan(grids,xl,xr,y);
                            xl-=tan(m1);
                            if(topx>bottomx)
                                xr+=tan(m2);
                            else
                                xr-=tan(m2);
                        }
                        m1=acos((topy-midy)/distance(topx,topy,midx,midy));
                        xl=midx;
                        for(int y=midy;y<=topy;y++){
                            scan(grids,xl,xr,y);
                            xl+=tan(m1);
                            if(topx>bottomx)
                                xr+=tan(m2);
                            else
                                xr-=tan(m2);
                        }
                    }
                    else{
                        GLfloat m1=acos((topy-bottomy)/distance(topx,topy,bottomx,bottomy));
                        GLfloat m2=acos((midy-bottomy)/distance(midx,midy,bottomx,bottomy));
                        GLfloat xl=bottomx;
                        GLfloat xr=bottomx;
                        for(int y=bottomy;y<=midy;y++){
                            scan(grids,xl,xr,y);
                            if(topx>bottomx)
                                xl+=tan(m1);
                            else
                                xl-=tan(m1);
                            xr+=tan(m2);
                        }
                        m2=acos((topy-midy)/distance(topx,topy,midx,midy));
                        xr=midx;
                        for(int y=midy;y<=topy;y++){
                            scan(grids,xl,xr,y);
                            if(topx>bottomx)
                                xl+=tan(m1);
                            else
                                xl-=tan(m1);
                            xr-=tan(m2);
                        }
                    }
                }
            }
            /*
               4.------------o-----------------------
               ------------*---*---------------------
               --------------------------------------
               -----------*------*-------------------
               --------------------------------------
               ---------*----------*-----------------
               --------o--*---*---*--o---------------
               --------------------------------------
             */
            else{
                GLfloat xl=bottomx<midx?bottomx:midx;
                GLfloat xr=bottomx>midx?bottomx:midx;
                GLfloat m1=acos((topy-bottomy)/distance(topx,topy,xl,bottomy));
                GLfloat m2=acos((topy-bottomy)/distance(topx,topy,xr,bottomy));
                for(int y=bottomy;y<=topy;y++){
                    scan(grids,xl,xr,y);
                    xl+=tan(m1);
                    xr-=tan(m2);
                }
            }
        }
        void scan(Grids *grids,int xl,int xr,int y){
            for(int x=xl;x<=xr;x++){
                grids->grids[x][y].color=1;
            }
        }
};
