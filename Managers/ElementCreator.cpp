#include "ElementCreator.hpp"




/*** Constructors & Destructors ***/

ElementCreator::ElementCreator ()
{
}

ElementCreator::ElementCreator (Application* App, const char* hierarchy_conf)
{
	default_config[ElemType::window_t].resize (NUM_WINDOW_TYPES);

	default_config[ElemType::button_t].resize (NUM_FUNCTOR_TYPES);


	FILE* hierarchy = fopen (hierarchy_conf, "r");

	if (hierarchy)
	{
		ParseHierarchy (App, hierarchy);
	}
	else
	{
		cout << "Warning: cannot open file with hierarchy: \"" << hierarchy_conf << "\"" << std::endl;
	}

	fclose (hierarchy);
}




/*** Main interface ***/

ElemConfig* ElementCreator::GetConfig (ElemType type, int detailed_type)
{
	return default_config[type][detailed_type];
}

ElemConfig* ElementCreator::GetMainConfig ()
{
	return main_config;
}


Element* ElementCreator::NewElement (ElemConfig* config)
{
	switch (config->type)
	{
		case ElemType::window_t: return NewWindow (config);

		case ElemType::button_t: return NewButton (config);

		default: cout << "Unkown type of element: " << static_cast <int> (config->type) << std::endl << std::flush;
				 assert (!"OK");
	}
}

Window* ElementCreator::NewWindow (ElemConfig* config)
{
	Shape*   init_shape     = NewShape (config);
	Shape*   init_collision = NewCollision (config);
	Texture* init_texture   = config->assets->texture;


	switch (config->detailed_type)
	{		
		case main_t      		: return new Main          (init_shape, init_collision, init_texture);

		case menu_t      		: return new Menu          (init_shape, init_collision, init_texture);
		
		case palette_t   		: return new Palette       (init_shape, init_collision, init_texture);

		case canvas_t    		: return new Canvas        (init_shape, init_collision, init_texture);

		case title_t     		: return new Title         (init_shape, init_collision, init_texture);

		case colour_picker_t	: return new ColourPicker  (init_shape, init_collision, init_texture);

		case hue_palette_t		: return new HuePalette    (init_shape, init_collision, init_texture);

		case size_palette_t		: return new SizePalette   (init_shape, init_collision, init_texture);

		case container_t		: return new Container	   (init_shape, init_collision, init_texture);

		case menu_place_t		: return new MenuPlace	   (init_shape, init_collision, init_texture);
		
		case turn_off_win_t		: return new TurnOffWindow (init_shape, init_collision, init_texture);

		default: cout << "Unkown type of window: " << config->detailed_type << std::endl << std::flush;
				 assert (!"OK");
	}

	return nullptr;
}

Button* ElementCreator::NewButton (ElemConfig* config)
{
	Shape*   init_shape     = NewShape (config);
	Shape*   init_collision = NewCollision (config);
	Texture* init_texture   = config->assets->texture;

	switch (config->detailed_type)
	{		
		case show_menu_t		  : return new ShowMenuButton		  (init_shape, init_collision, init_texture);
		
		case add_canvas_t   	  : return new AddCanvasButton		  (init_shape, init_collision, init_texture);

		case show_palette_t 	  : return new ShowPaletteButton	  (init_shape, init_collision, init_texture);

		case client_t       	  : return new ClientButton			  (init_shape, init_collision, init_texture);
		
		case hide_canvas_t  	  : return new HideCanvasButton		  (init_shape, init_collision, init_texture);

		case close_canvas_t 	  : return new CloseCanvasButton	  (init_shape, init_collision, init_texture);
		
		case pick_colour_t		  : return new PickColourButton		  (init_shape, init_collision, init_texture);
		
		case slider_t 			  : return new SliderButton			  (init_shape, init_collision, init_texture);

		case show_colour_picker_t : return new ShowColourPickerButton (init_shape, init_collision, init_texture);

		case pick_brush_t		  : return new PickBrushButton		  (init_shape, init_collision, init_texture);

		case pick_size_t		  : return new PickSizeButton		  (init_shape, init_collision, init_texture);

		case canvas_ptr_t		  : return new CanvasPointerButton	  (init_shape, init_collision, init_texture);

		case turn_off_t			  : return new TurnOffButton		  (init_shape, init_collision, init_texture);

		default: cout << "Unkown type of functor: " << config->detailed_type << std::endl;
				 assert (!"OK");
	}

	return nullptr;
}


Shape* ElementCreator::NewShape (ElemConfig* config)
{
	if (!config->assets->shape)
	{
		return nullptr;
	}

	switch (config->assets->shape->GetType ())
	{
		case ShapeType::circle_t:	return new Circle (*(dynamic_cast <Circle*> (config->assets->shape)));

		case ShapeType::rect_t:		return new Rectangle (*(dynamic_cast <Rectangle*> (config->assets->shape)));

		default:	std::cout << "Wrong Shape Type." << std::endl;
					assert (!"OK");
	}
}

Shape* ElementCreator::NewCollision (ElemConfig* config)
{
	if (!config->assets->collision)
	{
		return nullptr;
	}

	switch (config->assets->collision->GetType ())
	{
		case ShapeType::circle_t:	return new Circle (*(dynamic_cast <Circle*> (config->assets->collision)));

		case ShapeType::rect_t:		return new Rectangle (*(dynamic_cast <Rectangle*> (config->assets->collision)));

		default:	std::cout << "Wrong Shape Type." << std::endl;
					assert (!"OK");
	}
}



/*** Parsing hierarchy ***/

void ElementCreator::ParseHierarchy (Application* App, FILE* hierarchy)
{
	char rubbish [200];
	char token   [200];

	while (fscanf (hierarchy, " %[^\n:]", token) != EOF)
	{
		fscanf (hierarchy, " %[\n:]", rubbish);

		if (strcmp (token, "Hierarchy") == 0)
		{
			GetHierarchy (App, hierarchy);
		}
		else
		{
			fscanf (hierarchy, "%[\n:]", token);
		}
	}
}

void ElementCreator::GetHierarchy (Application* App, FILE* hierarchy)
{
	char rubbish [200];
	char token   [200];


	int num_tabs = 0;

	char type = 0;
	char enabled = 0;


	int read = 0;

	while (read != EOF)
	{
		if (fscanf (hierarchy, "%[\t]%n", rubbish, &num_tabs) <= 0)
		{
			break;
		}
		num_tabs--;

		read = fscanf (hierarchy, "%[^( ]", token);
		read = fscanf (hierarchy, " ( %c , %c )", &type, &enabled);
		read = fscanf (hierarchy, "%[\n: ]", rubbish);


		ElemType elem_type = GetElemType (type);

		int detailed_type = DetailedTypeForName (elem_type, token);

		EnabledType enabled_type = GetEnabledType (enabled);

		Assets* assets = GetAssets (App, hierarchy);
		
		CreateElem (num_tabs, enabled_type, elem_type, detailed_type, assets);
	}
}




void ElementCreator::CreateElem (int num_tabs, EnabledType enabled_type, ElemType type, int detailed_type, Assets* assets)
{
	static ElemConfig* element[MAX_NESTING];

	ElemConfig* parent = nullptr;
	ElemConfig* child  = nullptr;

	if (num_tabs > 0)
	{
		parent = element[num_tabs - 1];

		child = new ElemConfig (enabled_type, type, detailed_type, assets, parent);

		parent->children.push_back (child);
	}
	else if (num_tabs == 0)
	{
		child = new ElemConfig (enabled_type, type, detailed_type, assets, nullptr);
	
		main_config = child;
	}


	default_config[type][detailed_type] = child;

	element[num_tabs] = child;
}




ElemType GetElemType (char type)
{
	ElemType elem_type = ElemType::wrong_t;

	switch (type)
	{
		case 'w':	elem_type = ElemType::window_t;
					break;

		case 'f':	elem_type = ElemType::button_t;
					break;

		default:	cout << "Unkown type specificator: \"" << type << "\"\n" << "It must bew \"w\" (window) or \"f\" (functor)" << std::endl;
					assert (!"OK"); 
	}

	return elem_type;
}

EnabledType GetEnabledType (char enabled)
{
	EnabledType enabled_type = EnabledType::disabled;

	switch (enabled)
	{
		case '+':	enabled_type = EnabledType::enabled;
					break;

		case '-':	enabled_type = EnabledType::disabled;
					break;

		case '?':	enabled_type = EnabledType::hidden;
					break;

		default:	cout << "Unkown enabled type: " << enabled << std::endl;
					assert (!"OK");
	}

	return enabled_type;
}


Assets* GetAssets (Application* App, FILE* hierarchy)
{
	char rubbish [200];
	char token   [200];


	char asset_type = 0;

	Shape* shape     = nullptr;
	Shape* collision = nullptr;
	Texture* texture = nullptr;


	int read = 0;

	read = fscanf (hierarchy, "%[{\n\t ]", rubbish);

	while (read != 0 && read != EOF)
	{
		read = fscanf (hierarchy, "%[sct]%[: ]", token, rubbish);

		asset_type = token[0];

		if (read == 0 || read == EOF)
		{
			break;
		}

		switch (asset_type)
		{
			case 's':	shape = GetShape (hierarchy);
						break;

			case 'c':	collision = GetShape (hierarchy);
						break;

			case 't':	texture = GetTexture (App, hierarchy);
						break;
		}
	}

	read = fscanf (hierarchy, "%[}\n ]", rubbish);


	return new Assets (shape, collision, texture);
}


Shape* GetShape (FILE* hierarchy)
{
	char token   [200];
	char rubbish [200];


	Point left;
	Point right;
	Point centre;

	float radius = 0.0;

	Colour colour (0, 0, 0, 0);

	ShapeType shape_type = ShapeType::wrong_t;



	int read = 0;


	read = fscanf (hierarchy, " %[^( ]", token);

	shape_type = ShapeTypeForName (token);

	switch (shape_type)
	{
		case ShapeType::circle_t : fscanf (hierarchy, " ( %f, %f, %f, %[a-zA-Z]", &centre.x, &centre.y, &radius, token);
								   break;

		case ShapeType::rect_t   : fscanf (hierarchy, " ( %f, %f, %f, %f, %[a-zA-Z]", &left.x, &left.y, &right.x, &right.y, token);
								   break;
		
		case ShapeType::rdrect_t : fscanf (hierarchy, " ( %f, %f, %f, %f, %f, %[a-zA-Z]", &left.x, &left.y, &right.x, &right.y, &radius, token);
								   break;

		default: cout << "Unkown type of shape: " << static_cast<int> (shape_type) << std::endl << std::flush;
				 assert (!"OK");
	}


	colour = ColourForName (token);


	Shape* init_shape = NULL;

	switch (shape_type)
	{
		case ShapeType::circle_t : init_shape = new Circle (centre, radius, colour);
								   break;

		case ShapeType::rect_t   : init_shape = new Rectangle (left, right, colour);
								   break;
		
		case ShapeType::rdrect_t : init_shape = new RoundedRect (left, right, radius, colour);
								   break;

		default: cout << "Unkown type of shape: " << static_cast<int> (shape_type) << std::endl << std::flush;
				 assert (!"OK");
	}

	read = fscanf (hierarchy, "%[)\n\t ]",  rubbish);


	return init_shape;
}



Texture* GetTexture (Application* App, FILE* hierarchy)
{
	char token   [200];
	char rubbish [200];


	int read = 0;

	read = fscanf (hierarchy, "%[\" ]", rubbish);

	read = fscanf (hierarchy, "%[^\"]", token);

	read = fscanf (hierarchy, "%[\"\n\t ]", rubbish);


	return TextureFromPath (App, token);	
}


int DetailedTypeForName (ElemType elem_type, const char* type_name)
{
	switch (elem_type)
	{
		case ElemType::window_t:	return WindowTypeForName (type_name);

		case ElemType::button_t:	return ButtonTypeForName (type_name);

		default: 	cout << "Unkown element type: " << static_cast <int> (elem_type) << std::endl;
					assert (!"OK");
	}
}

WindowType WindowTypeForName (const char* type_name)
{
	if (strcmp (type_name, "Main") == 0)
	{
		return main_t;
	}
	else if (strcmp (type_name, "Menu") == 0)
	{
		return menu_t;
	}
	else if (strcmp (type_name, "Palette") == 0)
	{
		return palette_t;
	}
	else if (strcmp (type_name, "Canvas") == 0)
	{
		return canvas_t;
	}
	else if (strcmp (type_name, "Title") == 0)
	{
		return title_t;
	}
	else if (strcmp (type_name, "ColourPicker") == 0)
	{
		return colour_picker_t;
	}
	else if (strcmp (type_name, "HuePalette") == 0)
	{
		return hue_palette_t;
	}
	else if (strcmp (type_name, "SizePalette") == 0)
	{
		return size_palette_t;
	}
	else if (strcmp (type_name, "Container") == 0)
	{
		return container_t;
	}
	else if (strcmp (type_name, "MenuPlace") == 0)
	{
		return menu_place_t;
	}
	else if (strcmp (type_name, "TurnOffWindow") == 0)
	{
		return turn_off_win_t;
	}
	else
	{
		printf ("window type name = \"%s\"\n", type_name);
		return wrong_wt;
	}
}

ButtonType ButtonTypeForName (const char* type_name)
{
	if (strcmp (type_name, "ShowMenu") == 0)
	{
		return show_menu_t;
	}
	else if (strcmp (type_name, "AddCanvas") == 0)
	{
		return add_canvas_t;
	}
	else if (strcmp (type_name, "ShowPalette") == 0)
	{
		return show_palette_t;
	}
	else if (strcmp (type_name, "Client") == 0)
	{
		return client_t;
	}
	else if (strcmp (type_name, "HideCanvas") == 0)
	{
		return hide_canvas_t;
	}
	else if (strcmp (type_name, "CloseCanvas") == 0)
	{
		return close_canvas_t;
	}
	else if (strcmp (type_name, "PickColour") == 0)
	{
		return pick_colour_t;
	}
	else if (strcmp (type_name, "Slider") == 0)
	{
		return slider_t;
	}
	else if (strcmp (type_name, "ShowColourPicker") == 0)
	{
		return show_colour_picker_t;
	}
	else if (strcmp (type_name, "PickBrush") == 0)
	{
		return pick_brush_t;
	}
	else if (strcmp (type_name, "PickSize") == 0)
	{
		return pick_size_t;
	}
	else if (strcmp (type_name, "CanvasPointer") == 0)
	{
		return canvas_ptr_t;
	}
	else if (strcmp (type_name, "TurnOff") == 0)
	{
		return turn_off_t;
	}
	else
	{
		printf ("functor type name = \"%s\"\n", type_name);
		return wrong_ft;
	}
}

ShapeType ShapeTypeForName (const char* type_name)
{
	if (strcmp (type_name, "Circle") == 0)
	{
		return ShapeType::circle_t;
	}
	else if (strcmp (type_name, "Rectangle") == 0)
	{
		return ShapeType::rect_t;
	}
	else if (strcmp (type_name, "RoundedRectangle") == 0)
	{
		return ShapeType::rdrect_t;
	}
	else
	{
		return ShapeType::wrong_t;
	}
}


Texture* TextureFromPath (Application* App, const char* path)
{
	if (strcmp (path, "No texture") != 0)
	{
		return App->LoadTexture (path);
	}

	return nullptr;
}