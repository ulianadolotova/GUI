#ifndef DRAWING_HPP
#define DRAWING_HPP


#include "Brushes.hpp"

#include <map>
#include <string>

using std::map;
using std::string;


class DrawingContext
{

private:

	Brush* paint_brush;

	Colour paint_colour;

	Brush** brushes;


	void ParseConfig (FILE* config);

	void GetColour   (FILE* config);

	void GetBrushes  (FILE* config);



public:

	 DrawingContext ();

	 DrawingContext (const char* drawing_conf);

	~DrawingContext ();


	void ApplyBrush (Layer* layer, Point location);

	void SetColour (Colour new_colour);

	void SetBrush (int brush_type);

	void SetSize (int new_size);
};



Brush* BrushForName (const char* type_name, int init_size);

Colour ColourForName (const char* colour_name);


#endif