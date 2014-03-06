#pragma once
#include "Object3D.h"
#include "DrawingWindow.h"
#include "Transform3D.h"

#define PI 3.14159265358979323846


class Sphere3D 
{
public:
	Object3D *obj, *wire;
	Point3D *centre, *p_centre, *b;
	Point3D *eye;
	Point3D *sq_pSJ, *sq_pDS;
	Point3D *sq_pSJ_p, *sq_pDS_p;
	float distanceMoved;
	float radius;
	int points;

public:
	Sphere3D(Point3D p, float _radius, Point3D *_eye){
		radius = _radius;
		eye = _eye;
		centre = new Point3D(p.x, p.y + _radius, p.z);
		b = new Point3D(p.x, p.y, p.z);
		p_centre = new Point3D();
		sq_pSJ = new Point3D(b->x - 100, b->y, b->z + 100);
		sq_pDS = new Point3D(b->x + 100, b->y, b->z - 100);
		sq_pSJ_p = new Point3D();
		sq_pDS_p = new Point3D();
		distanceMoved = 0;
		points = 150;

		vector<Point3D*> vertices;
		vector<Face*> faces;
		vector<int> contour;
	
		float phi, theta;
		int i = 0;
		float d = 18.0;

		for (phi = 0; phi <= PI; phi += PI / d) {
			// AICI INCEPE
			contour.clear();
			for (theta = 0; theta <= 2 * PI; theta += PI / d) {
				Point3D *p = new Point3D();
				p->x = centre->x + radius * cos(theta) * sin(phi);
				p->y = centre->y + radius * sin(theta) * sin(phi);
				p->z = centre->z + radius * cos(phi);
				vertices.push_back(p);
				contour.push_back(i);
				i++;
			}
			faces.push_back(new Face(contour));
			// AICI SE TERMINA
		}
	
		phi = 180 + d;
		theta = 360 + d;

		for (int i = 0; i < theta / d; i++){
			contour.clear();
			for (int j = 0; j < phi / d; j++){
				contour.push_back(i + j * theta / d);
			}
			faces.push_back(new Face(contour));
		}

		wire = new Object3D(vertices, faces, Color(1, 0, 0), false);
	}

	void draw(Visual2D *v, float pas){
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(pas, 0, 0);
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(wire);
		DrawingWindow::addObject3D_to_Visual2D(wire, v);
		Transform3D::applyTransform_p(centre, p_centre);
		Transform3D::applyTransform_p(sq_pSJ, sq_pSJ_p);
		Transform3D::applyTransform_p(sq_pDS, sq_pDS_p);
	}

	void move(float pas, float movement){
		distanceMoved += movement;
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(pas, 0, distanceMoved);
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(wire);
		Transform3D::applyTransform_p(centre, p_centre);
		Transform3D::applyTransform_p(sq_pSJ, sq_pSJ_p);
		Transform3D::applyTransform_p(sq_pDS, sq_pDS_p);
	}

	bool collision(Car *c){
		Point3D *lC = c->obj->transf_vertices[3];
		Point3D *rC = c->obj->transf_vertices[2];
		Point3D *mP = new Point3D((lC->x + rC->x) / 2, (lC->y + rC->y) / 2, 0);

		if(lC->x > sq_pSJ_p->x && lC->y > sq_pSJ_p->y && lC->x < sq_pDS_p->x && lC->y < sq_pDS_p->y)
			return true;

		if(rC->x > sq_pSJ_p->x && rC->y > sq_pSJ_p->y && rC->x < sq_pDS_p->x && rC->y < sq_pDS_p->y)
			return true;

		if(mP->x > sq_pSJ_p->x && mP->y > sq_pSJ_p->y && mP->x < sq_pDS_p->x && mP->y < sq_pDS_p->y)
			return true;
		
		lC = c->obj->transf_vertices[0];
		rC = c->obj->transf_vertices[1];
		mP = new Point3D((lC->x + rC->x) / 2, (lC->y + rC->y) / 2, 0);

		if(lC->x > sq_pSJ_p->x && lC->y > sq_pSJ_p->y && lC->x < sq_pDS_p->x && lC->y < sq_pDS_p->y)
			return true;

		if(rC->x > sq_pSJ_p->x && rC->y > sq_pSJ_p->y && rC->x < sq_pDS_p->x && rC->y < sq_pDS_p->y)
			return true;

		if(mP->x > sq_pSJ_p->x && mP->y > sq_pSJ_p->y && mP->x < sq_pDS_p->x && mP->y < sq_pDS_p->y)
			return true;

		return false;
	}

	bool isInBoundaries(){
		if(p_centre->x > 0 && p_centre->y > 0)
			return true;
		else
			return false;
	}

	void erase(Visual2D *v){
		DrawingWindow::removeObject3D_from_Visual2D(wire, v);
	}
};