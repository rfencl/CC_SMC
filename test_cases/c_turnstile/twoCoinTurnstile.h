#ifndef TWOCOINTURNSTILE_H
#define TWOCOINTURNSTILE_H

struct TurnstileActions;
struct TwoCoinTurnstile;
struct TwoCoinTurnstile *make_TwoCoinTurnstile(struct TurnstileActions*);
void TwoCoinTurnstile_Coin(struct TwoCoinTurnstile*);
void TwoCoinTurnstile_Pass(struct TwoCoinTurnstile*);
void TwoCoinTurnstile_Reset(struct TwoCoinTurnstile*);
#endif
