#pragma once

class GLFWwindow;
class BaseLevel
{
public:
	BaseLevel(GLFWwindow* window);

	virtual void handleInput(float deltaTime);
	virtual void update(float deltaTime);
	virtual void render(float deltaTime);
protected:
	GLFWwindow* window;
};