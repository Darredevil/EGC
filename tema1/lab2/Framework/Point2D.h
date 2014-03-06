#pragma once
#include "Color.h"
#include <math.h>

using namespace std;

class Point2D
{
public:
	float x,y;
	//Color color;

public:
	Point2D()
	{
		x = 0;
		y = 0;
		//color.r = color.g = color.b = 0;
	}

	Point2D(float _x, float _y)
	{
		x = _x;
		y = _y;
		//color.r = color.g = color.b = 0;
	}

	/* Point2D(float _x, float _y, Color _color)
	{
		x = _x;
		y = _y;
		color.r = _color.r;
		color.g = _color.g;
		color.b = _color.b;
	}*/

	static float distance(Point2D *p1, Point2D *p2){
		return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
	}

	static float slope(Point2D *p1, Point2D *p2){
		return (p1->y - p2->y)/(p1->x - p2->x);
	}
	
	~Point2D()
	{}

};

