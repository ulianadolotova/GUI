#ifndef BRUSHES_HPP
#define BRUSHES_HPP


#include "../Elems/Elements.hpp"



enum BrushType
{
	wrong_bt  = -1,
	circled_t =  0,
	squared_t =  1
};


struct Layer
{
	uint32_t* data;

	int width;
	
	int height;

	Point location;


	 Layer ();

	 Layer (int init_width, int init_height, Point init_location);

	~Layer ();

	void Replace (float dx, float dy);

	void SetColour (Colour new_colour);
};




class Brush
{

private:

	int size;

	BrushType type;


public:

	Brush ();

	Brush (BrushType init_type, int init_size);


	int GetSize ();

	void SetSize (int new_size);

	BrushType GetType ();

	virtual void Apply (Layer* layer, Point abs_location, Colour colour) = 0;
};




class CircledBrush : public Brush
{

public:

	CircledBrush ();

	CircledBrush (int init_size);


	void Apply (Layer* layer, Point abs_location, Colour colour) override;
};


class SquaredBrush : public Brush
{

public:

	SquaredBrush ();

	SquaredBrush (int init_size);


	void Apply (Layer* layer, Point abs_location, Colour colour) override;
};




const int MAX_NUM_BRUSHES = 2;




void DrawHorizontalLine (Layer* layer, uint32_t colour_mask, int start_x, int start_y, int len);




#endif