#include "Elements.hpp"


Element::Element ()
{
	type = ElemType::wrong_t;

	detailed_type = -1;


	shape = nullptr;

	collision = nullptr;

	texture = nullptr;


	old_location = Point (0, 0);
}


Element::Element (ElemType init_type,
		 		  Shape*   init_shape,
		 		  Shape*   init_collision,
		 		  Texture* init_texture)
{
	type = init_type;

	detailed_type = -1;


	shape = init_shape;

	collision = init_collision;

	texture = init_texture;


	old_location = Point (0, 0);
}



ElemType Element::GetType ()
{
	return type;
}


int Element::GetDetailedType ()
{
	return detailed_type;
}


void Element::SetDetailedType (int init_detailed_type)
{
	detailed_type = init_detailed_type;
}




void Element::Move (Point new_location)
{	
	float dx = 0;
	float dy = 0;

	Move (new_location, &dx, &dy);
}


void Element::Move (Point new_location, float* delta_x, float* delta_y)
{	
	float dx = new_location.x - old_location.x;
	float dy = new_location.y - old_location.y;

	if (old_location == Point (0, 0) || 
		dx <= -15 || dx >= 15        || 
		dy <= -15 || dy >= 15)
	{
		old_location = new_location;

		return;
	}
	
	Replace (dx, dy);

	old_location.x += dx;
	old_location.y += dy;

	*delta_x = dx;
	*delta_y = dy;
}


void Element::Replace (float dx, float dy)
{
	if (shape)
	{
		shape->Replace (dx, dy);
	}
	
	if (collision)
	{
		collision->Replace (dx, dy);
	}
}


bool Element::Contains (Point location)
{
	if (collision)
	{
		return collision->Contains (location);
	}
	else if (shape)
	{
		return shape->Contains (location);
	}

	return false;
}



void Element::Draw (Application* App)
{
	if (shape && texture)
	{
		shape->DrawTexture (App, texture);
	}
	else if (shape)
	{
		shape->Draw (App);
	}

	// if (collision)
	// {
	// 	collision->Draw (App);
	// }
}

void Element::DrawLabel (Application* App, const char* label)
{
	shape->DrawLabel (App, label);
}

void Element::DrawTexture (Application* App, Texture* elem_texture)
{
	shape->DrawTexture (App, elem_texture);
}