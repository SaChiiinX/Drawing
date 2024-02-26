#include "graphics.h"
#include "drawing.h"

#include <GL/glut.h>

#include <cfloat>
#include <cmath>

using namespace std;

const colorType HIGHLIGHT_COLOR = WHITE;

const double TRANSFORM_GISMO_SCALE = 25.0;


bool highlight = false;

bool getHighlight()
{
  return highlight;
}

void setHighlight(bool h)
{
  if (h) setColor(HIGHLIGHT_COLOR);
  highlight = h;
}


colorType intToColor(int item)
{
  switch ( item ) {
    case 0 : return BLACK   ;
    case 1 : return BLUE    ;
    case 2 : return GREEN   ;
    case 3 : return CYAN    ;
    case 4 : return RED     ;
    case 5 : return MAGENTA ;
    case 6 : return YELLOW  ;
    case 7 : return WHITE   ;
    case 8 : return GRAY    ;
   default : return BLACK   ;
  }
}


void setColor( colorType color )
{
  if (!highlight)
    switch ( color )
    {
      case BLACK   : glColor3f( 0.0, 0.0, 0.0 ); break;
      case BLUE    : glColor3f( 0.0, 0.0, 1.0 ); break;
      case GREEN   : glColor3f( 0.0, 1.0, 0.0 ); break;
      case CYAN    : glColor3f( 0.0, 1.0, 1.0 ); break;
      case RED     : glColor3f( 1.0, 0.0, 0.0 ); break;
      case MAGENTA : glColor3f( 1.0, 0.0, 1.0 ); break;
      case YELLOW  : glColor3f( 1.0, 1.0, 0.0 ); break;
      case WHITE   : glColor3f( 1.0, 1.0, 1.0 ); break;
      case GRAY    : glColor3f( 0.5, 0.5, 0.5 ); break;
    }
}



TransformStack transformStack;

TransformStack::TransformStack()
{
    //mStack.push_front(new Matrix());
}

void TransformStack::push(Matrix* transform)
{
    //if (mStack.empty()) {
    //    mStack.push_front(new Matrix());
    //}
    //else {
    //    mStack.push_front(top()->multiply(transform));
    //}
}

void TransformStack::pop()
{
    //if (mStack.size() > 1) {
    //    mStack.pop_front();
    //}
}

Matrix* TransformStack::top()
{
    //return mStack.front();
    return NULL;
}


void gPush(Matrix* transform)
{
    //transformStack.push(transform);
}

void gPop()
{
    //transformStack.pop();
}


void drawLine(double x0, double y0, double x1, double y1)
{
    Vector* p0 = new Vector(x0, y0);
    Vector* p1 = new Vector(x1, y1);
    drawLine(p0, p1);
    delete p0;
    delete p1;
}

void drawLine(Vector* p0, Vector* p1)
{
    
    double x0 = (*p0)[0];
    double y0 = (*p0)[1];
    double x1 = (*p1)[0];
    double y1 = (*p1)[1];
    gPop();
    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);
    glEnd();
    glFlush();
}

void drawRectangle(double x0, double y0, double x1, double y1)
{
    Vector* p0 = new Vector(x0, y0);
    Vector* p1 = new Vector(x0, y1);
    Vector* p2 = new Vector(x1, y1);
    Vector* p3 = new Vector(x1, y0);
    drawLine(p0,p1);
    drawLine(p1,p2);
    drawLine(p2, p3);
    drawLine(p3, p0);
    delete p0;
    delete p1;
    delete p2;
    delete p3;
}

void drawCircle(double x0, double y0, double x1, double y1)
{

}

void drawCircle(double cX, double cY, double radius)
{
    double num_points = TWO_PI * radius;
    Vector* v1;
    Vector* v2;
    for (int i = 1; i <= 360; i++) {
        double angle1 = TWO_PI * i / 360;
        double angle2 = TWO_PI * (i - 1) / 360;
        v1 = new Vector(cX+radius*cos(angle1), cY+radius*sin(angle1));
        v2 = new Vector(cX+radius*cos(angle2), cY+radius*sin(angle2));
        drawLine(v2, v1);
    }
    delete v1;
    delete v2;
}

void drawPolygon(const list<Vector*>& vertices, bool close)
{
    if (vertices.size() <= 1) {
        return;
    }

    Vector* first = *vertices.begin();
    Vector* v1;
    Vector* v2;
    auto itr1 = vertices.begin();
    auto itr2 = vertices.begin();
    for (itr2++; itr2 != vertices.end(); itr2++){
        v1 = *itr1;
        v2 = *itr2;
        drawLine(v1, v2);
        itr1++;
    }
    if (close) {
        drawLine(first, v2);
    }
}

void drawTransformGismo()
{
  double halfSize = TRANSFORM_GISMO_SCALE/2.0;
  drawRectangle(-halfSize,-halfSize,halfSize,halfSize);
}
