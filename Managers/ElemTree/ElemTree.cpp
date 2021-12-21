#include "ElemTree.hpp"




/*** Tree Node class ***/

Node::Node ()
{
	enabled = EnabledType::disabled;

	elem    = nullptr;

	parent  = nullptr;
}

Node::Node (EnabledType init_enabled, Element* init_elem, Node* init_parent)
{
	enabled = init_enabled;

	elem    = init_elem;

	parent  = init_parent;
}

Node::~Node ()
{
	if (elem)
	{
		delete elem;

		elem = nullptr;
	}

	for (Node* child : children)
	{
		delete child;
	}

	children.clear ();
}


bool Node::Match (ElemType presumed_type, int presumed_detailed_type)
{
	return (presumed_type 		   == elem->GetType ()) &&
		   (presumed_detailed_type == elem->GetDetailedType ());
}

bool Node::Match (Element* presumed_elem)
{
	return (presumed_elem == elem);
}




/*** Tree class ***/

ElemTree::ElemTree ()
{
	head = nullptr;

	elem_fabric = nullptr;
}


ElemTree::ElemTree (Application* App, const char* hierarchy_conf)
{
	head = nullptr;

	elem_fabric = new ElementCreator (App, hierarchy_conf);

	ElemConfig* main = elem_fabric->GetMainConfig ();

	AddElement (main, nullptr);
}


ElemTree::~ElemTree ()
{
	if (head)
	{
		delete head;
	}

	if (elem_fabric)
	{
		delete elem_fabric;
	}
}



/*** Extrenal Manipulation with elements ***/

Element* ElemTree::AddElement (EnabledType enabled, ElemType type, int detailed_type, Element* parent)
{
	Element* result = nullptr;

	ElemConfig* config = elem_fabric->GetConfig (type, detailed_type);

	if (type == ElemType::window_t && detailed_type == canvas_t)
	{
		config = config->parent;
	
		config->enabled = enabled;

		result = AddElement (config, parent);

		AddElement (EnabledType::enabled, ElemType::button_t, canvas_ptr_t, nullptr);
	}
	else
	{
		config->enabled = enabled;

		result = AddElement (config, parent);
	}

	return result;

}


Element* ElemTree::AddElement (ElemConfig* config, Element* parent)
{
	assert (config);


	Node* parent_node = GetParent (config, parent);


	Element* new_elem = elem_fabric->NewElement (config);

	Node* new_node = new Node (config->enabled, new_elem, parent_node);


	if (parent_node)
	{
		parent_node->children.push_front (new_node);
	}
	else
	{
		head = new_node;
	}


	for (ElemConfig* child : config->children)
	{
		if (child->enabled != EnabledType::disabled)
		{
			AddElement (child, new_elem);
		}
	}


	return new_elem;
}

Node* ElemTree::GetParent (ElemConfig* config, Element* parent)
{
	Node* parent_node = nullptr;

	if (parent)
	{
		parent_node = FindElement (ElemType::wrong_t, -1, parent);
	}
	else if (config->parent)
	{
		parent_node = FindElement (config->parent->type, config->parent->detailed_type, parent);
	}

	return parent_node;
}


void ElemTree::ShowElement (Element* entry)
{
	Node* target = FindElement (ElemType::wrong_t, -1, entry);

	int detailed_type = entry->GetDetailedType ();

	switch (detailed_type)
	{
		case canvas_t:			target = target->parent;
								break;

		case colour_picker_t:	target = target->parent;
								break;

		default:
								break;
	}

	Prioritize (target);

	target->enabled = EnabledType::enabled;
}

void ElemTree::HideElement (Element* entry)
{
	Node* target = FindElement (ElemType::wrong_t, -1, entry);
	
	int detailed_type = entry->GetDetailedType ();

	switch (detailed_type)
	{
		case canvas_t:			target = target->parent;
								break;

		case colour_picker_t:	target = target->parent;
								break;

		default:
								break;
	}

	target->enabled = EnabledType::disabled;
}

void ElemTree::DeleteElement (Element* entry)
{
	Node* target = FindElement (ElemType::wrong_t, -1, entry);

	int detailed_type = entry->GetDetailedType ();

	switch (detailed_type)
	{
		case canvas_t:			target = target->parent;
								break;

		default:
								break;
	}

	DeleteSubTree (target);
}

void ElemTree::DeleteSubTree (Node* target)
{
	Node* parent = target->parent;

	if (parent)
	{
		for (auto it = parent->children.begin (); it != parent->children.end (); it++)
		{
			Node* child = *it;
			
			if (child == target)
			{
				parent->children.erase (it);
				break;
			}
		}		
	}

	delete target;
}



/*** Internal Manipulation with elements ***/

Node* ElemTree::FindElement (ElemType type, int detailed_type, Element* elem)
{
	if (elem)
	{
		return FindElementInSubTree (head, elem);
	}

	return FindElementInSubTree (head, type, detailed_type);
}

Node* ElemTree::FindElementInSubTree (Node* entry, ElemType type, int detailed_type)
{
	if (!entry || entry->Match (type, detailed_type))
	{
		return entry;
	}

	Node* target = nullptr;

	for (Node* child : entry->children)
	{
		target = FindElementInSubTree (child, type, detailed_type);

		if (target)
		{
			break;
		}
	}

	return target;
}

Node* ElemTree::FindElementInSubTree (Node* entry, Element* elem)
{
	if (!entry || entry->Match (elem))
	{
		return entry;
	}

	Node* target = nullptr;

	for (Node* child : entry->children)
	{
		target = FindElementInSubTree (child, elem);

		if (target)
		{
			break;
		}
	}

	return target;
}



bool ElemTree::ApplyClickForSubTree (Node* entry, Mouse* mouse)
{
	if (entry->enabled == EnabledType::enabled && entry->elem->Contains (mouse->location))
	{
		for (Node* child : entry->children)
		{
			if (ApplyClickForSubTree (child, mouse))
			{
				return true;
			}
		}

		if (entry->elem->GetType () == ElemType::button_t)
		{
			if (mouse->LeftClick ())
			{
				PrioritizeNode (entry);
			}

			Button* button = dynamic_cast <Button*> (entry->elem);
			
			button->Act (mouse);
			
			return true;
		}
		else if (entry->elem->GetType () == ElemType::window_t)
		{
			if (mouse->LeftClick ())
			{
				Prioritize (entry);

				Move (entry, mouse->location);

				return true;
			}
		}
	}
	
	return false;
}


void ElemTree::UpdateSubTree (Node* entry, Mouse* mouse)
{
	for (Node* child : entry->children)
	{
		UpdateSubTree (child, mouse);
	}


	if (entry->elem->GetType () == ElemType::button_t)
	{
		Button* button = dynamic_cast <Button*> (entry->elem);
		
		if (!entry->elem->Contains (mouse->location))
		{
			button->Update ();
		}
	}
}


void ElemTree::DrawSubTree (Node* entry, Application* App)
{
	if (!entry || entry->enabled != EnabledType::enabled)
	{
		return;
	}
	
	entry->elem->Draw (App);

	for (auto it = entry->children.rbegin (); it != entry->children.rend (); ++it)
	{
		Node* child = *it;

		DrawSubTree (child, App);
	}
}


void ElemTree::Move (Node* entry, Point new_location)
{
	int detailed_type = entry->elem->GetDetailedType ();

	switch (detailed_type)
	{
		case container_t:		MoveSubTree (entry, new_location);
								break;

		case title_t:			MoveSubTree (entry->parent->parent, new_location);
								break;

		case colour_picker_t:	MoveSubTree (entry->parent, new_location);
								break;

		// case menu_t:			MoveSubTree (entry, new_location);
		// 						break;

		default:
								break;
	}
}

void ElemTree::MoveSubTree (Node* entry, Point new_location)
{
	entry->elem->Move (new_location);

	for (Node* child : entry->children)
	{
		MoveSubTree (child, new_location);
	}
}


void ElemTree::Prioritize (Node* entry)
{
	Node* target = nullptr;

	int detailed_type = entry->elem->GetDetailedType ();

	switch (detailed_type)
	{
		case container_t:		target = entry;
								break;

		case palette_t:			target = entry;
								break;

		case title_t:			target = entry->parent->parent;
								break;

		case colour_picker_t:	target = entry->parent;
								break;

		case menu_t:			target = entry;
								break;

		default:
								break;
	}

	PrioritizeNode (target);
}

void ElemTree::PrioritizeNode (Node* entry)
{
	if (!entry || !entry->parent)
	{
		return;
	}

	Node* parent = entry->parent;
	
	PrioritizeNode (parent);
	
	for (auto it = parent->children.begin (); it != parent->children.end (); ++it)
	{
		if ((*it) == entry)
		{
			parent->children.erase (it);
			break;
		}
	}

	parent->children.push_front (entry);
}


void ElemTree::SetFunctorsInSubTree (Node* entry, EventManager* event_manager)
{
	if (!entry)
	{
		return;
	}

	if (entry->elem->GetType () == ElemType::button_t)
	{
		SetFunctor (entry, event_manager);
	}

	for (Node* child : entry->children)
	{
		SetFunctorsInSubTree (child, event_manager);
	}
}

void ElemTree::SetFunctor (Node* entry, EventManager* event_manager)
{
	Functor* functor = nullptr;

	Button* button = dynamic_cast <Button*> (entry->elem);

	if (!button->Dummy ())
	{
		return;
	}

	switch (entry->elem->GetDetailedType ())
	{
		case show_menu_t   :		functor = SetShowMenu (entry, event_manager);
									break;


		case add_canvas_t   :		functor = SetAddCanvas (entry, event_manager);
								
									break;


		case show_palette_t :		functor = SetShowPalette (entry, event_manager);	
								
									break;

		
		case client_t :				functor = SetClient (entry, event_manager);
								
									break;


		case hide_canvas_t  :		functor = SetHideCanvas (entry, event_manager);
								
									break;


		case close_canvas_t :		functor = SetCloseCanvas (entry, event_manager);
								
									break;


		case pick_colour_t:			functor = SetPickColour (entry, event_manager);

									break;


		case slider_t:				functor = SetSlider (entry, event_manager);

									break;


		case show_colour_picker_t:	functor = SetShowColourPicker (entry, event_manager);

									break;


		case pick_brush_t:			functor = SetPickBrush (entry, event_manager);

									break;


		case pick_size_t:			functor = SetPickSize (entry, event_manager);

									break;


		case canvas_ptr_t:			functor = SetCanvasPointer (entry, event_manager);

									break;


		case turn_off_t:			functor = SetTurnOff (entry, event_manager);

									break;


		default: cout << "Unkown type of functor: " << entry->elem->GetDetailedType () << std::endl << std::flush;
				 assert (!"OK");
	}

	button->SetFunctor (functor);
}


Functor* ElemTree::SetShowMenu (Node* entry, EventManager* event_manager)
{
	Node* help_elem = FindElementInSubTree (entry->parent, ElemType::window_t, menu_t);

	Element* target = help_elem->elem;

	Functor* functor = new ShowMenu (target, event_manager);

	return functor;
}

Functor* ElemTree::SetAddCanvas (Node* entry, EventManager* event_manager)
{
	Element* parent = entry->parent->parent->elem;

	Functor* functor = new AddCanvas (parent, event_manager);

	return functor;
}

Functor* ElemTree::SetShowPalette (Node* entry, EventManager* event_manager)
{
	Node* help_elem = FindElementInSubTree (entry->parent, ElemType::window_t, palette_t);

	Element* target = help_elem->elem;

	Functor* functor = new ShowPalette (target, event_manager);

	return functor;
}

Functor* ElemTree::SetClient (Node* entry, EventManager* event_manager)
{
	Element* target = entry->elem;

	Functor* functor = new Client (target, event_manager);
	
	return functor;
}

Functor* ElemTree::SetHideCanvas (Node* entry, EventManager* event_manager)
{
	Element* target = entry->parent->elem;

	Functor* functor = new HideCanvas (target, event_manager);

	return functor;
}

Functor* ElemTree::SetCloseCanvas (Node* entry, EventManager* event_manager)
{
	Node* help_elem = FindElement (ElemType::button_t, canvas_ptr_t, nullptr);

	Element* canvas_ptr_target = help_elem->elem;
	
	Element* canvas_target = entry->parent->elem;

	Functor* functor = new CloseCanvas (canvas_target, canvas_ptr_target, event_manager);
	
	return functor;
}

Functor* ElemTree::SetPickColour (Node* entry, EventManager* event_manager)
{
	Node* help_elem = FindElementInSubTree (entry->parent, ElemType::window_t, hue_palette_t);

	HuePalette* palette = dynamic_cast <HuePalette*> (help_elem->elem);

	PickColourButton* button = dynamic_cast <PickColourButton*> (entry->elem);

	Functor* functor = new PickColour (&((palette)->hue), 
					     		   	   button->colour_range,
					      		   	   event_manager);
	return functor;
}

Functor* ElemTree::SetPickSize (Node* entry, EventManager* event_manager)
{
	Node* help_elem = FindElementInSubTree (entry->parent, ElemType::window_t, size_palette_t);

	SizePalette* palette = dynamic_cast <SizePalette*> (help_elem->elem);

	Functor* functor = new PickSize (&((palette)->size), 
									 event_manager);
	return functor;
}

Functor* ElemTree::SetSlider (Node* entry, EventManager* event_manager)
{
	Functor* functor = nullptr;


	Node* help_elem = FindElementInSubTree (entry->parent, ElemType::window_t, hue_palette_t);

	if (!help_elem)
	{
		help_elem = FindElementInSubTree (entry->parent, ElemType::window_t, size_palette_t);

		SizePalette* palette = dynamic_cast <SizePalette*> (help_elem->elem);
		
		functor = new Slider (palette->sliding_context, entry->elem, event_manager);
	}
	else
	{
		HuePalette* palette = dynamic_cast <HuePalette*> (help_elem->elem);
		
		functor = new Slider (palette->sliding_context, entry->elem, event_manager);
	}

	return functor;
}

Functor* ElemTree::SetShowColourPicker (Node* entry, EventManager* event_manager)
{
	Node* help_elem = FindElementInSubTree (entry->parent->parent, ElemType::window_t, colour_picker_t);

	// Here we need parent cause it's container
	Element* target = help_elem->parent->elem;

	Functor* functor = new ShowColourPicker (target, event_manager);
	
	return functor;
}

Functor* ElemTree::SetPickBrush (Node* entry, EventManager* event_manager)
{
	PickBrushButton* button = dynamic_cast <PickBrushButton*> (entry->elem);

	Functor* functor = new PickBrush (button->brush, event_manager);

	return functor;
}

Functor* ElemTree::SetCanvasPointer (Node* entry, EventManager* event_manager)
{
	Node* help_elem = FindElement (ElemType::window_t, canvas_t, nullptr);

	Element* target = help_elem->elem;

	Functor* functor = new CanvasPointer (target, event_manager);

	return functor;
}

Functor* ElemTree::SetTurnOff (Node* entry, EventManager* event_manager)
{
	Node* help_elem = FindElement (ElemType::window_t, turn_off_win_t, nullptr);

	TurnOffWindow* target = dynamic_cast <TurnOffWindow*> (help_elem->elem);

	Functor* functor = new TurnOff (&(target->visibility), event_manager);

	return functor;
}