#include <algorithm>
#include <cassert>
#include <ctime>
#include "Application.hpp"




/*** Points ***/

Point::Point ()
{
	x = 0.0;
	y = 0.0;
}

Point::Point (float a, float b)
{
	x = a;
	y = b;
}

bool operator== (const Point& left, const Point& right)
{
	return (left.x == right.x) && (left.y == right.y);
}

bool operator!= (const Point& left, const Point& right)
{
	return !(left == right);
}


/*** Colour ***/

Colour::Colour ()
{
	r = g = b = a = 0;
}

Colour::Colour (uint8_t init_r, uint8_t init_g, uint8_t init_b, uint8_t init_a)
{
	r = init_r;
	g = init_g;
	b = init_b;
	a = init_a;
}

Colour::Colour (uint32_t init_mask)
{
	int shift = 24;

	r = (init_mask << shift) >> 24;
	shift -= 8;

	g = (init_mask << shift) >> 24;
	shift -= 8;

	b = (init_mask << shift) >> 24;
	shift -= 8;

	a = (init_mask << shift) >> 24;
}


uint32_t Colour::ToMask ()
{
	uint32_t mask = 0;

	int shift = 0;


	mask  += r << shift;
	shift += 8;

	mask  += g << shift;
	shift += 8;

	mask  += b << shift;
	shift += 8;

	mask  += a << shift;


	return mask;
}


Colour::operator SDL_Color()
{
	return {r, g, b, a};
}




/*** Borders ***/

Borders::Borders (Point l_corner, Point r_corner, Colour init_colour)
{
	hit_coef = 1;
	
	left  = l_corner.x + BORDERS_WIDTH;
	right = r_corner.x - BORDERS_WIDTH;

	down = std::max (l_corner.y, r_corner.y) - BORDERS_WIDTH;
	up   = std::min (l_corner.y, r_corner.y) + BORDERS_WIDTH;

	colour = init_colour;
}




/*** Mouse ***/

Mouse::Mouse ()
{

}

Mouse::Mouse (Point init_location, MouseState init_state)
{
	location = init_location;

	state = init_state;
}


void Mouse::SetState (MouseState new_state)
{
	state = new_state;
}


bool Mouse::LeftClick  ()
{
	return (state == left_b);
}

bool Mouse::MidClick   ()
{
	return (state == mid_b);
}

bool Mouse::RightClick ()
{
	return (state == right_b);
}




/*** Application ***/


/*** Members ***/

void Application::TurnOff ()
{
	Running = false;
}

bool Application::IsRunning ()
{
	return Running;
}



/*** Initialization ***/

Application::Application (const char* windowname, const char* fontname)
{
	Running = true;

	SDL_Init(SDL_INIT_VIDEO);

	CreateWindow (windowname);

	CreateRenderer ();

	FontsInit (fontname);

	SetColour (Renderer, DefaultColour);
	
	SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}


void Application::CreateWindow (const char* windowname)
{
	int WindowFlags = 0;

	Window = SDL_CreateWindow (windowname, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, WindowFlags);

	if (!Window)
	{
		printf ("\n\n\nFailed to create %d x %d window:\n%s\n\n", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());
		Running = false;
		
		return;
	}
}

void Application::CreateRenderer ()
{
	int RendererFlags = 0;

	Renderer = SDL_CreateRenderer (Window, -1, RendererFlags);

	if (!Renderer)
	{
		printf ("\n\n\nFailed to create renderer:\n%s\n\n", SDL_GetError());
		Running = false;

		return;
	}
    
    SDL_RenderClear (Renderer);
}

void Application::FontsInit (const char* fontname)
{
	if (!TTF_WasInit() && TTF_Init() == -1)
	{
    	printf ("TTF_Init: %s\n", TTF_GetError());
    	Running = false;

    	return;
	}

	Font = TTF_OpenFont (fontname, 72);
}



/*** Evenets ***/

void Application::ShowFPS ()
{
	static clock_t prev_time = clock ();

	clock_t time = clock ();

	double drawing_time = ((double) (time - prev_time)) / CLOCKS_PER_SEC;

	double fps = 1.0 / drawing_time;

	prev_time = time;
	

	char title[MAX_TITLE_SIZE];

	if (fps <= 1.0)
	{
		snprintf (title, MAX_TITLE_SIZE, "FPS: %.3f", fps);
	}
	else
	{
		snprintf (title, MAX_TITLE_SIZE, "FPS: %.2f", fps);
	}

	SDL_SetWindowTitle (Window, title);
}

void Application::CheckEvents (Mouse* mouse)
{
	static bool button_pressed = false;
	static MouseState button = wrong_b;

	mouse->SetState (wrong_b);
	
	
	SDL_Event event;

	while (SDL_PollEvent (&event))
	{
		switch (event.type)
		{
			case SDL_MOUSEMOTION:		mouse->location.x = event.button.x;
										mouse->location.y = event.button.y;										
										if (button_pressed)
										{
											mouse->SetState (button);
										}
										return;


			case SDL_MOUSEBUTTONDOWN:	button_pressed = true;
										mouse->location.x = event.button.x;
										mouse->location.y = event.button.y;
										button = static_cast<MouseState> (event.button.button);
										mouse->SetState (button);
										return;


			case SDL_MOUSEBUTTONUP:		button_pressed = false;
										mouse->location.x = event.button.x;
										mouse->location.y = event.button.y;
										button = wrong_b;
										mouse->SetState (button);
										return;

			case SDL_QUIT:				Running = false;
										return;
			
			default:	break;
		}

	}
}

void Application::Wait (float seconds)
{
	int m_seconds = (int) (1000 * seconds);

	SDL_Delay (m_seconds);
}



/*** Quit ***/

Application::~Application ()
{
	if (Renderer)
	{
		SDL_DestroyRenderer (Renderer);
	}

	if (Window)
	{
		SDL_DestroyWindow (Window);
	}

	SDL_Quit ();
}



/*** Screen Management ***/


void Application::RefreshScreen ()
{
	SDL_RenderPresent (Renderer);
}

void Application::ClearScreen ()
{
	SetColour (Renderer, DefaultColour);
	
	SDL_RenderClear (Renderer);
}



/*** Drawing ***/


Texture* Application::LoadTexture (const char* image)
{
	SDL_Surface *image_surface = SDL_LoadBMP(image);

	if (!image_surface)
	{
		printf ("Error, while loading image: %s\n", TTF_GetError ());

		return NULL;
	}

	Texture* image_texture = SDL_CreateTextureFromSurface (Renderer, image_surface);

	if (!image_texture)
	{
		printf ("Error, while loading image: %s\n", TTF_GetError ());

		return NULL;	
	}

	SDL_FreeSurface (image_surface);

	return image_texture;
}


void Application::UpdateTexture (Texture** texture, void* data, int width, int height)
{
	int pitch = 4 * width;

	if (!(*texture))
	{
		*texture = SDL_CreateTexture (Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);

		SDL_SetTextureBlendMode (*texture, SDL_BLENDMODE_BLEND);
	}

	SDL_UpdateTexture (*texture, NULL, data, pitch);
}



void Application::DrawTexture (Texture* texture, float start_x, float start_y, float end_x, float end_y)
{
	SDL_FRect target_location;
	target_location.x = start_x;
	target_location.w = end_x - start_x;
	target_location.y = (start_y < end_y) ? start_y : end_y;
	target_location.h = (start_y < end_y) ? end_y - start_y : start_y - end_y;

	SDL_RenderCopyF (Renderer, texture, NULL, &target_location);
}


void Application::DrawText (const char* text, float start_x, float start_y, float end_x, float end_y, Colour colour)
{
	SetColour (Renderer, colour);

	SDL_FRect target_location;
	target_location.x = start_x;
	target_location.w = end_x - start_x;
	target_location.y = (start_y < end_y) ? start_y : end_y;
	target_location.h = (start_y < end_y) ? end_y - start_y : start_y - end_y;


	SDL_Surface* text_surface = TTF_RenderText_Solid (Font, text, colour);

	if (!text_surface)
	{
		printf ("Error, while printing text: %s\n", TTF_GetError ());

		return;
	}


	Texture* text_texture = SDL_CreateTextureFromSurface (Renderer, text_surface);

	if (!text_texture)
	{
		printf ("Error, while printing text: %s\n", TTF_GetError ());

		return;	
	}


	SDL_RenderCopyF (Renderer, text_texture, NULL, &target_location);

	SDL_FreeSurface (text_surface);
	SDL_DestroyTexture (text_texture);
}


void Application::DrawLine (float start_x, float start_y, float end_x, float end_y, Colour colour)
{
	SetColour (Renderer, colour);
	
	SDL_RenderDrawLineF (Renderer, start_x, start_y, end_x, end_y);
}


void Application::DrawRect (float start_x, float start_y, float end_x, float end_y, Colour colour)
{
	SetColour (Renderer, colour);

	SDL_FRect rectangle;
	rectangle.x = start_x;
	rectangle.w = end_x - start_x;
	rectangle.y = (start_y < end_y) ? start_y : end_y;
	rectangle.h = (start_y < end_y) ? end_y - start_y : start_y - end_y;

	const SDL_FRect* rect_ptr = &rectangle;

	SDL_RenderDrawRectF (Renderer, rect_ptr);
}


void Application::DrawCircle (float centre_x, float centre_y, float radius, Colour colour)
{
	SetColour (Renderer, colour);

	float x = radius;		// Starting with (r, 0) and going up
	float y = 0;

	float P = 1 - radius;

	// y can only increase
	// and x is decreasing till x < y
	// that way we making iterations for one eigths of dots

	while (x >= y)
	{
		SDL_RenderDrawPointF (Renderer, centre_x + x, centre_y + y);
		SDL_RenderDrawPointF (Renderer, centre_x + x, centre_y - y);
		SDL_RenderDrawPointF (Renderer, centre_x - x, centre_y + y);
		SDL_RenderDrawPointF (Renderer, centre_x - x, centre_y - y);

		SDL_RenderDrawPointF (Renderer, centre_x + y, centre_y + x);
		SDL_RenderDrawPointF (Renderer, centre_x + y, centre_y - x);
		SDL_RenderDrawPointF (Renderer, centre_x - y, centre_y + x);
		SDL_RenderDrawPointF (Renderer, centre_x - y, centre_y - x);

		if (P <= 0)
		{
			P += 2*y + 1;
		}
		else
		{
			x--;

			P+= 2*y - 2*x + 1;
		}

		y++;
	}
}


void Application::DrawRoundedRect (float start_x, float start_y, float end_x, float end_y, float radius, Colour colour)
{
	SetColour (Renderer, colour);

	float x = radius;		// Starting with (r, 0) and going up
	float y = 0;

	float P = 1 - radius;

	// y can only increase
	// and x is decreasing till x < y
	// that way we making iterations for one eigths of dots

	while (x >= y)
	{
		SDL_RenderDrawPointF (Renderer, end_x   - radius + x, end_y   - radius + y);
		SDL_RenderDrawPointF (Renderer, end_x   - radius + x, start_y + radius - y);
		SDL_RenderDrawPointF (Renderer, start_x + radius - x, end_y   - radius + y);
		SDL_RenderDrawPointF (Renderer, start_x + radius - x, start_y + radius - y);

		SDL_RenderDrawPointF (Renderer, end_x   - radius + y, end_y   - radius + x);
		SDL_RenderDrawPointF (Renderer, end_x   - radius + y, start_y + radius - x);
		SDL_RenderDrawPointF (Renderer, start_x + radius - y, end_y   - radius + x);
		SDL_RenderDrawPointF (Renderer, start_x + radius - y, start_y + radius - x);

		if (P <= 0)
		{
			P += 2*y + 1;
		}
		else
		{
			x--;

			P+= 2*y - 2*x + 1;
		}

		y++;
	}
	
	SDL_RenderDrawLineF (Renderer, start_x + radius, start_y, end_x - radius, start_y);
	SDL_RenderDrawLineF (Renderer, start_x + radius, end_y,   end_x - radius, end_y);
	
	SDL_RenderDrawLineF (Renderer, start_x, start_y + radius, start_x, end_y - radius);
	SDL_RenderDrawLineF (Renderer, end_x,   start_y + radius, end_x,   end_y - radius);
}


void SetColour (SDL_Renderer* Renderer, Colour colour)
{
	SDL_SetRenderDrawColor (Renderer, colour.r, colour.g, colour.b, colour.a);
}



/*** Mouse Tracking ***/

uint32_t GetClick (float* mouse_x, float* mouse_y)
{
	int x, y;

	SDL_PumpEvents ();

	uint32_t buttons_mask = SDL_GetMouseState (&x, &y);

	*mouse_x = (float) x;
	*mouse_y = (float) y;

	return buttons_mask;
}


bool IsLeft (uint32_t buttons_mask)
{
	if (buttons_mask & SDL_BUTTON_LMASK != 0)
	{
		return true;
	}

	return false;
}