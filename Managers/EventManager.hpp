#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP



#include "../Elems/Windows.hpp"

#include <queue>

using std::queue;



enum EventType
{
	wrong_t          = -1,
	show_t           =  0,
	hide_t           =  1,
	add_t            =  2,
	delete_t         =  3,
	draw_t           =  4,
	set_colour_t     =  5,
	set_brush_t      =  6,
	set_size_t		 =  7,
	move_t           =  8,
	quit_t           =  9,
};


struct Event
{
	int type;

	struct
	{
		ElemType type;

		int detailed_type;


		Element* target;

		Element* parent;

	} elem;


	struct
	{
		Colour colour;

		int brush;

		int size;
	
	} drawing_context;


	struct
	{
		int dx;

		int dy;
	
	} move;


	Point click;
};



class EventManager
{

private:

	queue <Event> event_queue;


public:

	 EventManager ();

	~EventManager ();


	bool PollEvent (Event* event);

	void NewEvent (int type, ...);
};




#endif