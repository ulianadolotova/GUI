#include "Drawing.hpp"




/*** Drawing Context ***/

DrawingContext::DrawingContext ()
{
	paint_brush = nullptr;

	brushes = nullptr;

	paint_colour = Red;
}

DrawingContext::DrawingContext (const char* drawing_conf)
{

	paint_brush = nullptr;

	brushes = new Brush* [MAX_NUM_BRUSHES];

	paint_colour = Red;
	

	FILE* config = fopen (drawing_conf, "r");

	if (config)
	{
		ParseConfig (config);
	}
	else
	{
		cout << "Warning: cannot open file with config: \"" << drawing_conf << "\"" << std::endl;
	}

	fclose (config);
}

DrawingContext::~DrawingContext ()
{
	if (brushes)
	{
		for (int i = 0; i < MAX_NUM_BRUSHES; i++)
		{
			if (brushes[i])
			{
				delete brushes[i];
			}
		}
	}

	delete[] brushes;
}




void DrawingContext::ApplyBrush (Layer* layer, Point location)
{
	if (paint_brush)
	{
		paint_brush->Apply (layer, location, paint_colour);
	}
}


void DrawingContext::SetColour (Colour new_colour)
{
	paint_colour = new_colour;
}

void DrawingContext::SetBrush (int brush_type)
{
	paint_brush = brushes[brush_type];
}

void DrawingContext::SetSize (int new_size)
{
	if (paint_brush)
	{
		paint_brush->SetSize (new_size);
	}
}




/*** Parsing Config ***/

void DrawingContext::ParseConfig (FILE* config)
{
	char rubbish [200];
	char token   [200];

	while (fscanf (config, " %[^\n:]", token) != EOF)
	{
		fscanf (config, " %[\n:]", rubbish);

		if (strcmp (token, "Colour") == 0)
		{
			GetColour (config);
		}
		else if (strcmp (token, "Brushes") == 0)
		{
			GetBrushes (config);
		}
		else
		{
			fscanf (config, "%[\n:]", token);
		}
	}
}


void DrawingContext::GetColour (FILE* config)
{
	char token   [200];
	char rubbish [200];


	int read = 0;

	while (read != EOF)
	{
		read = fscanf (config, "%[\t]", rubbish);

		if (read == 0 || read == EOF)
		{
			break;
		}

		read = fscanf (config, " %s%[\n ]", token, rubbish);

		paint_colour = ColourForName (token);
	}
}


void DrawingContext::GetBrushes (FILE* config)
{
	char token   [200];
	char rubbish [200];


	bool main = true;

	int brush_type = wrong_bt;

	int init_size = 0;


	int read = 0;

	while (read != EOF)
	{
		read = fscanf (config, "%[\t]", rubbish);

		if (read == 0)
		{
			break;
		}

		read = fscanf (config, " %s ", token);
		
		read = fscanf (config, " ( %d ", &init_size);

		read = fscanf (config, ", main )%[\n ]", rubbish);

		if (read == 0)
		{
			read = fscanf (config, ")%[\n ]", rubbish);

			main = false;
		}
		
		Brush* new_brush = BrushForName (token, init_size);

		brushes[new_brush->GetType ()] = new_brush;

		if (main)
		{
			paint_brush = new_brush;
		}

		main = true;
	}
}



Brush* BrushForName (const char* type_name, int init_size)
{
	if (strcmp (type_name, "CircledBrush") == 0)
	{
		return new CircledBrush (init_size);
	}
	else if (strcmp (type_name, "SquaredBrush") == 0)
	{
		return new SquaredBrush (init_size);
	}
	else
	{
		return nullptr;
	}
}


Colour ColourForName (const char* colour_name)
{
	#define DEF_COLOUR(name, r, g, b, empty)		\
		{#name, name##empty},

	static map <string, Colour> colours
	{
		#include "../Application/Colours.hpp"
	};

	#undef DEF_COLOUR

	return colours[colour_name];
}