To compile the project:
make

To run the project:
./bank inputFile numOfThreads
Example: ./bank input.txt 15

To clean the executable files:
make clean

This project consists of:

1) bank.c
	- Input is read from the input file into two structures: userAccounts and transactions by the main thread
	- UserAccounts consists of user data including account number and account balance
	- Transactions consists of list of all transactions between different user accounts
	- Number of threads to be spawned to process the transactions are specified by the user
	- Each thread processes the transfers from one user account to another using mutex locks
	- Deadlocks and race conditions have been handled

2) setup.h
    - Contains structures for user account and transactions
    - HashMap implementation of user account structure
    - pthread data setup

3) input.txt, input2.txt
   	- contains sample input

4) Makefile
	- use make compile to compile all files
	- use make clean to clean the generated executable files

4) ReadMe.txt
	- Description of files in the project