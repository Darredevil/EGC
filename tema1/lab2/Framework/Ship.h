#pragma once
#include "Circle2D.h"
#include "Polygon2D.h"
#include "Visual2D.h"
#include "DrawingWindow.h"
#include "Enemies.h"
#include "Point2D.h"

#define SCREEN_HEIGHT 650
#define SCREEN_WIDTH 1300


class Ship
{
public:
	float x, y;
	float startX, startY;
	float radius, shipAngle;
	bool activatedDrill;
	Circle2D *body;
	Polygon2D *dir;
	Polygon2D *drill;

public:
	Ship(float _startX, float _startY, float _radius){
		startX = _startX;	x = _startX;
		startY = _startY;	y = _startY;
		radius = _radius;
		shipAngle = 0;		activatedDrill = false;

		dir = new Polygon2D(Color(0, 0, 1), false);
		dir->addPoint(Point2D(x - 26, y));
		dir->addPoint(Point2D(x - 10, y - 22));
		dir->addPoint(Point2D(x + 10, y - 22));
		dir->addPoint(Point2D(x + 26, y));
		dir->addPoint(Point2D(x + 22, y));
		dir->addPoint(Point2D(x + 3, y + 25));
		dir->addPoint(Point2D(x + 14, y));
		dir->addPoint(Point2D(x - 14, y));
		dir->addPoint(Point2D(x - 3, y + 25));
		dir->addPoint(Point2D(x - 22, y));
		Transform2D::rotateAroundPoint(dir, Point2D(x, y), -PI/2);

		body = new Circle2D(Point2D(startX, startY), radius, Color(0.5, 1, 0), true);

		drill = new Polygon2D(Color(1, 0, 0), true);
		drill->addPoint(Point2D(startX + radius, startY + radius/2));
		drill->addPoint(Point2D(startX + 1.5 * radius, startY + 3*radius/8));
		drill->addPoint(Point2D(startX + 2 * radius, startY + radius/4));
		drill->addPoint(Point2D(startX + 2.5 * radius, startY + radius/8));
		drill->addPoint(Point2D(startX + 3 * radius, startY));
		drill->addPoint(Point2D(startX + 2.5 * radius, startY - radius/8));
		drill->addPoint(Point2D(startX + 2 * radius, startY - radius/4));
		drill->addPoint(Point2D(startX + 1.5 * radius, startY - 3*radius/8));
		drill->addPoint(Point2D(startX + radius, startY - radius/2));
		
		
	}

	void toggleDrill(Visual2D *v){
		if(activatedDrill == false){
			DrawingWindow::addObject2D_to_Visual2D(drill, v);
			activatedDrill = true;
		} else {
			DrawingWindow::removeObject2D_from_Visual2D(drill, v);
			activatedDrill = false;
		}
	}


	void drawShip(Visual2D *v){
		DrawingWindow::addObject2D_to_Visual2D(dir, v);
		DrawingWindow::addObject2D_to_Visual2D(body, v);		
	}

	void rotate(string s, float u){
		if(s == "left"){
			shipAngle += u;
			Transform2D::rotateAroundPoint(dir, Point2D(x, y), u); 
			Transform2D::rotateAroundPoint(drill, Point2D(x, y), u);
		} else if(s == "right"){
			shipAngle -= u;
			Transform2D::rotateAroundPoint(dir, Point2D(x, y), -u); 
			Transform2D::rotateAroundPoint(drill, Point2D(x, y), -u);
		}
	}

	void moveForward() {
		float units;
		if(activatedDrill == false) units = 10;
		if(activatedDrill == true) units = 6;
		int tempx = x + units * cos(shipAngle);
		int tempy = y + units * sin(shipAngle);
		if(tempy > radius + 10 && tempx > radius && tempy < SCREEN_HEIGHT - 50 - radius && tempx < SCREEN_WIDTH - 20 - radius){
			Transform2D::loadIdentityMatrix();
			y += units * sin(shipAngle);
			x += units * cos(shipAngle);
			Transform2D::translateMatrix(units * cos(shipAngle), units * sin(shipAngle));
			Transform2D::applyTransform_o(dir);
			Transform2D::applyTransform_o(body);
			Transform2D::applyTransform_o(drill);
		}
	}

	int collision(Enemy1 *e, Visual2D *v){
		if(activatedDrill == true){
			if(Point2D::distance(new Point2D(e->x, e->y), new Point2D(x, y)) < 3 * radius + e->radius){
				Point2D *varfB = drill->transf_points.at(4);
				if(Point2D::distance(varfB, new Point2D(e->x, e->y)) < e->radius){
					e->erase(v);
					return 2;
				}
				for(int i = 1; i < drill->transf_points.size(); i++){
					if(Point2D::distance(drill->transf_points.at(i), new Point2D(e->x, e->y)) < e->radius){
						e->erase(v);
						return 2;
					}
				}
			}
			
		}
		float dist = sqrt(pow(e->x - x, 2) + pow(e->y - y, 2));
		if(dist < radius + e->radius) {
			e->erase(v);
			return 1;
		}
		return 0;
	}

	int collision(Enemy2 *e, Visual2D *v){
		if(activatedDrill == true){
			if(Point2D::distance(new Point2D(e->x, e->y), new Point2D(x, y)) < 3 * radius + e->radius){
				Point2D *varfB = drill->transf_points.at(4);
				if(Point2D::distance(varfB, new Point2D(e->x, e->y)) < e->radius){
					e->erase(v);
					return 2;
				}
				for(int i = 1; i < drill->transf_points.size(); i++){
					if(Point2D::distance(drill->transf_points.at(i), new Point2D(e->x, e->y)) < e->radius){
						e->erase(v);
						return 2;
					}
				}
			}
			
		}
		float dist = sqrt(pow(e->x - x, 2) + pow(e->y - y, 2));
		if(dist < radius + e->radius) {
			e->erase(v);
			return 1;
		}
		return 0;
	}

	int collision(Enemy3 *e, Visual2D *v){
		if(activatedDrill == true){
			if(Point2D::distance(new Point2D(e->x, e->y), new Point2D(x, y)) < 3 * radius + e->radius){
				Point2D *varfB = drill->transf_points.at(4);
				if(Point2D::distance(varfB, new Point2D(e->x, e->y)) < e->radius){
					e->erase(v);
					return 2;
				}
				for(int i = 1; i < drill->transf_points.size(); i++){
					if(Point2D::distance(drill->transf_points.at(i), new Point2D(e->x, e->y)) < e->radius){
						e->erase(v);
						return 2;
					}
				}
			}
			
		}
		float dist = sqrt(pow(e->x - x, 2) + pow(e->y - y, 2));
		if(dist < radius + e->radius) {
			e->erase(v);
			return 1;
		}
		return 0;
	}

};
