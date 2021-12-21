#include "Buttons.hpp"



/*** Button abstract class ***/

Button::Button () : Element ()
{
	functor = nullptr;
}

Button::Button (ButtonType init_type,
				Shape*	   init_shape,
				Shape*	   init_collision,
				Texture*   init_texture) : Element (ElemType::button_t, init_shape, init_collision, init_texture)
{
	Element::SetDetailedType (init_type);

	functor = nullptr;
}

bool Button::Dummy ()
{
	return (functor == nullptr);
}

void Button::SetFunctor (Functor* init_functor)
{
	functor = init_functor;
}

void Button::Act (Mouse* mouse)
{
	if (functor)
	{
		functor->Act (mouse);
	}
}

void Button::Update ()
{
	if (functor)
	{
		functor->Update ();
	}	
}




/*** Show Canvas List ***/

ShowMenuButton::ShowMenuButton ()
					  : Button ()
{
}

ShowMenuButton::ShowMenuButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
					  : Button (show_menu_t, init_shape, init_collision, init_texture)
{
}

void ShowMenuButton::Draw (Application* App)
{
	Element::Draw (App);
}




/*** Add Canvas ***/

AddCanvasButton::AddCanvasButton ()
					    : Button ()
{
}

AddCanvasButton::AddCanvasButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
					    : Button (add_canvas_t, init_shape, init_collision, init_texture)
{
}

void AddCanvasButton::Draw (Application* App)
{
	Element::Draw (App);

	Element::DrawLabel (App, "Add Canvas");
}




/*** Show Palette ***/

ShowPaletteButton::ShowPaletteButton ()
					        : Button ()
{
}

ShowPaletteButton::ShowPaletteButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
					        : Button (show_palette_t, init_shape, init_collision, init_texture)
{
}




/*** Show Palette ***/

ClientButton::ClientButton ()
				  : Button ()
{
	screen = nullptr;

	layer = nullptr;
}

ClientButton::ClientButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
				  : Button (client_t, init_shape, init_collision, init_texture)
{
	int left  = init_shape->left_border  ();
	int right = init_shape->right_border ();
	int up    = init_shape->up_border    ();
	int down  = init_shape->down_border  ();

	int width  = right - left;
	int height = down  - up;

	layer = new Layer (width, height, Point (left, up));

	screen = nullptr;
}

ClientButton::~ClientButton ()
{
	delete layer;
}


void ClientButton::Draw (Application* App)
{
	UpdateTexture (App);

	if (screen)
	{
		Element::DrawTexture (App, screen);
	}
}


void ClientButton::UserDraw (Application* App, DrawingContext* context, Point location)
{
	context->ApplyBrush (layer, location);
}

void ClientButton::UpdateTexture (Application* App)
{
	int width  = layer->width;
	int height = layer->height;

	App->UpdateTexture (&screen, (void*) layer->data, width, height);
}

void ClientButton::Move (Point new_location)
{
	float dx = 0;
	float dy = 0;

	Element::Move (new_location, &dx, &dy);	

	layer->Replace (dx, dy);
}




/*** Hide Canvas ***/

HideCanvasButton::HideCanvasButton ()
					      : Button ()
{
}

HideCanvasButton::HideCanvasButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
					      : Button (hide_canvas_t, init_shape, init_collision, init_texture)
{
}




/*** Close Canvas ***/

CloseCanvasButton::CloseCanvasButton ()
					        : Button ()
{
}

CloseCanvasButton::CloseCanvasButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
					        : Button (close_canvas_t, init_shape, init_collision, init_texture)
{
}




/*** Pick Colour ***/

PickColourButton::PickColourButton ()
				  : Button ()
{
}

PickColourButton::PickColourButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
				  : Button (pick_colour_t, init_shape, init_collision, init_texture)
{
	int left  = init_shape->left_border  ();
	int right = init_shape->right_border ();
	int up    = init_shape->up_border    ();
	int down  = init_shape->down_border  ();

	int width  = right - left;
	int height = down  - up;


	colour_range = new HSV (0, Point (left, up), width, height);

	colour_range_texture = nullptr;

}

PickColourButton::~PickColourButton ()
{
	delete colour_range;
}


void PickColourButton::Draw (Application* App)
{
	colour_range_texture = colour_range->GetTexture (App);

	if (colour_range_texture)
	{
		Element::DrawTexture (App, colour_range_texture);
	}
}

void PickColourButton::Move (Point new_location)
{
	float dx = 0;
	float dy = 0;

	Element::Move (new_location, &dx, &dy);
	
	colour_range->Replace (dx, dy);
}




/*** Slider ***/

SliderButton::SliderButton ()
				  : Button ()
{
}

SliderButton::SliderButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
				  : Button (slider_t, init_shape, init_collision, init_texture)
{
}




/*** Show Colour Picker ***/

ShowColourPickerButton::ShowColourPickerButton ()
					        		  : Button ()
{
}

ShowColourPickerButton::ShowColourPickerButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
					        		  : Button (show_colour_picker_t, init_shape, init_collision, init_texture)
{
}




/*** Pick Brush ***/

PickBrushButton::PickBrushButton ()
						: Button ()
{
	brush = wrong_bt;
}

PickBrushButton::PickBrushButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
						: Button (pick_brush_t, init_shape, init_collision, init_texture)
{
	switch (init_shape->GetType ())
	{
		case ShapeType::circle_t:	brush = circled_t;
									break;

		case ShapeType::rect_t:		brush = squared_t;
									break;

		default:	cout << "PickBrush button with shape, that not corresponding any of brushes" << std::endl;
					assert (!"OK");
	}
}




/*** Pick Size ***/

PickSizeButton::PickSizeButton ()
					  : Button ()
{
}

PickSizeButton::PickSizeButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
					  : Button (pick_size_t, init_shape, init_collision, init_texture)
{
}




/*** Canvas pointer ***/

CanvasPointerButton::CanvasPointerButton ()
					        	: Button ()
{
}

CanvasPointerButton::CanvasPointerButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
					        	: Button (canvas_ptr_t, init_shape, init_collision, init_texture)
{
	const int dy = init_shape->down_border () - init_shape->up_border ();

	static int canvas_num = 0;

	name = new char [MAX_CANVAS_NAME];

	sprintf (name, "Canvas %d", canvas_num);

	Replace (0, (canvas_num % 4) * 3 * dy / 2);

	canvas_num++;
}

CanvasPointerButton::~CanvasPointerButton ()
{
	delete[] name;
}

void CanvasPointerButton::Draw (Application* App)
{
	Element::Draw (App);

	Element::DrawLabel (App, name);
}




/*** Turn Off ***/

TurnOffButton::TurnOffButton ()
					: Button ()
{
}

TurnOffButton::TurnOffButton (Shape* init_shape, Shape* init_collision, Texture* init_texture)
					: Button (turn_off_t, init_shape, init_collision, init_texture)
{
}