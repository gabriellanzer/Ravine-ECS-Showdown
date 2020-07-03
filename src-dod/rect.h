#ifndef RECT_H
#define RECT_H

struct Rect
{
	Rect(float x, float y, float width, float height) : x(x), y(y), hWidth(width*0.5f), hHeight(height*0.5f) {}

	float x;
	float y;
	float hWidth;
	float hHeight;
};

#endif