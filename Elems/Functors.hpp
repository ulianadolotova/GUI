#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP


#include "../Managers/EventManager.hpp"



#include <queue>
using std::queue;




const int NUM_FUNCTOR_TYPES = 14;

enum ButtonType
{
	wrong_ft             =  -1,
	show_menu_t          =   0,
	add_canvas_t         =   1,
	show_palette_t       =   3,
	client_t             =   4,
	hide_canvas_t        =   5,
	close_canvas_t       =   6,
	pick_colour_t        =   7,
	slider_t             =   8,
	show_colour_picker_t =   9,
	pick_brush_t         =  10,
	pick_size_t          =  11,
	canvas_ptr_t         =  12,
	turn_off_t           =  13,
};


enum class EnabledType
{
	enabled  = 0,
	disabled = 1,
	hidden   = 2,
};


struct Assets
{
	Shape*   shape;
	Shape*   collision;
	Texture* texture;

	Assets ();

	Assets (Shape* init_shape, Shape* init_collision, Texture* init_texture);
};


struct ElemConfig
{
	EnabledType enabled;

	ElemType type;

	int detailed_type;


	ElemConfig* parent;

	list <ElemConfig*> children;


	Assets* assets;


	ElemConfig ();

	ElemConfig (EnabledType init_enabled, ElemType init_type, int init_detailed_type, Assets* init_assets, ElemConfig* init_parent);
};




/*** Functor abstract Class ***/

class Functor
{

protected:

	EventManager* event_manager;


public:

	Functor ();

	Functor (EventManager* init_manager);


	virtual void Act (Mouse* mouse) = 0;

	virtual void Update ();
};




/*** Show Canvas List ***/

class ShowMenu : public Functor
{

private:

	bool active;

	Element* menu;


public:

	ShowMenu ();

	ShowMenu (Element* init_target, EventManager* init_manager);


	void Act (Mouse* mouse) override;
	
	void Update () override;
};




/*** Add Canvas ***/

class AddCanvas : public Functor
{

private:

	Element* parent;


public:

	AddCanvas ();

	AddCanvas (Element* init_parent, EventManager* init_manager);


	void Act (Mouse* mouse) override;
};




/*** Show Palette ***/

class ShowPalette : public Functor
{

private:

	bool active;

	Element* palette;


public:

	ShowPalette ();

	ShowPalette (Element* init_target, EventManager* init_manager);


	void Act (Mouse* mouse) override;
};




/*** Client part of canvas ***/

class Client : public Functor
{

private:

	Element* client_button;


public:

	 Client ();

	 Client (Element* init_target, EventManager* init_manager);

	~Client ();

	void Act (Mouse* mouse) override;
};




/*** Hide Canvas ***/

class HideCanvas : public Functor
{

private:

	Element* canvas;


public:

	HideCanvas ();

	HideCanvas (Element* init_target, EventManager* init_manager);


	void Act (Mouse* mouse) override;
};




/*** Close Canvas ***/

class CloseCanvas : public Functor
{

private:

	Element* canvas;

	Element* canvas_ptr;


public:

	CloseCanvas ();

	CloseCanvas (Element* init_first_target, Element* init_second_target, EventManager* init_manager);


	void Act (Mouse* mouse) override;
};




/*** PickColour ***/

class PickColour : public Functor
{

private:

	float* hue;

	float last_hue;

	HSV* colour_range;

	Colour chosen_colour;


public:

	PickColour ();

	PickColour (float* init_hue, HSV* init_colour_range, EventManager* init_manager);


	void Act (Mouse* mouse) override;
	
	void Update () override;
};




/*** Scroll bar ***/

class Slider : public Functor
{

private:

	Point old_location;

	Element* slider_button;

	SlidingContext* sliding_context;


public:

	Slider ();

	Slider (SlidingContext* init_context, Element* init_target, EventManager* init_manager);


	void Act (Mouse* mouse) override;
	
	void Update () override;
};




/*** Show Colour Picker ***/

class ShowColourPicker : public Functor
{

private:

	bool active;

	Element* colour_picker;


public:

	ShowColourPicker ();

	ShowColourPicker (Element* init_target, EventManager* init_manager);


	void Act (Mouse* mouse) override;
};




/*** Pick Brush ***/

class PickBrush : public Functor
{

private:

	int brush_num;


public:

	PickBrush ();

	PickBrush (int init_brush_num, EventManager* init_manager);


	void Act (Mouse* mouse) override;
};



/*** Pick Size ***/

class PickSize : public Functor
{

private:

	float* size;

	float last_size;


public:

	PickSize ();

	PickSize (float* init_size, EventManager* init_manager);


	void Act (Mouse* mouse) override;
	
	void Update () override;
};




/*** Canvas pointer ***/

class CanvasPointer : public Functor
{

private:

	Element* canvas;


public:

	CanvasPointer ();

	CanvasPointer (Element* init_target, EventManager* init_manager);


	void Act (Mouse* mouse) override;
};




/*** Turn Off ***/

class TurnOff : public Functor
{

private:

	int delta;

	int* visibility;


public:

	TurnOff ();

	TurnOff (int* init_visibility, EventManager* init_manager);


	void Act (Mouse* mouse) override;
};




#endif