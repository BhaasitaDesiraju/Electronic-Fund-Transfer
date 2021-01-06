compile: 
	gcc -o bank.o bank.c -pthread

clean: 
	$(RM) bank.o