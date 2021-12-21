#include "HSV.hpp"




ColourDistribution::ColourDistribution ()
{
	hue = 0;

	r = g = b = 0;

	max = mid = min = nullptr;
}

ColourDistribution::ColourDistribution (int init_hue)
{
	hue = init_hue;

	segment = hue / 60.0;


	if (segment <= 1)
	{
		max = &r;
		mid = &g;
		min = &b;
	}
	else if (segment > 1 && segment <= 2)
	{
		max = &g;
		mid = &r;
		min = &b;
	}
	else if (segment > 2 && segment <= 3)
	{
		max = &g;
		mid = &b;
		min = &r;	
	}
	else if (segment > 3 && segment <= 4)
	{
		max = &b;
		mid = &g;
		min = &r;
	}
	else if (segment > 4 && segment <= 5)
	{
		max = &b;
		mid = &r;
		min = &g;	
	}
	else if (segment > 5)
	{
		max = &r;
		mid = &b;
		min = &g;
	}
}

Colour ColourDistribution::CountColour (float value, float saturation)
{
	CountMaxMin (value, saturation);

	CountMid ();

	return Colour (r, g, b, 255);
}

void ColourDistribution::CountMaxMin (float value, float saturation)
{
	*max = static_cast <int> (value * 255);
	
	*min = static_cast <int> ((1 - saturation) * (*max));
}

void ColourDistribution::CountMid ()
{
	int c = *max - *min;

	*mid = c * (1.0 - fabs (segment - 2.0 * static_cast <int> (segment / 2) - 1.0)) + *min;
}







HSV::HSV ()
{
	hue = 0;

	colours = nullptr;

	texture = nullptr;

	changed = false;


	width = 0;

	height = 0;
}

HSV::HSV (int init_hue, Point init_location, int init_width, int init_height)
{
	hue = init_hue;

	colours = new uint32_t [init_width * init_height];

	texture = nullptr;

	changed = false;


	location = init_location;

	width  = static_cast <float> (init_width);

	height = static_cast <float> (init_height);


	GenerateColours ();
}

HSV::~HSV ()
{
	delete[] colours;
}




void HSV::SetHue (int new_hue)
{
	hue = new_hue;

	GenerateColours ();
}

void HSV::Replace (float dx, float dy)
{
	location.x += dx;
	location.y += dy;
}


void HSV::GenerateColours ()
{
	changed = true;

	ColourDistribution rgb = ColourDistribution (hue);

	float value = 0;
	float saturation = 0;

	for (float i = 1; i <= height; i++)
	{
		value = i / height;
		
		for (float j = 0; j < width; j++)
		{
			saturation = j / width;

			Colour point_colour = rgb.CountColour (value, saturation);


			int point = static_cast <int> ((height - i) * width + j);

			colours[point] = point_colour.ToMask ();
		}		
	}
}


Texture* HSV::GetTexture (Application* App)
{
	if (changed)
	{
		App->UpdateTexture (&texture, (void*) colours, width, height);

		changed = false;
	}

	return texture;
}


Colour HSV::PickColour (Point target_point)
{	
	int point = static_cast <int> ((target_point.y - location.y) * width + (target_point.x - location.x));

	return Colour (colours[point]);
}