//
// Created by Aidan Campbell on 3/23/15.
//

#ifndef _ASN1_MAIN_H_
#define _ASN1_MAIN_H_

#define DEPOSIT 0
#define WITHDRAWAL 1

int main();
int randomType();
int randomAmount();

#endif //_ASN1_MAIN_H_

/*
var semaphore mutex := 1;
semaphore wlist := 0; // Customers wait FIFO on this nonbinary semaphore to withdraw.
int wcount := 0; // The number of waiting withdrawal customers on wlist
int balance := 500;
linked-list LIST := NULL; // The list containing the withdrawal requests of waiting customers. // Linked-list manipulation procedures:
void AddEndOfList (linked-list A, int Val); // Adds an element with value Val to the end of A.
void DeleteFirstRequest (linked-list A); // Deletes the first element in list A.
int FirstRequestAmount (linked-list A); //Returns the value in the first element in list A.


Depositing Customer:// Assume that the local variable deposit (int) contains the amount to be deposited.
wait (mutex);
balance := balance + deposit;
if (wcount = 0) {
    signal (mutex)
} else if (FirstRequestAmount (LIST) > balance){
    signal (mutex)
} else {
    signal (wlist); // Deposit has taken place.
}


Withdrawing Customer:// Assume that the local variable withdraw (int) contains the amount to be withdrawn.
wait (mutex);
if (wcount = 0 and balance > withdraw){
    begin balance := balance – withdraw;
    signal (mutex);
} else { // Either other withdrawal requests are waiting or not enough balance.
    AddEndOf List (LIST, withdraw);
    wcount := wcount + 1;
    signal (mutex);
    wait (wlist); // Start waiting for a deposit
    balance := balance – FirstRequestAmount (LIST); // Withdraw.
    DeleteFirstRequest (LIST); // Remove own request from the waiting list.
    wcount := wcount – 1;
    if (FirstRequestAmount (LIST) ≤ balance) {
        signal (wlist)
    } else {
        signal (mutex);
    }
} // Withdrawal is completed.
*/