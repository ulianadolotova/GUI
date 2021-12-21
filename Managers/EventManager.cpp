#include "EventManager.hpp"



/*** Event Manager Class ***/


EventManager::EventManager ()
{
}

EventManager::~EventManager ()
{
	while (!event_queue.empty())
	{
		event_queue.pop ();
	}
}


bool EventManager::PollEvent (Event* event)
{
	if (event_queue.empty ())
	{
		return false;
	}


	Event new_event = event_queue.front ();

	memcpy (event, &new_event, sizeof (Event));

	event_queue.pop ();


	return true;
}


void EventManager::NewEvent (int type, ...)
{    
    Event new_event;
    new_event.type = type;

    va_list   ap;
    va_start (ap, type);

    switch (type)
    {
    	case show_t 		:	new_event.elem.target = va_arg (ap, Element*);	
								break;

		case hide_t 		:	new_event.elem.target = va_arg (ap, Element*);
								break;

		case add_t 			:	new_event.elem.type = static_cast <ElemType> (va_arg (ap, int));
								new_event.elem.detailed_type = va_arg (ap, int);
								new_event.elem.parent = va_arg (ap, Element*);
								break;

		case delete_t		:	new_event.elem.target = va_arg (ap, Element*);
								break;

		case draw_t 		:	new_event.elem.target = va_arg (ap, Element*);
								new_event.click = va_arg (ap, Point);
								break;

		case set_colour_t	:	new_event.drawing_context.colour = va_arg (ap, Colour);
								break;

		case move_t			:	new_event.elem.target = va_arg (ap, Element*);
								new_event.move.dx = va_arg (ap, int);
								new_event.move.dy = va_arg (ap, int);
								break;

		case set_brush_t	:	new_event.drawing_context.brush = va_arg (ap, int);
								break;

		case set_size_t		:	new_event.drawing_context.size = va_arg (ap, int);
								break;

		case quit_t			:
								break;

		default:			cout << "Wrong event type: " << type << std::endl;
							assert (!"Correct event type\n");
							break;
    }

    va_end (ap);


    event_queue.push (new_event);
}