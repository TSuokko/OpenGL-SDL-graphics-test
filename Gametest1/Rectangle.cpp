#include "Rectangle.h"

Rectangle::Rectangle(int x, int y, int w, int h)
{
	setWidth(w);
	setHeight(h);
	rX = x;
	rY = y;
}

Rectangle::~Rectangle()
{
}

void Rectangle::setHeight(int h)
{
	rHeight = h;
}
void Rectangle::setWidth(int w)
{
	rWidth = w;
}
int Rectangle::perimeter()
{
	return (2 * rHeight + 2 * rWidth);
}//piiri
float Rectangle::area()
{
	return rHeight * rWidth;
}
void Rectangle::show()
{
	//cout << "Length: " << length << " Width: " << width;
}
int Rectangle::sameArea(Rectangle other)
{
	int areaf = rHeight * rWidth;
	int areas = other.rHeight * other.rWidth;
	if (areaf == areas)
		return 1;
	return 0;
}

int Rectangle::left()
{
	return rX - (rWidth / 2);
}
int Rectangle::right()
{
	return rX + (rWidth / 2);
}
int Rectangle::top()
{
	return rY - (rHeight / 2);
}
int Rectangle::bottom()
{
	return rY + (rHeight / 2);
}