#ifndef __LTIMER_HH
#define __LTIMER_HH

class LTimer
{
	public:
		LTimer();
		
		void start();
		void stop();
		void pause();
		void unpause();
		
		unsigned int getTicks();
		
		bool isStarted();
		bool isPaused();
	
	private:
		unsigned int mStartTicks;
		unsigned int mPausedTicks;
		
		bool mStarted;
		bool mPaused;
};

#endif