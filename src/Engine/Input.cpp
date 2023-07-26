#include "Input.h"

#include "glfw3.h"

Input::Input(GLFWwindow* window):window{window}
{
	keyStatesCurrent = new bool[GLFW_KEY_LAST];	//Create 2 arrays with one bool for each input
	keyStatesPrev = new bool[GLFW_KEY_LAST];

	mouseStatesCurrent = new bool[GLFW_MOUSE_BUTTON_LAST];	//Same as keys (this is a common pattern)
	mouseStatesPrev = new bool[GLFW_MOUSE_BUTTON_LAST];

	for (int i = 0; i < GLFW_KEY_LAST; i++)	//Set all key states and mouse states to false
	{
		keyStatesCurrent[i] = false;
		keyStatesPrev[i] = false;
	}

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST;i++)
	{
		mouseStatesCurrent[i] = false;
		mouseStatesPrev[i] = false;
	}

	mousePosition = {};	//Initialise mouse position as {0,0}
}

Input::~Input()
{
	delete[] keyStatesCurrent;	//Clean up memory
	delete[] keyStatesPrev;

	delete[] mouseStatesCurrent;
	delete[] mouseStatesPrev;
}

void Input::update()
{
	glfwPollEvents();	//Poll events this frame, so inputs from this frame can be collected

	
	//Inputs stored in keyStatesCurrent are last frames inputs, so arrays are swapped, array in keyStatesCurrent after swap can then be overwritten
	bool* tempPtr = keyStatesPrev;
	keyStatesPrev = keyStatesCurrent;
	keyStatesCurrent = tempPtr;

	for (int i = 0; i < GLFW_KEY_LAST;i++)	//Iterate through each key and overwrite keyStatesCurrent
	{
		keyStatesCurrent[i] = glfwGetKey(window, i);	//GLFW_PRESS=1 (true) GLFW_RELEAE=0(false) so no need to do == check
	}

	//This code seems familiar
	tempPtr = mouseStatesPrev;
	mouseStatesPrev = mouseStatesCurrent;
	mouseStatesCurrent = tempPtr;

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		mouseStatesCurrent[i] = glfwGetMouseButton(window, i);
	}

	//Mouse position is gotten via pointers, so create temporary doubles to get values back from
	double x;
	double y;

	glfwGetCursorPos(window, &x, &y);	//Get mouse position
	mousePosition = { x,y };
}

bool Input::getKeyDown(int key)//Gets if the key is down
{
	return keyStatesCurrent[key];	//Get the relevent key
}

bool Input::getKeyPressed(int key)//Gets if the key is down AND was not down last update
{
	return keyStatesCurrent[key] && !keyStatesPrev[key];	
}

bool Input::getKeyReleased(int key)
{
	return !keyStatesCurrent[key] && keyStatesPrev[key];	//Gets if the key is not currently pressed, but was pressed last update
}

bool Input::getMouseDown(int mb)	//For mouseButton logic, look 15 lines above this one
{
	return mouseStatesCurrent[mb];	
}

bool Input::getMousePressed(int mb)
{
	return mouseStatesCurrent[mb] && !mouseStatesPrev[mb];
}

bool Input::getMouseReleased(int mb)
{
	return !mouseStatesCurrent[mb] && mouseStatesPrev[mb];
}

glm::vec2 Input::getMousePositionNormalised()	//Get the mouse position on the screen scaled between 0 and 1
{
	int x, y;
	glfwGetWindowSize(window, &x, &y);
	return mousePosition / glm::vec2{x,y};
}