#pragma once
#include "Object3D.h"

#define INF 1000000
#define THICK 7

class Line3D
{
public:
	Object3D *obj;
	Point3D *eye;

public:
	Line3D(Point2D p, Point3D *_eye){
		//eye = new Point3D(_eye->x, _eye->y, _eye->z);
		eye = _eye;
		vector <int> contLine;
		vector <Point3D*> vertices;
		vector <Face*> faces;

		vertices.push_back(new Point3D(p.x - THICK, p.y, INF));
		vertices.push_back(new Point3D(p.x - THICK, p.y, 0.5 * INF));
		vertices.push_back(new Point3D(p.x - THICK, p.y, 0));
		vertices.push_back(new Point3D(p.x - THICK, p.y, -0.5 * INF));
		vertices.push_back(new Point3D(p.x - THICK, p.y, -INF));
		vertices.push_back(new Point3D(p.x + THICK, p.y, -INF));
		vertices.push_back(new Point3D(p.x + THICK, p.y, -0.5 * INF));
		vertices.push_back(new Point3D(p.x + THICK, p.y, 0));
		vertices.push_back(new Point3D(p.x + THICK, p.y, 0.5 * INF));
		vertices.push_back(new Point3D(p.x + THICK, p.y, INF));

		contLine.push_back(0);
		contLine.push_back(9);
		contLine.push_back(8);
		contLine.push_back(1);
		contLine.push_back(2);
		contLine.push_back(7);
		contLine.push_back(6);
		contLine.push_back(3);
		contLine.push_back(4);
		contLine.push_back(5);

		faces.push_back(new Face(contLine));

		obj = new Object3D(vertices, faces, Color(1, 1, 1), true);
	}

	void draw(Visual2D *v){
		DrawingWindow::addObject3D_to_Visual2D(obj, v);
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(obj);
	}

	void redraw(Visual2D *v){
		DrawingWindow::removeObject3D_from_Visual2D(obj, v);
		DrawingWindow::addObject3D_to_Visual2D(obj, v);
	}

	void translate(float pas){
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(pas, 0, 0);
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(obj);
	}
};