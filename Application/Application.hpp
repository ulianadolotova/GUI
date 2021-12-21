#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

using std::cin;
using std::cout;


typedef SDL_Texture Texture;


enum MouseState
{
	wrong_b = 0,
	left_b  = SDL_BUTTON_LEFT,
	mid_b   = SDL_BUTTON_MIDDLE,
	right_b = SDL_BUTTON_RIGHT
};


struct Colour
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	Colour ();

	Colour (uint8_t init_r, uint8_t init_g, uint8_t init_b, uint8_t init_a);

	Colour (uint32_t init_mask);


	uint32_t ToMask ();


	operator SDL_Color();
};


struct Point
{
	float x;
	float y;

	Point ();

	Point (float a, float b);
};

bool operator== (const Point& left, const Point& right);
bool operator!= (const Point& left, const Point& right);


struct Borders
{
	float hit_coef;

	int left;
	int right;
	
	int up;
	int down;

	Colour colour;

	Borders (Point l_corner, Point r_corner, Colour init_colour);
};


class Mouse
{

private:

	MouseState state;


public:

	Point location;


	Mouse ();

	Mouse (Point init_location, MouseState init_state);

	void SetState (MouseState new_state);

	bool LeftClick  ();
	bool MidClick   ();
	bool RightClick ();
};


class Application
{

private:

	bool Running;
	
	SDL_Window*   Window;
	SDL_Renderer* Renderer;

	TTF_Font* Font;


public:

	/*** Members ***/

	void TurnOff ();

	bool IsRunning ();


	/*** Construction ***/

	 Application (const char* windowname, const char* fontname);
	~Application ();

	void CreateWindow   (const char* windowname);
	void CreateRenderer ();
	void FontsInit 		(const char* fontname);
	

	/*** Events ***/

	void ShowFPS ();

	void CheckEvents (Mouse* mouse);

	void Wait (float seconds);

	
	/*** Screen Management ***/

	void RefreshScreen ();

	void ClearScreen ();


	/*** Drawing ***/


	Texture* LoadTexture (const char* image);

	void UpdateTexture (Texture** texture, void* data, int width, int height);
	

	void DrawTexture (Texture* texture, float start_x, float start_y, float end_x, float end_y);

	inline void DrawTexture (Texture* texture, Point lcorner, Point rcorner)
	{
		DrawTexture (texture, lcorner.x, lcorner.y, rcorner.x, rcorner.y);
	}


	void DrawText (const char* text, float start_x, float start_y, float end_x, float end_y, Colour colour);

	inline void DrawText (const char* text, Point lcorner, Point rcorner, Colour colour)
	{
		DrawText (text, lcorner.x, lcorner.y, rcorner.x, rcorner.y, colour);
	}


	void DrawLine (float start_x, float start_y, float end_x, float end_y, Colour colour);

	inline void DrawLine (Point a, Point b, Colour colour)
	{
		DrawLine (a.x, a.y, b.x, b.y, colour);
	}


	void DrawRect (float start_x, float start_y, float end_x, float end_y, Colour colour);

	inline void DrawRect (Point a, Point b, Colour colour)
	{
		DrawRect (a.x, a.y, b.x, b.y, colour);
	}


	void DrawCircle (float centre_x, float centre_y, float radius, Colour colour);
	
	inline void DrawCircle (Point centre, float radius, Colour colour)
	{
		DrawCircle (centre.x, centre.y, radius, colour);
	}
	

	void DrawRoundedRect (float start_x, float start_y, float end_x, float end_y, float radius, Colour colour);

	inline void DrawRoundedRect (Point a, Point b, float radius, Colour colour)
	{
		DrawRoundedRect (a.x, a.y, b.x, b.y, radius, colour);
	}
};



const int MAX_TITLE_SIZE = 20;

const int WINDOW_WIDTH  = 1920;
const int WINDOW_HEIGHT = 1080;

const int BORDERS_WIDTH = 5;


#define DEF_COLOUR(name, r, g, b, empty)						\
	const Colour name (r, g, b, SDL_ALPHA_OPAQUE);

#include "Colours.hpp"

#undef DEF_COLOUR


const Colour DefaultColour = Black;



void SetColour (SDL_Renderer* Renderer, Colour colour);

uint32_t GetClick (float* mouse_x, float* mouse_y);

bool IsLeft (uint32_t buttons_mask);



#endif