#pragma once
#include <glm.hpp>

struct GLFWwindow;

class Input
{
public:
	Input(GLFWwindow* window);
	~Input();

	void update();

	bool getKeyDown(int key);		//Get if a key is held down
	bool getKeyPressed(int key);	//Get if a key was pressed this frame
	bool getKeyReleased(int key);	//Get if a key was released this frame

	bool getMouseDown(int mb);		//Get if a mouse button is held down
	bool getMousePressed(int mb);	//Get if a mouse button was pressed this frame
	bool getMouseReleased(int mb);	//Get if a mouse button was released this frame

	glm::vec2 getMousePosition() { return mousePosition; }	//Get the position of the cursor in pixels
	glm::vec2 getMousePositionNormalised();

protected:
	GLFWwindow* window;

	glm::vec2 mousePosition;	//The mouse position

	//Keeps the state of the keys in this frame and last frame, allows program to tell if key was just pressed this frame
	bool* keyStatesCurrent;
	bool* keyStatesPrev;

	//Same as key states but for the mouse
	bool* mouseStatesCurrent;
	bool* mouseStatesPrev;
};