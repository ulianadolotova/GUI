#include "Managers/Editor.hpp"


int main ()
{
	Editor* editor = new Editor ("GUI", "Application/Assets/Fonts/OpenSans-Bold.ttf",
								 "hierarchy_config.txt", "Drawing/drawing_config.txt");

	while (editor->IsRunning())
	{
		editor->Update ();
	}

	delete editor;
}