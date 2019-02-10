#include <stdlib.h>

int balances[100];
int numberOfAttemptedTransactions = 0;

void deposit(int amount, int accountNumber)
{
    numberOfAttemptedTransactions++;
    if (amount > 0 && accountNumber < 100 && accountNumber >= 0)
        balances[accountNumber] += amount;
}

int balance(int accountNumber)
{
    numberOfAttemptedTransactions++;
    if (accountNumber < 100 && accountNumber >= 0)
        return balances[accountNumber];
    else 
        return -1; //error code
}

