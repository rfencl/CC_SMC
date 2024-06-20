#ifndef TWOCOINTURNSTILE_H
#define TWOCOINTURNSTILE_H

#include "TurnstileActions.h"

class TwoCoinTurnstile : public TurnstileActions
{
public:
	TwoCoinTurnstile()
		: state(State_Locked)
	{
	}

	void Coin() { processEvent(Event_Coin, "Coin"); }
	void Pass() { processEvent(Event_Pass, "Pass"); }
	void Reset() { processEvent(Event_Reset, "Reset"); }

private:
	enum State
	{
		State_Alarming,
		State_FirstCoin,
		State_Locked,
		State_Unlocked
	};
	State state;
	void setState(State s) { state = s; }
	enum Event
	{
		Event_Coin,
		Event_Pass,
		Event_Reset
	};
	void processEvent(Event event, const char *eventName)
	{
		switch (state)
		{
		case State_Alarming:
			switch (event)
			{
			case Event_Reset:
				setState(State_Locked);
				alarmOff();
				lock();
				break;

			default:
				unexpected_transition("Alarming", eventName);
				break;
			}
			break;

		case State_FirstCoin:
			switch (event)
			{
			case Event_Pass:
				setState(State_Alarming);
				alarmOn();
				break;

			case Event_Coin:
				setState(State_Unlocked);
				unlock();
				break;

			case Event_Reset:
				setState(State_Locked);
				lock();
				break;

			default:
				unexpected_transition("FirstCoin", eventName);
				break;
			}
			break;

		case State_Locked:
			switch (event)
			{
			case Event_Pass:
				setState(State_Alarming);
				alarmOn();
				break;

			case Event_Coin:
				setState(State_FirstCoin);
				break;

			case Event_Reset:
				setState(State_Locked);
				lock();
				break;

			default:
				unexpected_transition("Locked", eventName);
				break;
			}
			break;

		case State_Unlocked:
			switch (event)
			{
			case Event_Pass:
				setState(State_Locked);
				lock();
				break;

			case Event_Coin:
				setState(State_Unlocked);
				thankyou();
				break;

			case Event_Reset:
				setState(State_Locked);
				lock();
				break;

			default:
				unexpected_transition("Unlocked", eventName);
				break;
			}
			break;
		}
	}
};

#endif
