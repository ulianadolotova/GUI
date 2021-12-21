#ifndef COOL_EDITOR_HPP
#define COOL_EDITOR_HPP



#include "ElemTree/ElemTree.hpp"



class Editor
{

private:

	Mouse* mouse;

	Application* app;

	ElemTree* elem_tree;

	EventManager* event_manager;

	DrawingContext* drawing_context;


	void StayUpdated ();

	void Draw ();

	void CheckClicks ();


public:

	 Editor ();

	 Editor (const char* windowname, const char* fontname, const char* hierarchy_conf, const char* drawing_conf);
	
	~Editor ();


	bool IsRunning ();

	void Update ();

};


#endif