#include <stdlib.h>
#include "TurnstileActions.h"
#include "TwoCoinTurnstile.h"

enum Event {Coin,Pass,Reset};
enum State {Alarming,FirstCoin,Locked,Unlocked};

struct TwoCoinTurnstile {
	enum State state;
	struct TurnstileActions *actions;
};

struct TwoCoinTurnstile *make_TwoCoinTurnstile(struct TurnstileActions* actions) {
	struct TwoCoinTurnstile *fsm = malloc(sizeof(struct TwoCoinTurnstile));
	fsm->actions = actions;
	fsm->state = Locked;
	return fsm;
}

static void setState(struct TwoCoinTurnstile *fsm, enum State state) {
	fsm->state = state;
}

static void thankyou(struct TwoCoinTurnstile *fsm) {
	fsm->actions->thankyou();
}

static void unlock(struct TwoCoinTurnstile *fsm) {
	fsm->actions->unlock();
}

static void alarmOn(struct TwoCoinTurnstile *fsm) {
	fsm->actions->alarmOn();
}

static void lock(struct TwoCoinTurnstile *fsm) {
	fsm->actions->lock();
}

static void alarmOff(struct TwoCoinTurnstile *fsm) {
	fsm->actions->alarmOff();
}

static void processEvent(enum State state, enum Event event, struct TwoCoinTurnstile *fsm, char *event_name) {
switch (state) {
case Alarming:
switch (event) {
case Reset:
setState(fsm, Locked);
alarmOff(fsm);
lock(fsm);
break;

default:
(fsm->actions->unexpected_transition)("Alarming", event_name);
break;
}
break;

case FirstCoin:
switch (event) {
case Pass:
setState(fsm, Alarming);
alarmOn(fsm);
break;

case Coin:
setState(fsm, Unlocked);
unlock(fsm);
break;

case Reset:
setState(fsm, Locked);
lock(fsm);
break;

default:
(fsm->actions->unexpected_transition)("FirstCoin", event_name);
break;
}
break;

case Locked:
switch (event) {
case Pass:
setState(fsm, Alarming);
alarmOn(fsm);
break;

case Coin:
setState(fsm, FirstCoin);
break;

case Reset:
setState(fsm, Locked);
lock(fsm);
break;

default:
(fsm->actions->unexpected_transition)("Locked", event_name);
break;
}
break;

case Unlocked:
switch (event) {
case Pass:
setState(fsm, Locked);
lock(fsm);
break;

case Coin:
setState(fsm, Unlocked);
thankyou(fsm);
break;

case Reset:
setState(fsm, Locked);
lock(fsm);
break;

default:
(fsm->actions->unexpected_transition)("Unlocked", event_name);
break;
}
break;

}
}

void TwoCoinTurnstile_Coin(struct TwoCoinTurnstile* fsm) {
	processEvent(fsm->state, Coin, fsm, "Coin");
}
void TwoCoinTurnstile_Pass(struct TwoCoinTurnstile* fsm) {
	processEvent(fsm->state, Pass, fsm, "Pass");
}
void TwoCoinTurnstile_Reset(struct TwoCoinTurnstile* fsm) {
	processEvent(fsm->state, Reset, fsm, "Reset");
}
