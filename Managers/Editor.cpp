#include "Editor.hpp"




/*** Cool editor manager ***/

Editor::Editor ()
{
}

Editor::Editor (const char* windowname, const char* fontname, const char* hierarchy_conf, const char* drawing_conf)
{
	mouse = new Mouse (Point (0, 0), wrong_b);

	app = new Application (windowname, fontname);

	if (!app->IsRunning())
	{		
		assert (!"OK");
	}

	elem_tree = new ElemTree (app, hierarchy_conf);

	event_manager = new EventManager ();

	drawing_context = new DrawingContext (drawing_conf);
}

Editor::~Editor ()
{
	if (mouse)
	{
		delete mouse;
	}

	if (app)
	{
		delete app;
	}

	if (elem_tree)
	{
		delete elem_tree;
	}

	if (event_manager)
	{
		delete event_manager;
	}

	if (drawing_context)
	{
		delete drawing_context;
	}
}




bool Editor::IsRunning ()
{
	return app->IsRunning ();
}

void Editor::Update ()
{
	app->ShowFPS ();

	CheckClicks ();		
	StayUpdated ();
	Draw ();

	app->RefreshScreen ();
	app->ClearScreen ();

	app->CheckEvents (mouse);
}



void Editor::StayUpdated ()
{
	elem_tree->SetFunctors (event_manager);

	Event event;

	ClientButton* client = nullptr;

	while (event_manager->PollEvent (&event))
	{
		switch (event.type)
		{
			case show_t:		elem_tree->ShowElement (event.elem.target);
								break;

			case hide_t:		elem_tree->HideElement (event.elem.target);
								break;

			case add_t:			elem_tree->AddElement (EnabledType::enabled, event.elem.type, event.elem.detailed_type, event.elem.parent);
								break;

			case delete_t:  	elem_tree->DeleteElement (event.elem.target);
								break;

			case draw_t:		client = dynamic_cast <ClientButton*> (event.elem.target);
								client->UserDraw (app, drawing_context, event.click);
								break;

			case set_colour_t:	drawing_context->SetColour (event.drawing_context.colour);
								break;

			case move_t:		(event.elem.target)->Replace (event.move.dx, event.move.dy);
								break;

			case set_brush_t:	drawing_context->SetBrush (event.drawing_context.brush);
								break;

			case set_size_t:	drawing_context->SetSize (event.drawing_context.size);
								break;

			case quit_t:		app->TurnOff ();
								break;

			default:		
								break;
		}
	}
}


void Editor::Draw ()
{
	elem_tree->Draw (app);
}


void Editor::CheckClicks ()
{
	elem_tree->ApplyClick (mouse);
	
	elem_tree->Update (mouse);
}