#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include "scene.h"
#include "graphics.h"

map<int, TransformNode*> tMap; 
int gIdentifier;


TransformNode::TransformNode(TransformNode* p)
{
	this->parent = p;
	this->shapeNode = NULL;
	this->transform = new Matrix();
	tMap[gIdentifier] = this;
	this->identifier = gIdentifier;
	gIdentifier++;
}

TransformNode::TransformNode(TransformNode* p, ShapeNode* s, Matrix* t)
{
	this->parent = p;
	this->shapeNode = s;
	this->transform = t;
}


TransformNode::~TransformNode()
{
	set<TransformNode*>::iterator itr;
	for (itr = this->child.begin(); itr != this->child.end(); itr++) {
		delete* itr;
	}

	delete this->transform;
	delete this->shapeNode;
}


void TransformNode::translate(double deltaX, double deltaY)
{
	delete this->transform;
	Matrix* m1 = Matrix::translation(deltaX, deltaY);
	this->transform = m1;
}

void TransformNode::rotate(double theta)
{
	delete this->transform;
	Matrix* m1 = Matrix::rotation(theta);
	this->transform = m1;
}

void TransformNode::shear(double shearXY, double shearYX)
{
	delete this->transform;
	Matrix* m1 = Matrix::shearing(shearXY, shearYX);
	this->transform = m1;
}

void TransformNode::scale(double scaleX, double scaleY)
{
	delete this->transform;
	Matrix* m1 = Matrix::scaling(scaleX, scaleY);
	this->transform = m1;
}

void TransformNode::draw(bool displayHelpers) const
{
	glPushName(this->identifier);
	gPush();

	if (displayHelpers) {

	}

	if (shapeNode != NULL) {
		shapeNode->draw();	
	}
	
	set<TransformNode*>::iterator itr;
	for (itr = child.begin(); itr != child.end(); itr++) {
		(*itr)->draw(displayHelpers);
	}

	gPop();
	glPopName();
}

TransformNode* TransformNode::getParent() const 
{ 
   return this->parent;
}

void TransformNode::setParent(TransformNode* p) 
{ 
	this->parent = p;
}

void TransformNode::changeParent(TransformNode* newParent)
{
	this->parent = newParent;
}

void TransformNode::groupObjects(set<TransformNode*>& groupMembers)
{
	set<TransformNode*>::iterator itr;
	for (itr = groupMembers.begin(); itr != groupMembers.end(); itr++){
		this->child.insert(*itr);
	}
}

Matrix* TransformNode::getTransform() const
{
   return this->transform;
}

TransformNode* TransformNode::clone() const
{
   return NULL;
}

void TransformNode::addChild(TransformNode* child)
{
	this->child.insert(child);
}

void TransformNode::removeChild(TransformNode* child)
{
	this->child.erase(child);
}

TransformNode* TransformNode::firstChild() const
{
   return *this->child.begin();
}

TransformNode* TransformNode::lastChild() const
{
	return *(this->child.end()--);
}

TransformNode* TransformNode::nextChild(TransformNode* child) const
{
	set<TransformNode*>::iterator itr = this->child.find(child);
	itr++;
	return *itr;
}

TransformNode* TransformNode::previousChild(TransformNode* child) const
{
	set<TransformNode*>::iterator itr = this->child.find(child);
	itr--;
	return *itr;
}


void TransformNode::select() 
{
	setHighlight(true);
}

void TransformNode::deSelect() 
{
	setHighlight(false);
}

TransformNode* TransformNode::nodeLookup(int identifier)
{
   return tMap[identifier];
}


ShapeNode::ShapeNode(colorType c) 
{
	color = c;
	transformNode = NULL;
	highlighted = false;
}

void ShapeNode::setTransformNode(TransformNode* tn)
{
	transformNode = tn;
}

TransformNode* ShapeNode::getTransformNode()
{
	return transformNode;
}

Line::Line(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c), x0(xx0), y0(yy0), x1(xx1), y1(yy1)
{}


ShapeNode* Line::clone() const
{
	Line* line = new Line(this->x0, this->y0, this->x1, this->y1, this->color);
    return line;
}

void Line::draw() const
{
	setColor(this->color);
	drawLine(x0, y0, x1, y1);
}



Rectangle::Rectangle(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c), x0(xx0), y0(yy0), x1(xx1), y1(yy1)
{}


ShapeNode* Rectangle::clone()  const
{
	Rectangle* rect = new Rectangle(this->x0,this->y0, this->x1, this->y1, this->color);
    return rect;
}

void Rectangle::draw() const
{
	setColor(color);
	drawRectangle(x0, y0, x1, y1);
}

Circle::Circle(double ccX, double ccY, double r, colorType c) 
	: ShapeNode(c), cX(ccX), cY(ccY), r(r)

{}

ShapeNode* Circle::clone() const
{

	Circle* circle = new Circle(this->cX, this->cY, this->r, this->color);
    return circle;
}

void Circle::draw() const
{
	setColor(color);
	drawCircle(cX, cY, r);
}


Polygon::Polygon(const list<Vector*>& vs, colorType c) 
	: ShapeNode(c)
{
	for (auto itr = vs.begin(); itr != vs.end(); itr++) {
		vertices.push_back(new Vector(**itr));
	}

}

Polygon::~Polygon()
{
	for (auto itr = vertices.begin(); itr != vertices.end(); itr++) {
		delete *itr;
	}
}

ShapeNode* Polygon::clone() const
{
	Polygon* poly = new Polygon(vertices, color);
    return poly;
}

void Polygon::draw() const
{
	if (vertices.size() > 1) {
		setColor(color);
		drawPolygon(vertices);
	}
	
}

