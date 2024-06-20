all : clean turnstile_test
	./turnstile_test

clean : 
	rm -f turnstile_test *.o twoCoinTurnstile.h twoCoinTurnstile.c

twoCoinTurnstile.o : twoCoinTurnstile.h twoCoinTurnstile.c turnstileActions.h

turnstile_test.o : turnstile_test.c turnstileActions.h twoCoinTurnstile.h

turnstile_test : turnstile_test.o twoCoinTurnstile.o

twoCoinTurnstile.h twoCoinTurnstile.c : twoCoinTurnstile.sm
	cp twocointurnstile.c twoCoinTurnstile.c
	cp twocointurnstile.h twoCoinTurnstile.h
	cp turnstileActions.h TurnstileActions.h
	java -jar ../../build/jar/smc.jar -l C twoCoinTurnstile.sm

