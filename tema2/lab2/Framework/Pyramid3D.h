#pragma once
#include "Object3D.h"
#include "DrawingWindow.h"

#define LENGTH 220
#define PYR_HEIGHT 300

class Pyramid3D
{
public:
	Object3D *obj, *wire;
	Point3D *eye;
	float distanceMoved;
	int points;

public:
	Pyramid3D(Point3D p, Point3D *_eye){
		eye = _eye;
		distanceMoved = 0;
		points = 250;
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);

		vector <int> contour;
		vector <Point3D*> vertices;
		vector <Face*> faces;

		vertices.push_back(new Point3D(p.x - LENGTH/2, p.y, p.z + LENGTH/2));
		vertices.push_back(new Point3D(p.x + LENGTH/2, p.y, p.z + LENGTH/2));
		vertices.push_back(new Point3D(p.x + LENGTH/2, p.y, p.z - LENGTH/2));
		vertices.push_back(new Point3D(p.x - LENGTH/2, p.y, p.z - LENGTH/2));
		vertices.push_back(new Point3D(p.x, p.y + PYR_HEIGHT, p.z));

		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(3);
		faces.push_back(new Face(contour));
	
		contour.clear();
		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(4);
		faces.push_back(new Face(contour));

		contour.clear();
		contour.push_back(1);
		contour.push_back(4);
		contour.push_back(2);
		faces.push_back(new Face(contour));

		contour.clear();
		contour.push_back(2);
		contour.push_back(4);
		contour.push_back(3);
		faces.push_back(new Face(contour));

		contour.clear();
		contour.push_back(3);
		contour.push_back(4);
		contour.push_back(0);
		faces.push_back(new Face(contour));

		obj = new Object3D(vertices, faces, Color(1, 1, 0), true);
		wire = new Object3D(vertices, faces, Color(0, 0, 1), false);
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

	bool isInBoundaries(){
		if(obj->transf_vertices[4]->y > 0 && obj->transf_vertices[4]->x > 0)
			return true;
		else
			return false;
	}

	bool collision(Car *c){
		Point3D *leftCorner = c->obj->transf_vertices[3];
		Point3D *rightCorner = c->obj->transf_vertices[2];
		Point3D *middlePoint = new Point3D((leftCorner->x + rightCorner->x) / 2, (leftCorner->y + rightCorner->y) / 2, 0);

		if(leftCorner->x > obj->transf_vertices[0]->x && leftCorner->y > obj->transf_vertices[0]->y
			&& leftCorner->x < obj->transf_vertices[2]->x && leftCorner->y < obj->transf_vertices[2]->y)
			return true;
		
		if(rightCorner->x > obj->transf_vertices[0]->x && rightCorner->y > obj->transf_vertices[0]->y
			&& rightCorner->x < obj->transf_vertices[2]->x && rightCorner->y < obj->transf_vertices[2]->y)
			return true;
		
		if(middlePoint->x > obj->transf_vertices[0]->x && middlePoint->y > obj->transf_vertices[0]->y
			&& middlePoint->x < obj->transf_vertices[2]->x && middlePoint->y < obj->transf_vertices[2]->y)
			return true;

		leftCorner = c->obj->transf_vertices[0];
		rightCorner = c->obj->transf_vertices[1];
		middlePoint = new Point3D((leftCorner->x + rightCorner->x) / 2, (leftCorner->y + rightCorner->y) / 2, 0);

		if(leftCorner->x > obj->transf_vertices[0]->x && leftCorner->y > obj->transf_vertices[0]->y
			&& leftCorner->x < obj->transf_vertices[2]->x && leftCorner->y < obj->transf_vertices[2]->y)
			return true;
		
		if(rightCorner->x > obj->transf_vertices[0]->x && rightCorner->y > obj->transf_vertices[0]->y
			&& rightCorner->x < obj->transf_vertices[2]->x && rightCorner->y < obj->transf_vertices[2]->y)
			return true;
		
		if(middlePoint->x > obj->transf_vertices[0]->x && middlePoint->y > obj->transf_vertices[0]->y
			&& middlePoint->x < obj->transf_vertices[2]->x && middlePoint->y < obj->transf_vertices[2]->y)
			return true;

		return false;
	}

	void erase(Visual2D *v){
		DrawingWindow::removeObject3D_from_Visual2D(obj, v);
		DrawingWindow::removeObject3D_from_Visual2D(wire, v);
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

	~Pyramid3D(){}

};