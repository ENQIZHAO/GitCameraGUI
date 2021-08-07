#include "Timer.h"
Timer::Timer(int _type,int _limit)
{
	limit = _limit;
	type = _type;
}

void Timer::run()
{
	clock_t ca,cb;
	ca = clock();
	while (true)
	{
		cb = clock();
		if((cb - ca) / CLOCKS_PER_SEC>=limit)
		{
			break;
		}
		
	}
	emit(timerOut(type));
}
