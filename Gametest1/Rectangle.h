#pragma once
#include <iostream>
#include <stdlib.h>
class Rectangle
{
public:
	Rectangle(int x, int y, int width, int height);
	void setHeight(int height);
	void setWidth(int width);
	int perimeter();		//piiri
	float area();
	void show();
	int sameArea(Rectangle);

	int left();
	int right();
	int top();
	int bottom();
	~Rectangle();
private:
	int rX, rY, rWidth, rHeight;
};

