#include "Brushes.hpp"




/*** Layer ***/

Layer::Layer ()
{
	data = nullptr;
	
	width = 0;
	height = 0;
}

Layer::Layer (int init_width, int init_height, Point init_location)
{
	width  = init_width;
	height = init_height;


	int size = width * height;

	Colour default_colour = OldLace;

	uint32_t colour_mask = default_colour.ToMask (); 


	data = new uint32_t [size];

	for (int i = 0; i < size; i++)
	{
		data[i] = colour_mask;
	}

	location = init_location;
}

Layer::~Layer ()
{
	if (data)
	{
		delete[] data;
	}
}


void Layer::Replace (float dx, float dy)
{
	location.x += dx;
	location.y += dy;
}

void Layer::SetColour (Colour new_colour)
{
	int size = width * height;

	uint32_t colour_mask = new_colour.ToMask ();

	if (data)
	{
		for (int i = 0; i < size; i++)
		{
			data[i] = colour_mask;
		}
	}
}



/*** Abstract brush class ***/

Brush::Brush ()
{
	type = wrong_bt;

	size = 0;
}

Brush::Brush (BrushType init_type, int init_size)
{
	type = init_type;

	size = init_size;
}


int Brush::GetSize ()
{
	return size;
}

void Brush::SetSize (int new_size)
{
	size = new_size;
}

BrushType Brush::GetType ()
{
	return type;
}



/*** Circled brush ***/

CircledBrush::CircledBrush ()
{
}

CircledBrush::CircledBrush (int init_size) : Brush (circled_t, init_size)
{
}

void CircledBrush::Apply (Layer* layer, Point abs_location, Colour colour)
{
	int radius = GetSize ();

	uint32_t colour_mask = colour.ToMask ();

	int location_x = (int) (abs_location.x - layer->location.x);
	int location_y = (int) (abs_location.y - layer->location.y);
	
	int line_len = 0;


	int x = radius;		// Starting with (r, 0) and going up
	int y = 0;

	int P = 1 - radius;

	// y can only increase
	// and x is decreasing till x < y
	// that way we making iterations for one eigths of dots

	while (x >= y)
	{
		line_len = 2 * x;

		DrawHorizontalLine (layer, colour_mask, location_x - x, location_y + y, line_len);
		DrawHorizontalLine (layer, colour_mask, location_x - x, location_y - y, line_len);

		line_len = 2 * y;

		DrawHorizontalLine (layer, colour_mask, location_x - y, location_y + x, line_len);
		DrawHorizontalLine (layer, colour_mask, location_x - y, location_y - x, line_len);


		if (P <= 0)
		{
			P += 2*y + 1;
		}
		else
		{
			x--;

			P+= 2*y - 2*x + 1;
		}

		y++;
	}
}




/*** Circled brush ***/

SquaredBrush::SquaredBrush ()
{
}

SquaredBrush::SquaredBrush (int init_size) : Brush (squared_t, init_size)
{
}

void SquaredBrush::Apply (Layer* layer, Point abs_location, Colour colour)
{
	int side = GetSize ();

	uint32_t colour_mask = colour.ToMask ();

	int location_x = (int) (abs_location.x - layer->location.x);
	int location_y = (int) (abs_location.y - layer->location.y);


	int left = location_x - side / 2;

	int up   = location_y - side / 2;
	int down = location_y + side / 2;

	for (int y = up; y <= down; y++)
	{
		DrawHorizontalLine (layer, colour_mask, left, y, side);
	}
}



void DrawHorizontalLine (Layer* layer, uint32_t colour_mask, int start_x, int start_y, int len)
{
	if (start_y < 0 || start_y > layer->height)
	{
		return;
	}

	if (start_x < 0)
	{
		len = start_x + len;

		start_x = 0;
	}

	if (start_x + len >= layer->width)
	{
		len = layer->width - start_x - 1;
	}

	for (int i = 0; i <= len; i++)
	{
		layer->data[start_y * layer->width + start_x + i] = colour_mask;
	}
}