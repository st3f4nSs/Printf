build:
	gcc -g -Wall -o myPrintf *.c -lm

clean:
	rm myPrintf

run:
	./myPrintf
