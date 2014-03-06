#pragma once
#include "Object3D.h"
#include "Line3D.h"

class Grass
{
public:
	Object3D *obj;
	Point3D *eye;

public:

	Grass(int x, int type, Point3D *_eye){
		eye = _eye;
		init(x, type);
	}

	void init(int x, int type){
		vector <int> contour;
		vector <Point3D*> vertices;
		vector <Face*> faces;
		if(type == 0){
			vertices.push_back(new Point3D(-5000, 0, -INF));
			vertices.push_back(new Point3D(x - THICK, 0, -INF));
			vertices.push_back(new Point3D(x - THICK, 0, 0));
			vertices.push_back(new Point3D(-5000, 0, 0));
			vertices.push_back(new Point3D(-5000, 0, INF));
			vertices.push_back(new Point3D(x - THICK, 0, INF));

			contour.push_back(0);
			contour.push_back(1);
			contour.push_back(2);
			contour.push_back(3);
			contour.push_back(4);
			contour.push_back(5);

			faces.push_back(new Face(contour));
			obj = new Object3D(vertices, faces, Color(0, 173/255.0, 67/255.0), true);		
		}

		if(type == 1){
			vertices.push_back(new Point3D(5000, 0, -INF));
			vertices.push_back(new Point3D(x + THICK, 0, -INF));
			vertices.push_back(new Point3D(x + THICK, 0, 0));
			vertices.push_back(new Point3D(5000, 0, 0));
			vertices.push_back(new Point3D(5000, 0, INF));
			vertices.push_back(new Point3D(x + THICK, 0, INF));
			contour.push_back(0);
			contour.push_back(1);
			contour.push_back(2);
			contour.push_back(3);
			contour.push_back(4);
			contour.push_back(5);
			faces.push_back(new Face(contour));
			obj = new Object3D(vertices, faces, Color(0, 173/255.0, 67/255.0), true);
		}
	}

	void draw(Visual2D *v){
		DrawingWindow::addObject3D_to_Visual2D(obj, v);

		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::loadIdentityModelMatrix();
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(obj);
	}

	void translate(float pas){
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(pas, 0, 0);
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(obj);
	}

	void redraw(Visual2D *v){
		DrawingWindow::removeObject3D_from_Visual2D(obj, v);
		DrawingWindow::addObject3D_to_Visual2D(obj, v);
	}

};

class Road
{
public:
	Object3D *obj;
	Point3D *eye;

public:
	Road(float xl, float xr, Point3D *_eye){
		eye = _eye;

		vector <int> contour;
		vector <Point3D*> vertices;
		vector <Face*> faces;

		vertices.push_back(new Point3D(xl, 0, INF));
		vertices.push_back(new Point3D(xl, 0, 0));
		vertices.push_back(new Point3D(xl, 0, -INF));
		vertices.push_back(new Point3D(xr, 0, -INF));
		vertices.push_back(new Point3D(xr, 0, 0));
		vertices.push_back(new Point3D(xr, 0, INF));

		contour.push_back(0);
		contour.push_back(5);
		contour.push_back(4);
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(3);

		faces.push_back(new Face(contour));

		obj = new Object3D(vertices, faces, Color(132/255.0, 132/255.0, 130/255.0), true);
	}

	void draw(Visual2D *v){
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::loadIdentityModelMatrix();
		Transform3D::perspectiveProjectionMatrix(eye->x, eye->y, eye->z);
		Transform3D::applyTransform(obj);

		DrawingWindow::addObject3D_to_Visual2D(obj, v);
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