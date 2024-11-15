#include<stdio.h> // Include the standard input/output library
#include <stdbool.h> // Include the standard boolean library

#define NROWS 8    // Define the number of rows in the matrix 
#define NCOLS 3     // Define the number of columns in the matrix
#define SIZE 24    // Define the size of the array

// Function prototypes
void print_array(int array[], int length); // Function to print an array
void print_matrix(int mat[][NCOLS], int rows, int cols); // Function to print a matrix
bool isValid(int length, int pos); // Function to check if a position is valid
void remove_element(int arr[], int length, int pos); // Function to remove an element from an array
void insert_element(int arr[], int length, int pos, int value); // Function to insert an element into an array
void reshape(const int arr[], int length, int rows, int cols, int arr2d[rows][cols]); // Function to reshape an array into a 2D matrix
void trans_matrix(int rows, int cols, const int mat[rows][cols], int mat_transp[cols][rows]); // Function to transpose a matrix
bool found_duplicate(int arr[], int length); // Function to check for duplicates in an array

// Main function
int main() {
    int arr[SIZE] = {10, 20, 30, 40, 50, 10, 20, 40, 50, 10, 10, 20, 30, 40, 50, 20, 30, 80, 40, 50, 10, 20, 30, 40}; // Initialize the array
    int arr2d[NROWS][NCOLS]; // Initialize the 2D matrix
    
    // Print the initial array
    printf("Initial Array:\n");
    print_array(arr, SIZE); 
   
    remove_element(arr, SIZE, 2); // Remove the element at index 2 
    insert_element(arr, SIZE, 2, 80); // Insert the value 80 at index 2
    reshape(arr, SIZE, NROWS, NCOLS, arr2d); // Reshape the array into a 2D matrix

   // Transpose the matrix
    int mat_transp[NCOLS][NROWS];  // Initialize the transposed matrix
    trans_matrix(NROWS, NCOLS, arr2d, mat_transp); // Transpose the matrix

   // Check for duplicates in the array
    if (found_duplicate(arr, SIZE)) {// Check for duplicates in the array
        printf("Duplicates found in the array.\n");// Print a message if duplicates are found
    } else {
        printf("No duplicates found in the array.\n");// Print a message if no duplicates are found
    }

    return 0; // Return 0 to indicate successful completion
}

// Function to print an array
void print_array(int array[], int length){ // Function to print an array
    for(int i = 0; i < length; i++) // Loop through the array
        printf("%d  ", array[i]);// Print each element
    printf("\n");// Print a newline
}

// Function to remove an element from an array
void remove_element(int arr[], int length, int pos) { // Function to remove an element from an array
    if (!isValid(length, pos)) { // Check if the position is valid
        printf("Invalid position for removal: %d\n", pos); // Print an error message
        return;
    }

    for (int i = pos; i < length - 1; i++) { // Loop through the array
        arr[i] = arr[i + 1];  // Shift elements to the left
    }

    printf("After removal, array is:\n"); //
    print_array(arr, length - 1); // Print the updated array
}

bool isValid(int length, int pos) { // Function to check if a position is valid
    return pos >= 0 && pos < length; // Return true if the position is valid
}

void insert_element(int arr[], int length, int pos, int value) { // Function to insert an element into an array
    if (!isValid(length, pos)) { // Check if the position is valid
        printf("Invalid position for insertion: %d\n", pos);// Print an error message
        return;
    }

    for (int i = length - 1; i > pos; i--) { // Loop through the array
        arr[i] = arr[i - 1];// Shift elements to the right
    }
    
    arr[pos] = value;  // Insert the value at the specified position

    printf("After insertion, array is:\n"); 
    print_array(arr, length); // Print the updated array
}

// Function to reshape an array into a 2D matrix
void reshape(const int arr[], int length, int rows, int cols, int arr2d[rows][cols]) { // Function to reshape an array into a 2D matrix
    if (length != rows * cols) { // Check if the length of the array matches the size of the matrix
        printf("Error: The length of the array does not match the size of the matrix.\n"); // Print an error message
        return;
    }

    int idx = 0; // Initialize the index
    for (int col = 0; col < cols; col++) { // Loop through the columns
        for (int row = 0; row < rows; row++) { // Loop through the rows
            arr2d[row][col] = arr[idx]; // Assign the value from the array to the matrix
            idx++; // Increment the index
        }
    }

    printf("Reshaped 2D matrix:\n"); 
    print_matrix(arr2d, rows, cols); // Print the reshaped matrix
}

// Function to print a matrix
void print_matrix(int mat[][NCOLS], int rows, int cols){ // Function to print a matrix
    for(int i = 0; i < rows; i++) { // Loop through the rows
        for(int j = 0; j < cols; j++) { // Loop through the columns
            printf("%d  ", mat[i][j]);    // Print the element
        }
        puts(""); // Print a newline
    }     
}

// Function to transpose a matrix
void trans_matrix(int rows, int cols, const int mat[rows][cols], int mat_transp[cols][rows]) { // Function to transpose a matrix
    for (int i = 0; i < rows; i++) { // Loop through the rows
        for (int j = 0; j < cols; j++) { // Loop through the columns
            mat_transp[j][i] = mat[i][j]; // Transpose the matrix
        }
    }

    printf("Transposed matrix:\n");
    print_matrix(mat_transp, cols, rows); // Print the transposed matrix
}

// Function to check for duplicates in an array
bool found_duplicate(int arr[], int length) { // Function to check for duplicates in an array
    for (int i = 0; i < length - 1; i++) { // Loop through the array
        for (int j = i + 1; j < length; j++) { // Loop through the remaining elements
            if (arr[i] == arr[j]) { // Check for duplicates
                return true; // Return true if a duplicate is found
            }
        }
    }
    return false; // Return false if no duplicates are found
}
