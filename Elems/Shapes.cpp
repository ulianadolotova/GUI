#include <cassert>
#include <algorithm>

using std::min;
using std::max;

#include "Shapes.hpp"




/*** Abstract Shape class ***/

Shape::Shape (ShapeType figure, Colour init_colour)
{
	type = figure;

	colour = init_colour;
}

ShapeType Shape::GetType ()
{
	return type;
}

Colour Shape::GetColour ()
{
	return colour;
}




/*** Rectangle shape class ***/

Rectangle::Rectangle (Point init_left, Point init_right, Colour colour) : Shape (ShapeType::rect_t, colour)
{
	left  = init_left;

	right = init_right;
}

float Rectangle::left_border () const
{
	return left.x;
}

float Rectangle::right_border () const
{
	return right.x;
}

float Rectangle::up_border () const
{
	return min (left.y, right.y);
}

float Rectangle::down_border () const
{
	return max (left.y, right.y);
}


void Rectangle::Draw (Application* App)
{
	App->DrawRect (left, right, GetColour ());
}

void Rectangle::DrawLabel (Application* App, const char* label)
{
	float right_b = right_border ();
	float left_b  = left_border  ();
	float up_b    = up_border    ();
	float down_b  = down_border  ();

	float width  = right_b - left_b;
	float height = down_b  - up_b;

	float x_shift = 0;
	float y_shift = 0;
	
	float label_factor = strlen (label) / 3.0;

	GetAdaptiveLabelsShifts (height, width, label_factor, &x_shift, &y_shift);

	DrawText (App, Point (left_b + x_shift, up_b + y_shift), Point (right_b - x_shift, down_b - y_shift), label);
}

void Rectangle::DrawText (Application* App, Point left, Point right, const char* label)
{
	assert (left_border () <= left.x  <= right_border ());
	assert (left_border () <= right.x <= right_border ());

	assert (up_border () <= left.y  <= down_border ());
	assert (up_border () <= right.y <= down_border ());
	
	App->DrawText (label, left, right, GetColour ());
}

void Rectangle::DrawTexture (Application* App, Texture* texture)
{
	App->DrawTexture (texture, left, right);
}


void Rectangle::Replace (float dx, float dy)
{
	left.x += dx;
	left.y += dy;

	right.x += dx;
	right.y += dy;
}

bool Rectangle::Contains (Point a)
{
	return (left_border () <= a.x && a.x <= right_border () && 
		  	  up_border () <= a.y && a.y <=  down_border ());
}




/*** Circle shpae class ***/

Circle::Circle (Point init_centre, float init_radius, Colour init_colour) : Shape (ShapeType::circle_t, init_colour)
{
	centre = init_centre;

	radius = init_radius;
}


float Circle::left_border () const
{
	return centre.x - radius;
}

float Circle::right_border () const
{
	return centre.x + radius;
}

float Circle::up_border () const
{
	return centre.y - radius;
}

float Circle::down_border () const
{
	return centre.y + radius;
}


void Circle::Draw (Application* App)
{
	App->DrawCircle (centre, radius, GetColour ());
}

void Circle::DrawLabel (Application* App, const char* label)
{
	float width  = 2 * radius * 0.7;
	float height = 2 * radius * 0.7;

	float x_shift = 0;
	float y_shift = 0;
	
	float label_factor = strlen (label) / 3.0;

	GetAdaptiveLabelsShifts (height, width, label_factor, &x_shift, &y_shift);

	x_shift = radius * 0.7 - x_shift;
	y_shift = radius * 0.7 - y_shift;

	App->DrawText (label, Point (centre.x - x_shift, centre.y - y_shift), Point (centre.x + x_shift, centre.y + y_shift), GetColour ());
}

void Circle::DrawText (Application* App, Point left, Point right, const char* label)
{
	assert (left_border () <= left.x  <= right_border ());
	assert (left_border () <= right.x <= right_border ());

	assert (up_border () <= left.y  <= down_border ());
	assert (up_border () <= right.y <= down_border ());
	
	App->DrawText (label, left, right, GetColour ());
}

void Circle::DrawTexture (Application* App, Texture* texture)
{
	Point left  = Point (centre.x - radius, centre.y - radius);
	Point right = Point (centre.x + radius, centre.y + radius); 

	App->DrawTexture (texture, left, right);
}


void Circle::Replace (float dx, float dy)
{
	centre.x += dx;
	centre.y += dy;
}

bool Circle::Contains (Point a)
{
	int dx = a.x - centre.x;
	int dy = a.y - centre.y;

	return (dx * dx + dy * dy <= radius * radius);
}




/*** Rounded Rectangle shape class ***/

RoundedRect::RoundedRect (Point init_left, Point init_right, float init_radius, Colour colour) : Shape (ShapeType::rdrect_t, colour)
{
	left  = init_left;

	right = init_right;

	radius = init_radius;
}


float RoundedRect::left_border () const
{
	return left.x;
}

float RoundedRect::right_border () const
{
	return right.x;
}

float RoundedRect::up_border () const
{
	return min (left.y, right.y);
}

float RoundedRect::down_border () const
{
	return max (left.y, right.y);
}


void RoundedRect::Draw (Application* App)
{
	App->DrawRoundedRect (left, right, radius, GetColour ());
}

void RoundedRect::DrawLabel (Application* App, const char* label)
{
	float right_b = right_border ();
	float left_b  = left_border  ();
	float up_b    = up_border    ();
	float down_b  = down_border  ();

	float width  = right_b - left_b;
	float height = down_b  - up_b;

	float x_shift = 0;
	float y_shift = 0;
	
	float label_factor = strlen (label) / 3.0;

	GetAdaptiveLabelsShifts (height, width, label_factor, &x_shift, &y_shift);

	DrawText (App, Point (left_b + x_shift, up_b + y_shift), Point (right_b - x_shift, down_b - y_shift), label);
}

void RoundedRect::DrawText (Application* App, Point left, Point right, const char* label)
{
	assert (left_border () <= left.x  <= right_border ());
	assert (left_border () <= right.x <= right_border ());

	assert (up_border () <= left.y  <= down_border ());
	assert (up_border () <= right.y <= down_border ());
	
	App->DrawText (label, left, right, GetColour ());
}

void RoundedRect::DrawTexture (Application* App, Texture* texture)
{	
	App->DrawTexture (texture, left, right);
}


void RoundedRect::Replace (float dx, float dy)
{
	left.x += dx;
	left.y += dy;

	right.x += dx;
	right.y += dy;
}

bool RoundedRect::Contains (Point point)
{
	float left  = left_border ();
	float right = right_border ();
	float up    = up_border ();
	float down  = down_border ();

	if (left <= point.x && point.x <= right &&
		  up <= point.y && point.y <=  down)
	{
		if ((left + radius <= point.x && point.x <= right - radius) || 
		    (  up + radius <= point.y && point.y <= down  - radius))
		{
			return true;
		}

		float corner_x = (point.x > right - radius) ? right : left;
		float corner_y = (point.y > down  - radius) ? down  : up;

		float dx = corner_x - point.x;
		float dy = corner_y - point.y;

		if (dx * dx + dy * dy < radius * radius)
		{
			return true;
		}
	}

	return false;
}




/*** Collision Functions ***/

bool CheckCollisionCC (Shape* first_c, Shape* second_c)
{
	Circle* first  = (Circle*) first_c;
	Circle* second = (Circle*) second_c;

	float dx = first->centre.x - second->centre.x;

	float dy = first->centre.y - second->centre.y;

	float dr = first->radius + second->radius;

	return (dx * dx + dy * dy <= dr * dr);
}

bool CheckCollisionCR (Shape* first_c, Shape* second_r)
{
	Circle*    first  = (Circle*)    first_c;
	Rectangle* second = (Rectangle*) second_r;

	if ((second->up_border () < first->up_border   () && first->up_border   () < second->down_border ()) || 
		 second->up_border () < first->down_border () && first->down_border () < second->down_border ())
	{
		if (first->right_border () > second->left_border () && first->right_border () < second->right_border ())
		{
			return true;
		}

		if (first->left_border () > second->left_border () && first->left_border () < second->right_border ())
		{
			return true;
		}
	}

	return false;
}

bool CheckCollisionRC (Shape* first_r, Shape* second_c)
{
	return CheckCollisionCR (second_c, first_r);
}

bool CheckCollisionRR (Shape* first_r, Shape* second_r)
{
	Rectangle* first  = (Rectangle*) first_r;
	Rectangle* second = (Rectangle*) second_r;

	bool f_in_s = false;

	if (first->up_border () > second->up_border () || first->down_border () < second->down_border ())
	{
		if (first->right_border () > second->left_border () && first->right_border () < second->right_border ())
		{
			f_in_s = true;
		}

		if (first->left_border () > second->left_border () && first->left_border () < second->right_border ())
		{
			f_in_s = true;
		}
	}

	bool s_in_f = false;

	if (second->up_border () > first->up_border () || second->down_border () < first->down_border ())
	{
		if (second->right_border () > first->left_border () && second->right_border () < first->right_border ())
		{
			s_in_f = true;
		}

		if (second->left_border () > first->left_border () && second->left_border () < first->right_border ())
		{
			s_in_f = true;
		}
	}
	
	return f_in_s || s_in_f;
}


void GetAdaptiveLabelsShifts (float height, float width, float label_factor, float* x_shift, float* y_shift)
{
	if (height > width)
	{
		*x_shift = width / 10;

		*y_shift = (height - (width - 2 * (*x_shift)) / label_factor) / 2;
	}
	else
	{
		if (width > 0.8 * height * label_factor)
		{
			*y_shift = height / 10;
		}
		else
		{
			*y_shift = (height * 1.1 - width / label_factor) / 2;
		}

		*x_shift = (width - (height - 2 * (*y_shift)) * label_factor) / 2;
	}
}