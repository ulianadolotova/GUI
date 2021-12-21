#ifndef BUTTONS_HPP
#define BUTTONS_HPP


#include "Functors.hpp"


class Button : public Element
{

private:

	Functor* functor;


public:

	Button ();

	Button (ButtonType init_type,
			Shape*	   init_shape,
			Shape*	   init_collision,
			Texture*   init_texture);


	void Act    (Mouse* mouse);

	void Update ();


	int GetDetailedType ();

	bool Dummy ();

	void SetFunctor (Functor* init_functor);
};




class ShowMenuButton : public Button
{

public:

	ShowMenuButton ();

	ShowMenuButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);

	void Draw (Application* App) override;
};




class AddCanvasButton : public Button
{

public:

	AddCanvasButton ();

	AddCanvasButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);

	void Draw (Application* App) override;
};




class ShowPaletteButton : public Button
{

public:

	ShowPaletteButton ();

	ShowPaletteButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);
};




class ClientButton : public Button
{

private:

	Layer* layer;

	Texture* screen;

	void UpdateTexture (Application* App);


public:

	 ClientButton ();

	 ClientButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);

	~ClientButton ();


	void Draw (Application* App) override;

	void Move (Point new_location) override;

	void UserDraw (Application* App, DrawingContext* context, Point location);
};




class HideCanvasButton : public Button
{

public:

	HideCanvasButton ();

	HideCanvasButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);
};




class CloseCanvasButton : public Button
{

public:

	CloseCanvasButton ();

	CloseCanvasButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);
};




class PickColourButton : public Button
{

private:

	Texture* colour_range_texture;


public:

	HSV* colour_range;


	 PickColourButton ();

	 PickColourButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);

	~PickColourButton ();

	
	void Draw (Application* App) override;

	void Move (Point new_location) override;
};




class SliderButton : public Button
{

public:

	 SliderButton ();

	 SliderButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);
};




class ShowColourPickerButton : public Button
{

public:

	ShowColourPickerButton ();

	ShowColourPickerButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);
};




class PickBrushButton : public Button
{

public:

	int brush;


	PickBrushButton ();

	PickBrushButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);
};




class PickSizeButton : public Button
{

public:

	 PickSizeButton ();

	 PickSizeButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);
};




class CanvasPointerButton : public Button
{

public:

	char* name;


	 CanvasPointerButton ();

	 CanvasPointerButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);

	~CanvasPointerButton ();


	void Draw (Application* App) override;
};




class TurnOffButton : public Button
{

public:

	 TurnOffButton ();

	 TurnOffButton (Shape* init_shape, Shape* init_collision, Texture* init_texture);
};




#endif