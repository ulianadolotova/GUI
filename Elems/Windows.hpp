#ifndef WINDOWS_HPP
#define WINDOWS_HPP


#include "../Drawing/HSV.hpp"


#include <cassert>
#include <algorithm>
#include <list>

using std::min;
using std::max;
using std::list;



const int MAX_CANVAS_NAME = 255;

const int NUM_WINDOW_TYPES = 12;

enum WindowType
{
	wrong_wt        = -1,
	
	main_t          =  0,
	menu_t          =  1,	
	palette_t       =  2,
	canvas_t        =  3,
	title_t         =  4,
	colour_picker_t =  6,
	hue_palette_t   =  7,
	size_palette_t  =  8,
	container_t     =  9,
	menu_place_t    = 10,
	turn_off_win_t  = 11,
};




/*** Sliding context ***/

struct SlidingContext
{
	float* value;

	float min_value;
	float max_value;


	bool vertical;

	float min_sliding_coord;
	float max_sliding_coord;

	float start_position;


	bool moved;

	float delta;


	SlidingContext ();
};




/*** Window abstract class ***/

class Window : public Element
{

public:

	Window ();

	Window (WindowType init_type,
			Shape*     init_shape,
			Shape*     init_collision,
			Texture*   init_texture);
};




/*** Main window ***/

class Main : public Window
{

public:

	Main ();

	Main (Shape*   init_shape,
		  Shape*   init_collision,
		  Texture* init_texture);
};




/*** Menu window ***/

class Menu : public Window
{

public:

	Menu ();

	Menu (Shape*   init_shape,
		  Shape*   init_collision,
		  Texture* init_texture);


	void Draw (Application* App) override;
};




/*** Palette window ***/

class Palette : public Window
{

public:

	Palette ();

	Palette (Shape*   init_shape,
			 Shape*   init_collision,
			 Texture* init_texture);
};




/*** Canvas ***/

class Canvas : public Window
{

public:

	char* name;


	Canvas ();

	Canvas (Shape*   init_shape,
			Shape*   init_collision,
			Texture* init_texture);

	~Canvas ();


	void Draw (Application* App) override;

	void UserDraw (Application* App, DrawingContext* context, Point location);
};





/*** Title of canvas ***/

class Title : public Window
{

public:

	char* name;

	 Title ();

	 Title (Shape*   init_shape,
		   Shape*   init_collision,
		   Texture* init_texture);

	~Title ();


	void Draw (Application* App) override;
};




/*** Colour Picker ***/

class ColourPicker : public Window
{

public:

	ColourPicker ();

	ColourPicker (Shape*   init_shape,
			   	  Shape*   init_collision,
			   	  Texture* init_texture);
};




/*** Hue Palette ***/

class HuePalette : public Window
{

public:

	float hue;

	SlidingContext* sliding_context;


	 HuePalette ();

	 HuePalette (Shape*   init_shape,
			   	 Shape*   init_collision,
			   	 Texture* init_texture);

	~HuePalette ();


	void Move (Point new_location) override;
};




/*** Hue Palette ***/

class SizePalette : public Window
{

public:

	float size;

	SlidingContext* sliding_context;


	 SizePalette ();

	 SizePalette (Shape*   init_shape,
			   	  Shape*   init_collision,
			   	  Texture* init_texture);

	~SizePalette ();
};




/*** Container window ***/

class Container : public Window
{

public:

	Container ();

 	Container (Shape*   init_shape,
			   Shape*   init_collision,
			   Texture* init_texture);
};




/*** Menu place (car) window ***/

class MenuPlace : public Window
{

public:

	MenuPlace ();

 	MenuPlace (Shape*   init_shape,
			   Shape*   init_collision,
			   Texture* init_texture);
};




/*** Turn off window ***/

class TurnOffWindow : public Window
{

private:

	Layer* layer;

	Texture* screen;

	void UpdateTexture (Application* App);


public:

	int visibility;

	TurnOffWindow ();

 	TurnOffWindow (Shape*   init_shape,
				   Shape*   init_collision,
				   Texture* init_texture);


	void Draw (Application* App) override;
};




#endif