#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include "scene.h"
#include "graphics.h"

map<int, TransformNode*> tMap;
int gIdentifier = 0;


TransformNode::TransformNode(TransformNode* p)
{
	this->parent = p;
	if(p){ 
		p->addChild(this); 
	}
	this->shapeNode = NULL;
	this->transform = new Matrix();
	this->identifier = gIdentifier;
	this->selected = false;

	// stores a mapping of identifier -> transformNode
	tMap[gIdentifier] = this;
	gIdentifier++;
	
}

TransformNode::TransformNode(TransformNode* p, ShapeNode* s, Matrix* t)
{
	this->parent = p;
	if (p) {
		p->addChild(this);
	}
	this->shapeNode = s;
	this->transform = t;
	this->identifier = gIdentifier;
	this->selected = false;

	// stores a mapping of identifier -> transformNode
	tMap[gIdentifier] = this;
	gIdentifier++;
	
}


TransformNode::~TransformNode()
{
	// deletes each child attached to (this) transformNode
	for (set<TransformNode*>::iterator itr = this->child.begin(); itr != this->child.end(); itr++) {
		delete* itr;
	}

	// deletes matrix of (this) transformNode
	delete this->transform;
}


void TransformNode::translate(double deltaX, double deltaY)
{
	Matrix* m1 = Matrix::translation(deltaX, deltaY);
	this->transform = m1->multiply(transform);
}

void TransformNode::rotate(double theta)
{
	Matrix* m1 = Matrix::rotation(theta);
	this->transform = m1->multiply(transform);
}

void TransformNode::shear(double shearXY, double shearYX)
{
	Matrix* m1 = Matrix::shearing(shearXY, shearYX);
	this->transform = m1->multiply(transform);
}

void TransformNode::scale(double scaleX, double scaleY)
{
	Matrix* m1 = Matrix::scaling(scaleX, scaleY);
	this->transform = m1->multiply(transform);
}

void TransformNode::draw(bool displayHelpers) const
{
	bool highlighted = getHighlight();
	if (selected) setHighlight(true);
	glPushName(this->identifier);
	gPush(this->transform);

	if (shapeNode) {
		shapeNode->draw();	
	}else if(displayHelpers){
		drawTransformGismo();
	}
	for (set<TransformNode*>::iterator itr = child.begin(); itr != child.end(); itr++) {
		(*itr)->draw(displayHelpers);
	}

	gPop();
	setHighlight(false);
	glPopName();
}

TransformNode* TransformNode::getParent() const 
{ 
   return this->parent;
}

void TransformNode::setParent(TransformNode* p) 
{ 
	if (parent) {
		parent->removeChild(this);
	}
	this->parent = p;
	p->addChild(this);
}

void TransformNode::changeParent(TransformNode* newParent)
{
	
	parent->removeChild(this);
	setParent(newParent);
	newParent->addChild(this);
}

void TransformNode::groupObjects(set<TransformNode*>& groupMembers)
{
	double rectSize = 12.5;
	ShapeNode* rect = new Rectangle(-rectSize, -rectSize, rectSize, rectSize, YELLOW);

	TransformNode* newNode;
	if (this){
		newNode = new TransformNode(this, rect, transform);
	}else{
		newNode = new TransformNode(this, rect, new Matrix());
	}
	rect->setTransformNode(newNode);
	
	newNode->translate(5, 5);
	for (set<TransformNode*>::iterator itr = groupMembers.begin(); itr != groupMembers.end(); itr++){
		(*itr)->changeParent(newNode);
		(*itr)->translate(-5, -5);
	}
}

Matrix* TransformNode::getTransform() const
{
   return this->transform;
}

TransformNode* TransformNode::clone() const
{
   return new TransformNode(parent, shapeNode, transform);
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
	selected = true;
	for (set<TransformNode*>::iterator itr = this->child.begin(); itr != this->child.end(); itr++) {
		(*itr)->select();
	}
}

void TransformNode::deSelect() 
{
	selected = false;
	for (set<TransformNode*>::iterator itr = this->child.begin(); itr != this->child.end(); itr++) {
		(*itr)->deSelect();
	}
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
	for (list<Vector*>::const_iterator itr = vs.begin(); itr != vs.end(); itr++) {
		vertices.push_back(new Vector(**itr));
	}

}

Polygon::~Polygon()
{
	for (list<Vector*>::iterator itr = vertices.begin(); itr != vertices.end(); itr++) {
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

