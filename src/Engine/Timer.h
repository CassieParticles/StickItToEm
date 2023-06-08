#pragma once

class Timer
{
public:
	Timer(double UPS);
	~Timer();

	void Update();
	bool getUpdate() { return update; }
	double getDeltaTime() { return deltaTime; }
	double getTimeRunning() { return timeRunning; }
private:
	double updateDelta;	//The amount of time in seconds between updates

	double deltaTime{};	//Used to track when the timer is 
	double lastTime{};
	bool update;

	double timeRunning{};
};