#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP


#include "Shapes.hpp"

enum class ElemType
{
	wrong_t  = -1,
	window_t =  0,
	button_t =  1
};


class Element
{

private:

	ElemType type;

	int      detailed_type;


	Shape* shape;

	Shape* collision;

	Texture* texture;


	Point old_location;


public:

	Element ();

	Element (ElemType init_type,
			 Shape*	  init_shape,
			 Shape*   init_collision,
			 Texture* init_texture);


	ElemType GetType ();

	int 	 GetDetailedType ();


	void SetDetailedType (int init_detailed_type);


	void Move (Point new_location, float* delta_x, float* delta_y);
	
	void Replace (float dx, float dy);

	bool Contains (Point location);

	void DrawLabel (Application* App, const char* label);

	void DrawTexture (Application* App, Texture* elem_texture);


	virtual void Draw (Application* App);

	virtual void Move (Point new_location);
};





#endif