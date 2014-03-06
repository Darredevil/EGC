#pragma once
#include "Object3D.h"
#include <ctime>

#define CAR_LENGTH 300
#define CAR_WIDTH 140
#define CAR_HEIGHT 55
#define SIDE 5

class Car
{
public:
	Object3D *obj, *wire;
	Point3D *eye;
	float angle, speed, distanceMoved, points;

public:
	Car(Point3D p, Point3D *_eye, Color objCol, Color wireCol){
		eye = _eye;			angle = 0;
		srand(time(NULL));	speed = rand() % 30 + 30;
		distanceMoved = 0;	points = 300;
		vector <int> contour;
		vector <Point3D*> vertices;
		vector <Face*> faces;
		Point3D coltStangaJos = Point3D(p.x - CAR_WIDTH/2, p.y, p.z);

		// varfurile de jos
		vertices.push_back(new Point3D(coltStangaJos.x, coltStangaJos.y, coltStangaJos.z));
		vertices.push_back(new Point3D(coltStangaJos.x + CAR_WIDTH, coltStangaJos.y, coltStangaJos.z));
		vertices.push_back(new Point3D(coltStangaJos.x + CAR_WIDTH - SIDE, coltStangaJos.y, coltStangaJos.z - CAR_LENGTH));
		vertices.push_back(new Point3D(coltStangaJos.x + SIDE, coltStangaJos.y, coltStangaJos.z - CAR_LENGTH));
		// varfurile de sus
		vertices.push_back(new Point3D(coltStangaJos.x, coltStangaJos.y + CAR_HEIGHT, coltStangaJos.z));
		vertices.push_back(new Point3D(coltStangaJos.x + CAR_WIDTH, coltStangaJos.y + CAR_HEIGHT, coltStangaJos.z));
		vertices.push_back(new Point3D(coltStangaJos.x + CAR_WIDTH - SIDE, coltStangaJos.y + CAR_HEIGHT, coltStangaJos.z - CAR_LENGTH));
		vertices.push_back(new Point3D(coltStangaJos.x + SIDE, coltStangaJos.y + CAR_HEIGHT, coltStangaJos.z - CAR_LENGTH));

		//fata jos
		contour.clear();
		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(3);
		faces.push_back(new Face(contour));

		//fata sus
		contour.clear();
		contour.push_back(4);
		contour.push_back(5);
		contour.push_back(6);
		contour.push_back(7);
		faces.push_back(new Face(contour));

		//fata fata
		contour.clear();
		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(5);
		contour.push_back(4);
		faces.push_back(new Face(contour));

		//fata dreapta
		contour.clear();
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(6);
		contour.push_back(5);
		faces.push_back(new Face(contour));

		//fata spate
		contour.clear();
		contour.push_back(2);
		contour.push_back(3);
		contour.push_back(7);
		contour.push_back(6);
		faces.push_back(new Face(contour));

		//fata stanga
		contour.clear();
		contour.push_back(3);
		contour.push_back(0);
		contour.push_back(4);
		contour.push_back(7);
		faces.push_back(new Face(contour));

		obj = new Object3D(vertices, faces, objCol, true);
		wire = new Object3D(vertices, faces, wireCol, false);
	}

	void draw(Visual2D *v, float pas){
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(pas, 0, 0);
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(obj);
		Transform3D::applyTransform(wire);

		DrawingWindow::addObject3D_to_Visual2D(wire, v);
		DrawingWindow::addObject3D_to_Visual2D(obj, v);
	}

	void rotate(float u){
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(-600, 0, 170);
		angle += u;
		Transform3D::rotateMatrixOy(angle);
		Transform3D::translateMatrix(600, 0, -170);
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(obj);
		Transform3D::applyTransform(wire);
	}

	void move(float pas, float movement){
		distanceMoved += movement;
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(pas, 0, distanceMoved);
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(obj);
		Transform3D::applyTransform(wire);
	}

	void constantSpeed(float pas){
		distanceMoved -= speed;
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(pas, 0, distanceMoved);
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(obj);
		Transform3D::applyTransform(wire);
	}

	bool isInBoundaries(){
		Point3D *lC = obj->transf_vertices[7];
		Point3D *rC = obj->transf_vertices[6];

		if(lC->x > 0 && lC->y > 0)
			return true;
		if(rC->x > 0 && rC->y > 0)
			return true;

		return false;
	}

	void erase(Visual2D *v){
		DrawingWindow::removeObject3D_from_Visual2D(obj, v);
		DrawingWindow::removeObject3D_from_Visual2D(wire, v);
	}

	bool collision(Car *c){
		Point3D *lC = c->obj->transf_vertices[3];
		Point3D *rC = c->obj->transf_vertices[2];
		Point3D *lOC = obj->transf_vertices[0];
		Point3D *rOC = obj->transf_vertices[2];

		if(lC->x > lOC->x && lC->y > lOC->y && lC->x < rOC->x && lC->y < rOC->y)
			return true;
		if(rC->x > lOC->x && rC->y > lOC->y && rC->x < rOC->x && rC->y < rOC->y)
			return true;

		lC = c->obj->transf_vertices[0];
		rC = c->obj->transf_vertices[1];

		if(lC->x > lOC->x && lC->y > lOC->y && lC->x < rOC->x && lC->y < rOC->y)
			return true;
		if(rC->x > lOC->x && rC->y > lOC->y && rC->x < rOC->x && rC->y < rOC->y)
			return true;

		return false;
	}

};