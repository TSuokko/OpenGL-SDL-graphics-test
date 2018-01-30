#pragma once


namespace DevyEngine
{
	class FpsLimiter
	{
	public:
		FpsLimiter();
		void init(float MaxFps);
		void setMaxFps(float MaxFps);

		void begin();
		float end();
	private:
		void calculateFps();

		float _maxFPS;
		float _fps;
		float _frameTime;

		unsigned int _startTicks;
	};
}