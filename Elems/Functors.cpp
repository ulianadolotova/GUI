#include "Functors.hpp"




/*** Assets ***/

Assets::Assets ()
{
	shape     = nullptr;
	collision = nullptr;
	texture   = nullptr;
}

Assets::Assets (Shape* init_shape, Shape* init_collision, Texture* init_texture)
{
	shape     = init_shape;
	collision = init_collision;
	texture   = init_texture;
}




/*** Element (window or functor) configuration ***/

ElemConfig::ElemConfig ()
{
	enabled = EnabledType::disabled;

	
	type = ElemType::wrong_t;

	detailed_type = wrong_wt;

	assets = nullptr;


	parent = nullptr;
}

ElemConfig::ElemConfig (EnabledType init_enabled, ElemType init_type, int init_detailed_type, Assets* init_assets, ElemConfig* init_parent)
{
	enabled = init_enabled;

	
	type = init_type;

	detailed_type = init_detailed_type;

	assets = init_assets;


	parent = init_parent;
}





/*** Functor abstract class ***/

Functor::Functor ()
{
	event_manager = nullptr;
}

Functor::Functor (EventManager* init_manager)
{
	event_manager = init_manager;
}

void Functor::Update ()
{
	return;
}




/*** Show Menu ***/

ShowMenu::ShowMenu () : Functor ()
{
	active = false;

	menu = nullptr;
}

ShowMenu::ShowMenu (Element* init_target, EventManager* init_manager) : Functor (init_manager)
{
	active = false;

	menu = init_target;
}


void ShowMenu::Act (Mouse* mouse)
{
	if (!active && mouse->LeftClick () && event_manager)
	{
		active = true;

		event_manager->NewEvent (show_t, menu);
	}

	if (active && mouse->RightClick () && event_manager)
	{
		active = false;

		event_manager->NewEvent (hide_t, menu);
	}
}

void ShowMenu::Update ()
{
}




/*** Add Canvas ***/

AddCanvas::AddCanvas () : Functor ()
{
	parent = nullptr;
}

AddCanvas::AddCanvas (Element* init_parent, EventManager* init_manager) : Functor (init_manager)
{
	parent = init_parent;
}


void AddCanvas::Act (Mouse* mouse)
{
	if (mouse->LeftClick () && event_manager)
	{
		event_manager->NewEvent (add_t, ElemType::window_t, canvas_t, parent);
	}
}




/*** Show Palette ***/

ShowPalette::ShowPalette () : Functor ()
{
	palette = nullptr;
}

ShowPalette::ShowPalette (Element* init_target, EventManager* init_manager) : Functor (init_manager)
{
	palette = init_target;
}


void ShowPalette::Act (Mouse* mouse)
{
	if (!active && mouse->LeftClick () && event_manager)
	{
		active = true;

		event_manager->NewEvent (show_t, palette);
	}
	else if (active && event_manager && mouse->RightClick ())
	{
		active = false;

		event_manager->NewEvent (hide_t, palette);
	}
}




/*** Client part of canvas ***/

Client::Client () : Functor ()
{
}

Client::Client (Element* init_target, EventManager* init_manager) : Functor (init_manager)
{
	client_button = init_target;
}

Client::~Client ()
{
}


void Client::Act (Mouse* mouse)
{
	if (mouse->LeftClick () && event_manager)
	{
		event_manager->NewEvent (draw_t, client_button, mouse->location);
	}
}




/*** Hide Canvas ***/

HideCanvas::HideCanvas () : Functor ()
{
	canvas = nullptr;
}

HideCanvas::HideCanvas (Element* init_target, EventManager* init_manager) : Functor (init_manager)
{
	canvas = init_target;
}


void HideCanvas::Act (Mouse* mouse)
{
	if (mouse->LeftClick () && event_manager)
	{
		event_manager->NewEvent (hide_t, canvas);
	}
}




/*** Hide Canvas ***/

CloseCanvas::CloseCanvas () : Functor ()
{
	canvas     = nullptr;

	canvas_ptr = nullptr;
}

CloseCanvas::CloseCanvas (Element* init_first_target, Element* init_second_target, EventManager* init_manager) : Functor (init_manager)
{
	canvas     = init_first_target;

	canvas_ptr = init_second_target;
}


void CloseCanvas::Act (Mouse* mouse)
{
	if (mouse->LeftClick () && event_manager)
	{
		event_manager->NewEvent (delete_t, canvas);

		event_manager->NewEvent (delete_t, canvas_ptr);
	}
}




/*** PickColour ***/

PickColour::PickColour () : Functor ()
{
	hue = nullptr;

	last_hue = 0;

	chosen_colour = Red;

	colour_range = nullptr;
}

PickColour::PickColour (float* init_hue, HSV* init_colour_range, EventManager* init_manager) : Functor (init_manager)
{
	hue = init_hue;

	last_hue = *init_hue;

	chosen_colour = Black;
	
	colour_range = init_colour_range;
}


void PickColour::Act (Mouse* mouse)
{
	if (mouse->LeftClick () && event_manager)
	{
		chosen_colour = colour_range->PickColour (mouse->location);

		event_manager->NewEvent (set_colour_t, chosen_colour);
	}
}

void PickColour::Update ()
{
	if (*hue != last_hue)
	{
		colour_range->SetHue (static_cast <int> (*hue));

		last_hue = *hue;
	}
}




/*** Scroll bar ***/

Slider::Slider () : Functor ()
{
	slider_button = nullptr;

	sliding_context = nullptr;

	old_location = Point (0, 0);
}

Slider::Slider (SlidingContext* init_context, Element* init_target, EventManager* init_manager) : Functor (init_manager)
{	
	slider_button = init_target;

	sliding_context = init_context;

	old_location = Point (0, 0);

	if (sliding_context->vertical)
	{
		old_location.y = sliding_context->start_position;
	}
	else
	{
		old_location.x = sliding_context->start_position;
	}
}


void Slider::Act (Mouse* mouse)
{
	float sliding_range = fabs (sliding_context->max_sliding_coord - sliding_context->min_sliding_coord);

	float value_range   = fabs (sliding_context->max_value - sliding_context->min_value);


	if (mouse->LeftClick () && event_manager)
	{
		float dx = mouse->location.x - old_location.x;
		float dy = mouse->location.y - old_location.y;

		float* value = sliding_context->value;
		float  min_value = sliding_context->min_value;
		float  max_value = sliding_context->max_value;

		if (sliding_context->vertical)
		{
			float new_value = *value - (dy * value_range) / sliding_range;

			if (new_value > max_value)
			{				
				dy = (*value - max_value) * sliding_range / value_range;

				*value = max_value;

			}
			else if (new_value < min_value)
			{
				dy = (*value - min_value) * sliding_range / value_range;
				
				*value = min_value;
			}
			else
			{
				*value = new_value;
			}

			old_location.y = old_location.y + dy;
			
			event_manager->NewEvent (move_t, slider_button, 0, static_cast <int> (dy));
		}
		else
		{
			float new_value = *value + (dx * value_range) / sliding_range;

			if (new_value > max_value)
			{				
				dx = (*value - max_value) * sliding_range / value_range;

				*value = max_value;
			}
			else if (new_value < min_value)
			{
				dx = (*value - min_value) * sliding_range / value_range;
				
				*value = min_value;
			}
			else
			{
				*value = new_value;
			}

			old_location.x = old_location.x + dx;

			event_manager->NewEvent (move_t, slider_button, static_cast <int> (dx), 0);
		}
	}
}

void Slider::Update ()
{
	if (sliding_context->moved)
	{
		sliding_context->max_sliding_coord += sliding_context->delta;

		sliding_context->min_sliding_coord += sliding_context->delta;

		if (sliding_context->vertical)
		{
			old_location.y += sliding_context->delta;
		}
		else
		{
			old_location.x += sliding_context->delta;
		}

		sliding_context->moved = false;
	}
}




/*** Show Colour Picker ***/

ShowColourPicker::ShowColourPicker () : Functor ()
{
	active = false;

	colour_picker = nullptr;
}

ShowColourPicker::ShowColourPicker (Element* init_target, EventManager* init_manager) : Functor (init_manager)
{
	active = false;
	
	colour_picker = init_target;
}


void ShowColourPicker::Act (Mouse* mouse)
{
	if (!active && mouse->LeftClick () && event_manager)
	{
		active = true;

		event_manager->NewEvent (show_t, colour_picker);
	}
	else if (active && event_manager && mouse->RightClick ())
	{
		active = false;

		event_manager->NewEvent (hide_t, colour_picker);
	}
}




/*** Pick Brush ***/

PickBrush::PickBrush () : Functor ()
{
	brush_num = wrong_bt;
}

PickBrush::PickBrush (int init_brush_num, EventManager* init_manager) : Functor (init_manager)
{
	brush_num = init_brush_num;
}


void PickBrush::Act (Mouse* mouse)
{
	if (mouse->LeftClick () && event_manager)
	{
		event_manager->NewEvent (set_brush_t, brush_num);
	}
}




/*** Pick Size ***/

PickSize::PickSize () : Functor ()
{
	size = nullptr;

	last_size = 0;
}

PickSize::PickSize (float* init_size, EventManager* init_manager) : Functor (init_manager)
{
	size = init_size;

	last_size = *init_size;
}


void PickSize::Act (Mouse* mouse)
{
}

void PickSize::Update ()
{
	if (*size != last_size)
	{
		event_manager->NewEvent (set_size_t, static_cast <int> (*size));

		last_size = *size;
	}
}




/*** Canvas pointer ***/

CanvasPointer::CanvasPointer () : Functor ()
{
	canvas = nullptr;
}

CanvasPointer::CanvasPointer (Element* init_target, EventManager* init_manager) : Functor (init_manager)
{
	canvas = init_target;
}


void CanvasPointer::Act (Mouse* mouse)
{
	if (mouse->LeftClick () && event_manager)
	{
		event_manager->NewEvent (show_t, canvas);
	}
}




/*** Turn Off ***/

TurnOff::TurnOff () : Functor ()
{
	delta = 0;

	visibility = nullptr;
}

TurnOff::TurnOff (int* init_visibility, EventManager* init_manager) : Functor (init_manager)
{
	delta = (SDL_ALPHA_OPAQUE - SDL_ALPHA_TRANSPARENT) / 5;

	visibility = init_visibility;
}


void TurnOff::Act (Mouse* mouse)
{
	if (mouse->LeftClick () && event_manager)
	{
		*visibility += delta;

		if (*visibility >= SDL_ALPHA_OPAQUE)
		{
			*visibility = SDL_ALPHA_OPAQUE;
			
			event_manager->NewEvent (quit_t);
		}
	}
}