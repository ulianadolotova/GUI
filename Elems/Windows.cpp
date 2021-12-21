#include "Windows.hpp"




/*** Sliding context ***/

SlidingContext::SlidingContext ()
{
	value = nullptr;

	min_value = 0;
	max_value = 0;

	vertical = false;

	min_sliding_coord = 0;
	max_sliding_coord = 0;
}





/*** Window abstract class ***/

Window::Window () : Element ()
{
}

Window::Window (WindowType init_type,
				Shape*     init_shape,
				Shape*     init_collision,
				Texture*   init_texture) : Element (ElemType::window_t, init_shape, init_collision, init_texture)
{
	Element::SetDetailedType (init_type);
}




/*** Main window ***/

Main::Main () : Window ()
{
}

Main::Main (Shape*   init_shape,
			Shape*   init_collision,
			Texture* init_texture) : Window (main_t, init_shape, init_collision, init_texture)
{
}




/*** Menu window ***/

Menu::Menu () : Window ()
{
}

Menu::Menu (Shape*   init_shape,
			Shape*   init_collision,
			Texture* init_texture) : Window (menu_t, init_shape, init_collision, init_texture)
{
}


void Menu::Draw (Application* App)
{
	Element::Draw (App);
}




/*** Palette window ***/

Palette::Palette () : Window ()
{
}

Palette::Palette (Shape*   init_shape,
				  Shape*   init_collision,
				  Texture* init_texture) : Window (palette_t, init_shape, init_collision, init_texture)
{
}




/*** Canvas ***/

Canvas::Canvas () : Window ()
{
}

Canvas::Canvas (Shape*   init_shape,
				Shape*   init_collision,
				Texture* init_texture) : Window (canvas_t, init_shape, init_collision, init_texture)
{
}

Canvas::~Canvas ()
{
}


void Canvas::Draw (Application* App)
{
	// Element::Draw (App);
}





/*** Title of canvas ***/

Title::Title () : Window ()
{
}

Title::Title (Shape*   init_shape,
			  Shape*   init_collision,
			  Texture* init_texture) : Window (title_t, init_shape, init_collision, init_texture)
{
	static int canvas_num = 0;

	name = new char [MAX_CANVAS_NAME];

	sprintf (name, "Canvas %d", canvas_num);

	canvas_num++;
}

Title::~Title ()
{
	delete[] name;
}


void Title::Draw (Application* App)
{
	Element::Draw (App);

	Element::DrawLabel (App, name);
}




/*** Colour Picker ***/

ColourPicker::ColourPicker () : Window ()
{
}

ColourPicker::ColourPicker (Shape*   init_shape,
							Shape*   init_collision,
							Texture* init_texture) : Window (colour_picker_t, init_shape, init_collision, init_texture)
{
}




/*** Hue Palette ***/

HuePalette::HuePalette () : Window ()
{
	hue = 0.0;

	sliding_context = nullptr;
}

HuePalette::HuePalette (Shape*   init_shape,
						Shape*   init_collision,
						Texture* init_texture) : Window (hue_palette_t, init_shape, init_collision, init_texture)
{
	hue = 0.0;

	sliding_context = new SlidingContext ();

	sliding_context->value = &hue;
	sliding_context->min_value = 0;
	sliding_context->max_value = 360;

	sliding_context->vertical = true;

	sliding_context->min_sliding_coord = init_shape->down_border ();
	sliding_context->max_sliding_coord = init_shape->up_border ();

	sliding_context->start_position = sliding_context->min_sliding_coord;
}

HuePalette::~HuePalette ()
{
	delete sliding_context;
}

void HuePalette::Move (Point new_location)
{
	float dx = 0;
	float dy = 0;

	Element::Move (new_location, &dx, &dy);

	if (sliding_context->vertical)
	{
		sliding_context->delta = dy;
	}
	else
	{
		sliding_context->delta = dx;
	}

	sliding_context->moved = true;
}





/*** Size Palette ***/

SizePalette::SizePalette () : Window ()
{
	size = 1.0;

	sliding_context = nullptr;
}

SizePalette::SizePalette (Shape*   init_shape,
						  Shape*   init_collision,
						  Texture* init_texture) : Window (size_palette_t, init_shape, init_collision, init_texture)
{
	size = 1.0;

	sliding_context = new SlidingContext ();

	sliding_context->value = &size;
	sliding_context->min_value = 1;
	sliding_context->max_value = 30;

	sliding_context->vertical = false;

	sliding_context->min_sliding_coord = init_shape->left_border  ();
	sliding_context->max_sliding_coord = init_shape->right_border ();

	sliding_context->start_position = sliding_context->min_sliding_coord;
}

SizePalette::~SizePalette ()
{
	delete sliding_context;
}




/*** Container window ***/

Container::Container () : Window ()
{
}

Container::Container (Shape*   init_shape,
					  Shape*   init_collision,
					  Texture* init_texture) : Window (container_t, init_shape, init_collision, init_texture)
{
}




/*** Menu place (car) window ***/

MenuPlace::MenuPlace () : Window ()
{
}

MenuPlace::MenuPlace (Shape*   init_shape,
					  Shape*   init_collision,
					  Texture* init_texture) : Window (menu_place_t, init_shape, init_collision, init_texture)
{
}




/*** Turn off window ***/

TurnOffWindow::TurnOffWindow () : Window ()
{
	visibility = 0;

	screen = nullptr;

	layer = nullptr;
}

TurnOffWindow::TurnOffWindow (Shape*   init_shape,
							  Shape*   init_collision,
							  Texture* init_texture) : Window (turn_off_win_t, init_shape, init_collision, init_texture)
{
	visibility = 0;
	
	int left  = init_shape->left_border  ();
	int right = init_shape->right_border ();
	int up    = init_shape->up_border    ();
	int down  = init_shape->down_border  ();

	int width  = right - left;
	int height = down  - up;

	layer = new Layer (width, height, Point (left, up));

	screen = nullptr;
}


void TurnOffWindow::Draw (Application* App)
{
	UpdateTexture (App);

	if (screen)
	{
		Element::DrawTexture (App, screen);
	}
}

void TurnOffWindow::UpdateTexture (Application* App)
{
	layer->SetColour (Colour (Black.r, Black.g, Black.b, visibility));

	int width  = layer->width;
	int height = layer->height;

	App->UpdateTexture (&screen, (void*) layer->data, width, height);
}