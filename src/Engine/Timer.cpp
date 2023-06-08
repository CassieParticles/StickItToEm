#include "Timer.h"
#include <glad/glad.h>
#include <glfw3.h>

Timer::Timer(double UPS):updateDelta{1./UPS}
{
	lastTime = glfwGetTime();	//Set the last time it was updated to the current time, so deltaTime accurately starts at 0
}

Timer::~Timer() {}

void Timer::Update()
{
	double currentTime = glfwGetTime();	//Get the current time

	update = currentTime - lastTime > updateDelta;	//Get if the timer should return an update
	if (update)
	{
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		timeRunning += deltaTime;
	}
}

