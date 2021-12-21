#ifndef ELEM_TREE_HPP
#define ELEM_TREE_HPP



#include "../ElementCreator.hpp"



/*** Tree Node class ***/

class Node
{

public:

	EnabledType enabled;

	Element* elem;


	Node* parent;

	list <Node*> children;



	 Node ();

	 Node (EnabledType init_enabled, Element* init_elem, Node* init_parent);
	
	~Node ();


	bool Match (ElemType type, int detailed_type);

	bool Match (Element* presumed_elem);
};




/*** Tree class ***/

class ElemTree
{

private:

	Node* head;

	ElementCreator* elem_fabric;


	Node* GetParent (ElemConfig* config, Element* parent);

	Node* FindElement (ElemType type, int detailed_type, Element* elem);

	Node* FindElementInSubTree (Node* head, ElemType type, int detailed_type);

	Node* FindElementInSubTree (Node* head, Element* elem);


	void DeleteSubTree (Node* entry);


	bool ApplyClickForSubTree (Node* entry, Mouse* mouse);

	void UpdateSubTree        (Node* entry, Mouse* mouse);

	void DrawSubTree 		  (Node* entry, Application* App);


	void Move 				  (Node* entry, Point new_location);

	void MoveSubTree 		  (Node* entry, Point new_location);
	

	void Prioritize 	(Node* entry);

	void PrioritizeNode (Node* entry);


	void SetFunctorsInSubTree (Node* head, EventManager* event_manager);

	void SetFunctor  		  (Node* head, EventManager* event_manager);


	Functor* SetShowMenu 		 (Node* entry, EventManager* event_manager);

	Functor* SetAddCanvas 		 (Node* entry, EventManager* event_manager);

	Functor* SetShowPalette 	 (Node* entry, EventManager* event_manager);

	Functor* SetClient 			 (Node* entry, EventManager* event_manager);

	Functor* SetHideCanvas 		 (Node* entry, EventManager* event_manager);

	Functor* SetCloseCanvas 	 (Node* entry, EventManager* event_manager);

	Functor* SetPickColour		 (Node* entry, EventManager* event_manager);

	Functor* SetSlider 			 (Node* entry, EventManager* event_manager);

	Functor* SetShowColourPicker (Node* entry, EventManager* event_manager);

	Functor* SetPickBrush 		 (Node* entry, EventManager* event_manager);

	Functor* SetPickSize 		 (Node* entry, EventManager* event_manager);

	Functor* SetCanvasPointer 	 (Node* entry, EventManager* event_manager);

	Functor* SetTurnOff			 (Node* entry, EventManager* event_manager);



public:

	 ElemTree ();

	 ElemTree (Application* App, const char* hierarchy_conf);

	~ElemTree ();



	Element* AddElement (EnabledType enabled, ElemType type, int detailed_type, Element* parent);

	Element* AddElement (ElemConfig* config, Element* parent = nullptr);


	void ShowElement   (Element* entry);
	
	void HideElement   (Element* entry);

	void DeleteElement (Element* entry);





	void ApplyClick (Mouse* mouse)
	{
		ApplyClickForSubTree (head, mouse);
	}

	void Update (Mouse* mouse)
	{
		UpdateSubTree (head, mouse);
	}

	void Draw (Application* App)
	{
		DrawSubTree (head, App);
	}

	void SetFunctors (EventManager* event_manager)
	{
		SetFunctorsInSubTree (head, event_manager);
	}
};







#endif