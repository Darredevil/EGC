	#pragma once
#include "Circle2D.h"
#include "Polygon2D.h"
#include "Visual2D.h"
#include "DrawingWindow.h"
#include <math.h>

#define PI 3.14159265358979323846

#define E1_LENGTH 25
#define E2_LENGTH1 30
#define E2_LENGTH2 10
#define E3_LENGTH 20

#define SCREEN_HEIGHT 650
#define SCREEN_WIDTH 1300

class Enemy1
{
public:
	float x, y;
	Polygon2D *body1, *body2;
	float sideLength;
	float angle, radius;
	int points;

public:

	Enemy1(Point2D _centre){
		sideLength = E1_LENGTH;	angle = PI/6;	radius = sideLength * sqrt(2);
		x = _centre.x;
		y = _centre.y;
		points = 20;
		body1 = new Polygon2D(Color(1, 0.3, 0.7), false);
		body2 = new Polygon2D(Color(1, 0.3, 0.7), true);
		body1->addPoint(Point2D(x - sideLength, y - sideLength));
		body1->addPoint(Point2D(x - sideLength, y + sideLength));
		body1->addPoint(Point2D(x + sideLength, y + sideLength));
		body1->addPoint(Point2D(x + sideLength, y - sideLength));

		body2->addPoint(Point2D(x - sideLength, y));
		body2->addPoint(Point2D(x, y + sideLength));
		body2->addPoint(Point2D(x + sideLength, y));
		body2->addPoint(Point2D(x, y - sideLength));
	}

	void draw(Visual2D *v){
		DrawingWindow::addObject2D_to_Visual2D(body1, v);
		DrawingWindow::addObject2D_to_Visual2D(body2, v);	
	}

	void erase(Visual2D *v){
		DrawingWindow::removeObject2D_from_Visual2D(body1, v);
		DrawingWindow::removeObject2D_from_Visual2D(body2, v);
	}

	void rotateAnimation(float _u){
		Transform2D::rotateAroundPoint(body1, Point2D(x, y), _u);
		Transform2D::rotateAroundPoint(body2, Point2D(x, y), -_u);
	}

	void rotate(float _u) {
		angle += _u;
		if(angle >= 2 * PI)
			angle -= 2 * PI;
	}

	void moveForward(float _speed){
		int tempx = x + _speed * cos(angle);
		int tempy = y + _speed * sin(angle);
		
		if(tempy > radius + 10 && tempx > radius && tempy < SCREEN_HEIGHT - 50 - radius && tempx < SCREEN_WIDTH - 20 - radius){
			y += _speed * sin(angle);
			x += _speed * cos(angle);
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(_speed * cos(angle), _speed * sin(angle));
			Transform2D::applyTransform_o(body1);
			Transform2D::applyTransform_o(body2);
		} else {
			if(tempy <= radius + 10 || tempy >= SCREEN_HEIGHT - 50 - radius){
				angle = 2 * PI - angle;
				if(angle > 2 * PI)
					angle -= 2 * PI;
				if(angle < - 2 * PI)
					angle += 2 * PI;
			} 
			if(tempx <= radius || tempx >= SCREEN_WIDTH - 20 - radius){
				angle = angle + PI / 2;
				angle = 2 * PI - angle;
				angle = angle - PI / 2;
				if(angle > 2 * PI)
					angle -= 2 * PI;
				if(angle < - 2 * PI)
					angle += 2 * PI;
			}
		}
	}

	~Enemy1(){}

};


class Enemy2
{
public:
	Polygon2D *body1, *body2;
	Point2D *c1, *c2;
	float x, y;
	float lengthY, lengthX;
	float angle, radius;
	int points;

public:
	Enemy2(Point2D _centre){
		lengthY = E2_LENGTH1;	lengthX = E2_LENGTH2;	angle = 2 * PI / 3;
		x = _centre.x;
		y = _centre.y;
		radius = lengthY * sqrt(2);
		points = 40;
		body1 = new Polygon2D(Color(0, 0, 1), true);
		body2 = new Polygon2D(Color(1, 0, 0), true);

		body1->addPoint(Point2D(x - lengthY, y - lengthY));
		body1->addPoint(Point2D(x - lengthY, y + lengthX));
		body1->addPoint(Point2D(x + lengthX, y + lengthX));
		body1->addPoint(Point2D(x + lengthX, y - lengthY));

		body2->addPoint(Point2D(x - lengthX, y - lengthX));
		body2->addPoint(Point2D(x - lengthX, y + lengthY));
		body2->addPoint(Point2D(x + lengthY, y + lengthY));
		body2->addPoint(Point2D(x + lengthY, y - lengthX));
	}

	void erase(Visual2D *v){
		DrawingWindow::removeObject2D_from_Visual2D(body1, v);
		DrawingWindow::removeObject2D_from_Visual2D(body2, v);
	}

	void draw(Visual2D *v){
		DrawingWindow::addObject2D_to_Visual2D(body1, v);
		DrawingWindow::addObject2D_to_Visual2D(body2, v);	
	}

	void rotateAnimation(float _u){
		Transform2D::rotateAroundPoint(body1, Point2D(x, y), _u);
		Transform2D::rotateAroundPoint(body2, Point2D(x, y), -_u);
	}

	void rotate(float _u) {
		angle += _u;
		if(angle >= 2 * PI)
			angle -= 2 * PI;
	}
	
	void moveForward(float _speed){
		int tempx = x + _speed * cos(angle);
		int tempy = y + _speed * sin(angle);
		
		if(tempy > radius + 10 && tempx > radius && tempy < SCREEN_HEIGHT - 50 - radius && tempx < SCREEN_WIDTH - 20 - radius){
			y += _speed * sin(angle);
			x += _speed * cos(angle);
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(_speed * cos(angle), _speed * sin(angle));
			Transform2D::applyTransform_o(body1);
			Transform2D::applyTransform_o(body2);
		} else {
			if(tempy <= radius + 10 || tempy >= SCREEN_HEIGHT - 50 - radius){
				angle = 2 * PI - angle;
				if(angle > 2 * PI)
					angle -= 2 * PI;
				if(angle < - 2 * PI)
					angle += 2 * PI;
			} 
			if(tempx <= radius || tempx >= SCREEN_WIDTH - 20 - radius){
				angle = angle + PI / 2;
				angle = 2 * PI - angle;
				angle = angle - PI / 2;
				if(angle > 2 * PI)
					angle -= 2 * PI;
				if(angle < - 2 * PI)
					angle += 2 * PI;
			}
		}
	}

	~Enemy2(){};
	
};

class Enemy3
{
public:
	Rectangle2D *sq1, *sq2, *sq3, *sq4;
	Rectangle2D *sq11, *sq22, *sq33, *sq44;
	float x, y;
	float sideLength, angle, radius;
	int points;

public:
	Enemy3(Point2D _centre){
		x = _centre.x;
		y = _centre.y;
		sideLength = E3_LENGTH;	angle = PI / 9;	radius = sideLength * sqrt(5);
		points = 30;
		sq1 = new Rectangle2D(Point2D(x, y), sideLength, sideLength, Color(0, 0, 0.8), true);
		sq2 = new Rectangle2D(Point2D(x - sideLength, y), sideLength, sideLength, Color(0, 0, 0.8), true);
		sq3 = new Rectangle2D(Point2D(x - sideLength, y - sideLength), sideLength, sideLength, Color(0, 0, 0.8), true);
		sq4 = new Rectangle2D(Point2D(x - sideLength, y - 2 * sideLength), sideLength, sideLength, Color(0, 0, 0.8), true);

		sq11 = new Rectangle2D(Point2D(x, y), sideLength, sideLength, Color(0, 0.75, 1), false);
		sq22 = new Rectangle2D(Point2D(x - sideLength, y), sideLength, sideLength, Color(0, 0.75, 1), false);
		sq33 = new Rectangle2D(Point2D(x - sideLength, y - sideLength), sideLength, sideLength, Color(0, 0.75, 1), false);
		sq44 = new Rectangle2D(Point2D(x - sideLength, y - 2 * sideLength), sideLength, sideLength, Color(0, 0.75, 1), false);
	}

	void draw(Visual2D *v){
		DrawingWindow::addObject2D_to_Visual2D(sq11, v);
		DrawingWindow::addObject2D_to_Visual2D(sq22, v);
		DrawingWindow::addObject2D_to_Visual2D(sq33, v);	
		DrawingWindow::addObject2D_to_Visual2D(sq44, v);

		DrawingWindow::addObject2D_to_Visual2D(sq1, v);
		DrawingWindow::addObject2D_to_Visual2D(sq2, v);
		DrawingWindow::addObject2D_to_Visual2D(sq3, v);	
		DrawingWindow::addObject2D_to_Visual2D(sq4, v);	
	}

	void erase(Visual2D *v){
		DrawingWindow::removeObject2D_from_Visual2D(sq11, v);
		DrawingWindow::removeObject2D_from_Visual2D(sq22, v);
		DrawingWindow::removeObject2D_from_Visual2D(sq33, v);	
		DrawingWindow::removeObject2D_from_Visual2D(sq44, v);

		DrawingWindow::removeObject2D_from_Visual2D(sq1, v);
		DrawingWindow::removeObject2D_from_Visual2D(sq2, v);
		DrawingWindow::removeObject2D_from_Visual2D(sq3, v);	
		DrawingWindow::removeObject2D_from_Visual2D(sq4, v);	
	}

	void rotateAnimation(float _u){
		Transform2D::rotateAroundPoint(sq1, Point2D(x, y), _u);
		Transform2D::rotateAroundPoint(sq2, Point2D(x, y), _u);
		Transform2D::rotateAroundPoint(sq3, Point2D(x, y), _u);
		Transform2D::rotateAroundPoint(sq4, Point2D(x, y), _u);
		Transform2D::rotateAroundPoint(sq11, Point2D(x, y), _u);
		Transform2D::rotateAroundPoint(sq22, Point2D(x, y), _u);
		Transform2D::rotateAroundPoint(sq33, Point2D(x, y), _u);
		Transform2D::rotateAroundPoint(sq44, Point2D(x, y), _u);
	}

	void rotate(float _u) {
		angle += _u;
		if(angle >= 2 * PI)
			angle -= 2 * PI;
	}
	void moveForward(float _speed){
		int tempx = x + _speed * cos(angle);
		int tempy = y + _speed * sin(angle);
		if(tempy > radius + 10 && tempx > radius && tempy < SCREEN_HEIGHT - 50 - radius && tempx < SCREEN_WIDTH - 20 - radius){
			y += _speed * sin(angle);
			x += _speed * cos(angle);
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(_speed * cos(angle), _speed * sin(angle));
			Transform2D::applyTransform_o(sq1); Transform2D::applyTransform_o(sq3);
			Transform2D::applyTransform_o(sq2); Transform2D::applyTransform_o(sq4);
			Transform2D::applyTransform_o(sq11); Transform2D::applyTransform_o(sq33);
			Transform2D::applyTransform_o(sq22); Transform2D::applyTransform_o(sq44);
		} else {
			if(tempy <= radius + 10 || tempy >= SCREEN_HEIGHT - 50 - radius){
				angle = 2 * PI - angle;
				if(angle > 2 * PI)
					angle -= 2 * PI;
				if(angle < - 2 * PI)
					angle += 2 * PI;
			} 
			if(tempx <= radius || tempx >= SCREEN_WIDTH - 20 - radius){
				angle = angle + PI / 2;
				angle = 2 * PI - angle;
				angle = angle - PI / 2;
				if(angle > 2 * PI)
					angle -= 2 * PI;
				if(angle < - 2 * PI)
					angle += 2 * PI;
			}
		}
	}

	~Enemy3(){}
};