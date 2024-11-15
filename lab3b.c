#include <stdio.h>

int main() {
    // Start with 1000 AED balance
    int transactions[] = {-200, -100, -500, -400, -500, -200, 300}; // List of transactions
    int num_transactions = sizeof(transactions) / sizeof(transactions[0]); // Number of transactions
    int tobeprocessed[num_transactions]; // Array to store unprocessed transactions
    int tobeprocessed_count = 0; // Count of unprocessed transactions
    int balance = 1000; // Starting balance

    // Going through each transaction
    for (int i = 0; i < num_transactions; i++) {
        // Check if the transaction is a withdrawal and if there is enough balance
        if (transactions[i] < 0 && balance + transactions[i] < 0) { // Withdrawal and insufficient balance 
            printf("Invalid transaction: %d. Insufficient balance.\n", transactions[i]); // Print error message
            tobeprocessed[tobeprocessed_count++] = transactions[i];  // Add the transaction to the unprocessed list
        }
        // If balance reaches 0, stop processing further transactions
        else if (balance == 0) { // Balance is 0
            printf("Balance is 0. No further transactions will be processed.\n"); // Print message
            tobeprocessed[tobeprocessed_count++] = transactions[i]; // Add the transaction to the unprocessed list
            break; // Stop processing further transactions
        }
        else {
            // Valid transaction: update balance
            balance += transactions[i];
        }
    }

    // Printing the final balance and unprocessed transactions
    printf("Final balance: %d AED\n", balance); // Print final balance
    printf("Unprocessed transactions: "); // Print unprocessed transactions
    for (int i = 0; i < tobeprocessed_count; i++) { // Loop through unprocessed transactions
        printf("%d ", tobeprocessed[i]); // Print unprocessed transactions
    }
    printf("\n");

    return 0;
}