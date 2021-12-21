#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "../Application/Application.hpp"

enum class ShapeType
{
	wrong_t  = -1,
	circle_t =  0,
	rect_t   =  1,
	rdrect_t =  2,
};



/*** Abstract Shape class ***/

class Shape
{

private:

	ShapeType type;

	Colour colour;

	
public:

	Shape (ShapeType figure, Colour init_colour);

	ShapeType GetType ();

	Colour    GetColour ();


	virtual float left_border () const = 0;
	
	virtual float right_border () const = 0;
	
	virtual float up_border () const = 0;
	
	virtual float down_border () const = 0;
	

	virtual void Draw        (Application* App) = 0;

	virtual void DrawLabel   (Application* App, const char* label) = 0;

	virtual void DrawText    (Application* App, Point left, Point right, const char* label) = 0;

	virtual void DrawTexture (Application* App, Texture* texture) = 0;

	
	virtual void Replace (float dx, float dy) = 0;
	
	virtual bool Contains (Point a) = 0;
};




/*** Rectangle shape class ***/

class Rectangle : public Shape
{

private:

	Point left, right;


public:

	Rectangle (Point init_left, Point init_right, Colour colour);


	float left_border  () const;

	float right_border () const;

	float up_border    () const;

	float down_border  () const;


	void Draw        (Application* App);

	void DrawLabel   (Application* App, const char* label);

	void DrawText    (Application* App, Point left, Point right, const char* label);

	void DrawTexture (Application* App, Texture* texture);


	void Replace (float dx, float dy);

	bool Contains (Point a);


	friend bool CheckCollisionRR   (Shape* first_r, Shape* second_r);

	friend bool CheckCollisionCR   (Shape* first_c, Shape* second_r);
	
	friend bool CheckCollisionRC   (Shape* first_r, Shape* second_c);
};




/*** Circle shape class ***/

class Circle : public Shape
{

private:

	Point centre;

	float radius;


public:

	Circle (Point init_centre, float init_radius, Colour init_colour);
	

	float left_border  () const;

	float right_border () const;

	float up_border    () const;

	float down_border  () const;


	void Draw        (Application* App);

	void DrawLabel   (Application* App, const char* label);

	void DrawText    (Application* App, Point left, Point right, const char* label);

	void DrawTexture (Application* App, Texture* texture);


	void Replace (float dx, float dy);

	bool Contains (Point a);


	friend bool CheckCollisionCC   (Shape* first_c, Shape* second_c);

	friend bool CheckCollisionCR   (Shape* first_c, Shape* second_r);
	
	friend bool CheckCollisionRC   (Shape* first_r, Shape* second_c);
};




/*** Rounded Rectangle shape Class ***/

class RoundedRect : public Shape
{

private:

	Point left, right;

	float radius;


public:

	RoundedRect (Point init_left, Point init_right, float init_radius, Colour init_colour);
	

	float left_border  () const;

	float right_border () const;

	float up_border    () const;

	float down_border  () const;


	void Draw        (Application* App);

	void DrawLabel   (Application* App, const char* label);

	void DrawText    (Application* App, Point left, Point right, const char* label);

	void DrawTexture (Application* App, Texture* texture);


	void Replace (float dx, float dy);

	bool Contains (Point a);
};



/*** Collision Functions ***/

bool CheckCollisionCC (Shape* first_c, Shape* second_c);

bool CheckCollisionCR (Shape* first_c, Shape* second_r);

bool CheckCollisionRC (Shape* first_r, Shape* second_c);

bool CheckCollisionRR (Shape* first_r, Shape* second_r);



/*** Adaptive Labels ***/

void GetAdaptiveLabelsShifts (float height, float width, float label_factor, float* x_shift, float* y_shift);


#endif