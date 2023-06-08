#pragma once
#include <glm.hpp>

struct GLFWwindow;
class Timer;
class Input;

class BaseLevel
{
public:
	BaseLevel(GLFWwindow* window,Input* input, glm::vec4 bgColour);	//Default constructor, ensures level has access to window, input, etc
	~BaseLevel();

	virtual void openLevel();	//Functions to call when first opening and closing a level, used when multiple levels exist
	virtual void closeLevel();

	virtual void handleInput(Timer* updateTimer)=0;	//Functions called every update/frame, needed for the game
	virtual void update(Timer* updateTimer)=0;
	virtual void render(Timer* frameTimer)=0;
protected:
	GLFWwindow* window;	//Base data needed for every level
	Input* input;

	glm::vec4 bgColour;

	//Clear a screen with bg colour to start rendering a frame, and swap the front and back buffers to display a rendered frame
	void beginDraw();	
	void endDraw();
};