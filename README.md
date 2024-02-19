# Matrix Multiplication Program in Unix based enviroment
A C program, created as a university project for the Operating Systems class, solves matrix multiplication using Inter-Process Communication (IPC) in a standard Unix-based system (on standalone Ubuntu system).
This program invites the user to designate the dimensions for two matrices and then generates and populates them with random integers between 1 and 20. It carries out matrix multiplication using child processes
by creating each child process in charge of calculating a particular element in the output matrix.

The program includes dynamic memory allocation for the matrices that will work with any given dimension within specific limits. It includes the error-detecting methods to deal with the user-supplied dimensions, checking whether they are within the acceptable range, and are non-zero positive integers. Furthermore, it checks whether the dimensions are good for multiplying a matrix (it must be that the number of columns in the first matrix equals the number of rows in the second matrix).

Communication between processes is done by pipes such that each child process writes its result to a particular pipe, and the parent process reads from these pipes when drawing the final matrix. The parent process only prints the resulting matrices and the product matrix when all the child processes have executed.

## Licence 
This project is licensed under the MIT License.
