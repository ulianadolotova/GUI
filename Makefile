Run: Main
	bin/GUI

Main: ScreenManagement DrawingStuff ElementsLibrary Managers
	g++ $(Screen) $(Drawing) $(Elements) $(Managers) $(SDL_FLAGS) -o bin/GUI




# There are files, that handles all actions with screen

ScreenManagement: Brushes Drawing Application

Assets: Assets/Assets.cpp
	g++ -c Assets/Assets.cpp -o bin/Assets.o

Application: Application/Application.cpp
	g++ -c Application/Application.cpp -o bin/Application.o


Screen = bin/Application.o

SDL_FLAGS = -lSDL2 -lSDL2_ttf -LSDL2_image




# There are files, that contains all classes connected with user drawing

DrawingStuff: Brushes Drawing HSV

Brushes: Drawing/Brushes.cpp
	g++ -c Drawing/Brushes.cpp -o bin/Brushes.o

Drawing: Drawing/Drawing.cpp
	g++ -c Drawing/Drawing.cpp -o bin/Drawing.o

HSV: Drawing/HSV.cpp
	g++ -c Drawing/HSV.cpp -o bin/HSV.o

Drawing = bin/Brushes.o bin/Drawing.o bin/HSV.o



# There are files that contains elements (windows & buttons) behaviour

ElementsLibrary: Shapes Elements Windows Functors Buttons


Shapes: Elems/Shapes.cpp
	g++ -c Elems/Shapes.cpp -o bin/Shapes.o

Elements: Elems/Elements.cpp
	g++ -c Elems/Elements.cpp -o bin/Elements.o

Windows: Elems/Windows.cpp
	g++ -c Elems/Windows.cpp -o bin/Windows.o

Functors: Elems/Functors.cpp
	g++ -c Elems/Functors.cpp -o bin/Functors.o

Buttons: Elems/Buttons.cpp
	g++ -c Elems/Buttons.cpp -o bin/Buttons.o


Elements = bin/Shapes.o bin/Elements.o bin/Windows.o bin/Functors.o bin/Buttons.o




# There are files, that handles all logic

Managers: GUI Editor ElementCreator ElemTree EventManager


GUI: GUI.cpp
	g++ -c GUI.cpp -o bin/GUI.o

Editor: Managers/Editor.cpp
	g++ -c Managers/Editor.cpp -o bin/Editor.o

ElementCreator: Managers/ElementCreator.cpp
	g++ -c Managers/ElementCreator.cpp -o bin/ElementCreator.o

ElemTree: Managers/ElemTree/ElemTree.cpp
	g++ -c Managers/ElemTree/ElemTree.cpp -o bin/ElemTree.o

EventManager: Managers/EventManager.cpp
	g++ -c Managers/EventManager.cpp -o bin/EventManager.o


Managers = bin/GUI.o bin/Editor.o bin/ElementCreator.o bin/ElemTree.o bin/EventManager.o