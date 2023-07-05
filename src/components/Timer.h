#pragma once
#include "../global.h"
#include "../ObjectComponent2D.h"
#include "../utils/VertexBufferWraper.h"

_R2D_NAMESPACE_START_

namespace components {

	class Timer : public ObjectComponent2D
	{
	public:
		Timer(real_t time);

		inline real_t getTimeLeft() const { return m_timeLeft; }
		// if the timer is not running; this will not work
		void setTimeLeft(real_t time);

		inline real_t getWaitTime() const { return m_waitTime; }
		void setWaitTime(real_t time);

		inline bool getOneShot() const { return m_oneShot; }
		void setOneShot(bool oneshot);

		inline bool paused() const { return !m_paused; }
		void start();
		void stop();

		inline Action_t getTimeoutAction() const { return m_timeoutAction; }
		void setTimeoutAction(Action_t action);

	protected:
		void update(real_t delta) override;

	private:
		Action_t m_timeoutAction;
		real_t m_timeLeft;
		real_t m_waitTime;
		bool m_oneShot;
		bool m_paused;
	};

}

_R2D_NAMESPACE_END_