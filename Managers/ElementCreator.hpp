#ifndef ELEMENT_CREATOR_HPP
#define ELEMENT_CREATOR_HPP



#include "../Elems/Buttons.hpp"

#include <vector>
using std::vector;



class ElementCreator
{

private:

	ElemConfig* main_config;

	map <ElemType, vector <ElemConfig*>> default_config;


	void ParseHierarchy (Application* App, FILE* hierarchy);

	void GetHierarchy   (Application* App, FILE* hierarchy);

	void CreateElem (int num_tabs, EnabledType enabled_type, ElemType type, int detailed_type, Assets* assets);


	Window*  NewWindow  (ElemConfig* config);

	Button*  NewButton  (ElemConfig* config);

	Shape*   NewShape	  (ElemConfig* config);

	Shape*   NewCollision (ElemConfig* config);


public:


	ElementCreator ();

	ElementCreator (Application* App, const char* hierarchy_conf);


	Element* NewElement (ElemConfig* config);


	ElemConfig* GetConfig (ElemType type, int detailed_type);

	ElemConfig* GetMainConfig ();
};



ElemType    GetElemType    (char type);

EnabledType GetEnabledType (char enabled);


Assets*  GetAssets (Application* App, FILE* hierarchy);

Shape*   GetShape (FILE* hierarchy);

Texture* GetTexture (Application* App, FILE* hierarchy);


int DetailedTypeForName (ElemType elem_type, const char* type_name);

WindowType WindowTypeForName (const char* type_name);

ButtonType ButtonTypeForName (const char* type_name);

ShapeType  ShapeTypeForName  (const char* type_name);

Texture* TextureFromPath (Application* App, const char* path);



const int MAX_NESTING = 30;



#endif