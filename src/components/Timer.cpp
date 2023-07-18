#include "global.h"
#include "Timer.h"

_R2D_NAMESPACE_START_
namespace components
{

	Timer::Timer(real_t time)
		: m_waitTime(time)
	{
	}

	void Timer::setTimeLeft(real_t time)
	{
		if (time <= 0.0f)
		{
			_r2d_error("can't set 'time left' to a value less then or equal to 0.0");
			return;
		}
		m_timeLeft = time;
	}

	void Timer::setWaitTime(real_t time)
	{
		if (time <= 0.0f)
		{
			_r2d_error("can't set 'wait time' to a value less then or equal to 0.0");
			return;
		}
		m_waitTime = time;
	}

	void Timer::setOneShot(bool oneshot)
	{
		m_oneShot = oneshot;
	}

	void Timer::start()
	{
		m_paused = false;
		m_timeLeft = m_waitTime;
	}

	void Timer::stop()
	{
		m_paused = true;
	}

	void Timer::setTimeoutAction(Action_t action)
	{
		m_timeoutAction = action;
	}

	void Timer::update(real_t delta)
	{
		if (m_paused)
			return;
		
		m_timeLeft -= delta;
		if (m_timeLeft < 0.0f)
		{
			if (m_oneShot)
			{
				stop();
			}
			else
			{
				m_timeLeft += m_waitTime;
			}
			if (m_timeoutAction)
				m_timeoutAction();
		}
		
	}
}

_R2D_NAMESPACE_END_