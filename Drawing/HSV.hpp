#ifndef HSV_HPP
#define HSV_HPP



#include "Drawing.hpp"
#include "cmath"



class ColourDistribution
{

private:

	int hue;

	float segment;


	int r;
	int g;
	int b;

	int* max;
	int* mid;
	int* min;


	void CountMaxMin (float value, float saturation);

	void CountMid ();


public:

	ColourDistribution ();

	ColourDistribution (int init_hue);


	Colour CountColour (float value, float saturation);
};



class HSV
{

private:

	int hue;			// 0 - 360
	
	uint32_t* colours;	// picker pixels

	Texture* texture;

	bool changed;


	Point location;

	float width;		// width of picker in pixels

	float height;		// height of picker in pixels
	


public:

	 HSV ();

	 HSV (int init_hue, Point init_location, int init_width, int init_height);

	~HSV ();



	void SetHue (int new_hue);

	void Replace (float dx, float dy);


	void GenerateColours ();

	Texture* GetTexture (Application* App);

	Colour PickColour (Point target_point);
};



#endif