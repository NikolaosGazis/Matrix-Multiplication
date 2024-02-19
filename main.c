/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <wait.h>
#include <unistd.h>

/* Definments */
#define MAX_ROWS 15
#define MAX_COLUMNS 15

/* Functions */
// ALlocate memory for Matrices //
int **memory_allocation(int rows, int cols) {
    int **arr = (int **)malloc(rows*sizeof(int)); // memory for every row.
    for(int i=0; i<rows; i++) {
        arr[i] = (int *)malloc(cols*sizeof(int)); // memory for every column.
    }
    return arr;
}

// Fill the arrays with random positive integers //
int fillArray(int **arr, int rows, int cols) {
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            arr[i][j] = rand()%20 + 1; // ensure its at least 1.
        }
    }
}

/* main */
int main(int argc, char *argv[]) {
    // srand for the seed to differ for each rand() call and have different values //
    srand(time(NULL));
    
    /* - Program Functionality Section - */
    // Check total arguments from console //
    if(argc != 5) { // "./<name>" counts as an argument as well, we ignore it.
    	printf("[SYSTEM] The total arguments for the dimenstions were not 4 therefore the program can not be executed...EXIT.\n");
    	return EXIT_FAILURE;
    }

    /* Pass the arguments to variables - using atoi to convert string numerical values to integers - check for buffer overflow */
    int rows1 = atoi(argv[1]);
    if(rows1==0) {
        printf("[SYSTEM] Looks like the argument could not be converted properly... EXIT.\n");
        return EXIT_FAILURE;
    }
    int cols1 = atoi(argv[2]);
    if(cols1==0) {  
        printf("[SYSTEM] Looks like the argument could not be converted properly... EXIT.\n");
        return EXIT_FAILURE;
    }
    int rows2 = atoi(argv[3]);
    if(rows2==0) {
        printf("[SYSTEM] Looks like the argument could not be converted properly... EXIT.\n");
        return EXIT_FAILURE;
    }
    int cols2 = atoi(argv[4]);
    if(cols2==0) {
        printf("[SYSTEM] Looks like the argument could not be converted properly... EXIT.\n");
        return EXIT_FAILURE;
    }
    
    // Check if any of the arguments are above the defined max rows/columns //
    if(rows1 > MAX_ROWS || cols1 > MAX_COLUMNS || rows2 > MAX_ROWS || cols2 > MAX_COLUMNS) {
        printf("[SYSTEM] The set dimenstions seem to be above the limit...EXIT.\n");
        return EXIT_FAILURE;
    }

    // Check if any of the arguments are equal or lower than zero //
    if(rows1 <= 0 || cols1 <= 0 || rows2 <= 0 || cols2 <=0) {
        printf("[SYSTEM] The arguments cannot be 0 or negative...EXIT.\n");
        return EXIT_FAILURE;
    }

    // Check if 3rd (columns of first matrix) and 4th (rows of second matrix) arguments are equal so the multiplication can proceed //
    if(cols1 != rows2) {
    	printf("[SYSTEM] The 2nd and 3rd argument were not equal, they must so multiplication between arrays can proceed...EXIT.\n");
        return EXIT_FAILURE;
    }

    // Create and fill the matrices //
    int **matrix1 = memory_allocation(rows1, cols1); // 1.
    fillArray(matrix1, rows1, cols1);
    int **matrix2 = memory_allocation(rows2, cols2); // 2.
    fillArray(matrix2, rows2, cols2);
    int **finalMatrix = memory_allocation(rows1, cols2); // new matrix.

    /* --- IPC Section --- */
    // File Descriptors for child-parent communication //
    int ***pipes = (int ***)malloc(rows1*sizeof(int**));
    for(int i=0; i<rows1; i++) {
        pipes[i] = (int **)malloc(cols2*sizeof(int*));
        for(int j=0; j<cols2; j++) {
            pipes[i][j] = (int *)malloc(2 * sizeof(int));
            if(pipe(pipes[i][j]) == -1) {
                perror("pipe:");
                return 1;
            }
        }
    }

    // Child Processes - Fork //
    pid_t pid;
    for(int i=0; i<rows1; i++) {
        for(int j=0; j<cols2; j++) {
            pid = fork();
            if(pid == -1){ // fail.
                perror("fork:");
                return EXIT_FAILURE;
            }
            else if (pid == 0) { // new child.
                close(pipes[i][j][0]); // close unsed end of pipe.

                // multiplication //
                int element = 0;
                for(int k=0; k<cols1; k++) {
                    element += matrix1[i][k]*matrix2[k][j];
                }

                // write to pipe //
                write(pipes[i][j][1], &element, sizeof(int));

                close(pipes[i][j][1]); // close write pipe.
                exit(0);
            }
        }
    }

    // Parent Process //
    for(int i=0; i<rows1; i++) {
        for(int j=0; j<cols2; j++) {
            wait(NULL); // sync childs.

            // read result //
            read(pipes[i][j][0], &finalMatrix[i][j], sizeof(int));

            close(pipes[i][j][0]);
        }
    }
    
    // Print each matrix //
    printf("Matrix 1:\n"); // 1.
    for(int i=0; i<rows1; i++) {
        for(int j=0; j<cols1; j++) {
            printf("%d ", matrix1[i][j]);
        }
        printf("\n"); // new line.
    }

    printf("\nMatrix 2:\n"); // 2.
    for(int i=0; i<rows2; i++) {
        for(int j=0; j<cols2; j++) {
            printf("%d ", matrix2[i][j]);
        }
        printf("\n"); // new line.
    }

    printf("\nNew Matrix:\n");
    for(int i=0; i<rows1; i++) {
    	for(int j=0; j<cols2; j++) {
    	    printf("%d ", finalMatrix[i][j]);
    	}
    	printf("\n"); // new line.
    }
    
    // Free memory for each matrix to avoid memory leaks //
    for(int i=0; i<rows1; i++) {
        free(matrix1[i]);
    }
    free(matrix1);

    for(int i=0; i<rows2; i++) {
        free(matrix2[i]);
    }
    free(matrix2);
    
    for(int i=0; i<rows1; i++) {
    	free(finalMatrix[i]);
    }
    free(finalMatrix);

    return EXIT_SUCCESS;
}