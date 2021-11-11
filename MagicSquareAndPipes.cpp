/*
 * File: Program1.cpp
 * Author: Sean Njenga
 * Modified: 15 September 2022
 * Procedures:
 * main                     - get user's input sequence and communicate it from client to server through a pipe,
 *                             then validate the character sequence as a magic square, finally print user system info
 * ValidMagicSquare         - user input sequence validation
 * GenerateMagicSquare      - character array to 2d int array conversion & magic square sequence test
 * DisplaySystemInfo        - display various info for user's machine
 */

#include <iostream>
#include <unistd.h>
#include <sys/utsname.h>
#include <math.h>
#include <cstring>
#include <sys/wait.h>

#define SQUARE_SIZE 9 // The max number of characters that can be used for a sequence

/*
 * void DisplaySystemInfo()
 * Author: Sean Njenga
 * Date: 15 September 2021
 * Description: Uses getdomainname, gethostname, and uname functions to display the user's machine system information.
 *
 * Parameters:
 *      DisplaySystemInfo      V/P     void    Status code (not used)
 */
void DisplaySystemInfo(){
    printf("System Info: \n");

    //Variables for storing the system info
    char domainname[256];
    char hostname[256];
    utsname userdata;

    if (getdomainname(domainname, 256) == 0){ // Gets the systems domain name
        printf("Domain Name: %s\n", domainname);
    }
    else {
        perror("getdomainname() error");
    }

    if (gethostname(hostname, 256) == 0){ // Gets the systems host name
        printf("Host Name: %s\n", hostname);
    }
    else {
        perror("gethostname() error");
    }

    if (uname(&userdata) == 0){ // Gets various system data
        printf("System Name: %s\n", userdata.sysname);
        printf("Node Name: %s\n", userdata.nodename);
        printf("Release: %s\n", userdata.release);
        printf("Version: %s\n", userdata.version);
        printf("Machine: %s\n", userdata.machine);
        printf("Domain Name: %s\n\n", userdata.domainname);
    }
    else {
        perror("uname() error");
    }
}

/*
 * bool GenerateMagicSquare( char *val )
 * Author: Sean Njenga
 * Date: 15 September 2021
 * Description: Generates a 2d array of integers using the inputted character sequence (string),
 *              then verifies whether the sequence represents a magic square,
 *              and returns the appropriate True/False validation.
 *
 * Parameters:
 *      val                       I/P     char*   The user's valid inputted character sequence
 *      GenerateMagicSquare       O/P     bool    The resulting output used to determine if the sequence is a valid magic square,
 *                                                  in the int main() method.
 */
char GenerateMagicSquare(char *sequence){
    /* Convert the given nine-digit square string to 2d array of integers */
    int square[3][3];
    char answer;
    int size = sqrt(SQUARE_SIZE); //Dimensions of square is size x size
    for (int x = 0; x < size; x++){
        for (int y = 0; y < size; y++){
            square[x][y] = (int) (sequence[y + (x * 3)] - '0'); //Convert character to number
        }
    }
    int magicNum = square[0][0] + square[0][1] + square[0][2]; //Tracks the magic num for the square to check with diagonals later and for final print if valid

    int topLeftToBottomRightDiag = 0, topRightToBottomLeftDiag = 0, rows = 0, cols = 0;

    /* Validates whether the sequence is a magic square */
    for (int x = 0; x < size; x++){
        topLeftToBottomRightDiag += square[x][x]; //Top-Left to Bottom-Right Diagonal sum
        topRightToBottomLeftDiag += square[x][(size - 1) - x]; //Top-Right to Bottom-Left Diagonal sum
        for (int y = 0; y < size; y++){
            rows += square[x][y]; //Sums each row
            cols += square[y][x]; //Sums each column
        }
        if (rows != cols){ //If a row sum or col sum ever != then exit
            answer = 'F';
            goto EXIT;
        }
        rows = 0; cols = 0; //Reset row and column values for next row
    }

    if ((topLeftToBottomRightDiag == topRightToBottomLeftDiag) && (topRightToBottomLeftDiag == magicNum)){ //Checks to sef the diagonal, row, and col sums match
        answer = 'T';
    }
    else{
        answer = 'F';
    }

    EXIT: return answer;
}

/*
 * bool ValidMagicSquare( char *sequence )
 * Author: Sean Njenga
 * Date: 15 September 2021
 * Description: Verifies whether the user's inputted sequence is 9 characters long and only contains arbaic numerals
 *
 * Parameters:
 *      sequence                  I/P     char*   The user's inputted character sequence
 *      GenerateMagicSquare       O/P     bool    The resulting output used to determine if the sequence is a valid magic square,
 *                                                  in the int main() method.
 */
bool ValidMagicSquare(char *sequence){
    bool validSeq = true; // Assume input sequence is valid
    int inputSize = strlen(sequence);
    for (int x = 0; x < inputSize - 1; x++){ // Checks to see if the sequence contains only numbers
        if (!isdigit(sequence[x])){
            printf("Input Magic Square Contains Non-Numerical Characters, TRY AGAIN\n");
            validSeq = false;
            goto NEXT; // Jumps to the next validation
        }
    }

    NEXT: if (inputSize < SQUARE_SIZE){ // Checks to see if the sequence has less than 9 characters
        printf("Input Magic Square Sequence is too Small, TRY AGAIN\n");
        validSeq = false;
    }
    else if (inputSize > SQUARE_SIZE) { // Checks to see if the sequence has greater than 9 characters
        printf("Input Magic Square Sequence is too Big, TRY AGAIN\n");
        validSeq = false;
    }
    return validSeq;
}

/*
* int main( int argc, char **argv )
* Author: Sean Njenga
* Date: 15 September 2021
* Description: Gets the input sequence from the user and validates it, then sends a valid sequence
*               across a client & server pipe. Where it is checked if it's a magic square and the
*               result is printed with various machine system information
*
* Parameters:
*       argc I/P int The number of arguments on the command line
*       argv I/P char *[] The arguments on the command line
*       main O/P int Status code (not currently used)
*/
int main(int argc, char **argv) {
    /*
     * Gets the input sequence to be tested from the user and validates before checking for magic square
     * True Test Values "276951438" & "492357816";
     */
    char *inputSquare;
    do{
        printf("Enter 9 Digit Sequence: ");
        scanf("%s", inputSquare);
    } while(!ValidMagicSquare(inputSquare));

    // Setup the client and server pipes
    int client[2];
    int server[2];

    if (pipe(client) < 0){ // Enable the client pipe
        fprintf(stderr, "Client Pipe Failed -> %s\n", strerror(errno));
    }

    if (pipe(server) < 0){ // Enable server pipe
        fprintf(stderr, "Server Pipe Failed -> %s\n", strerror(errno));
    }

    pid_t pid = fork(); // Clone the pipe process
    if (pid < 0){ // Check to see if the fork() worked
        fprintf(stderr, "Fork Failed: %s\n", strerror(errno));
        exit(0);
    }

    // Writes the user's sequence to the client
    if (pid == 0){ //Child Process
        close(client[1]); //Write end of client closed
        close(server[0]); //Read end of server closed

        char userInputSquare[SQUARE_SIZE + 1];
        if (read(client[0], &userInputSquare, sizeof(char) * SQUARE_SIZE + 1) < 0){ // Read user's input from the client to be passed to the server
            fprintf(stderr, "Read From Pipe Failed: %s\n", strerror(errno));
        }

        char answer = GenerateMagicSquare(userInputSquare); // Determines if the sequence is a magic square
        if (write(server[1], &answer, sizeof(char) * 2) < 0){ // Write the T/F answer to the server
            fprintf(stderr, "Write to Pipe Failed: %s\n", strerror(errno));
        }
        close(client[0]);
        close(server[1]);
    }
    else{ //Parent Process
        system("clear");
        close(client[0]); //Read end of client closed
        close(server[1]); //Write fo server closed
        char answer[2];

        if (write(client[1], inputSquare, sizeof(char) * SQUARE_SIZE + 1) < 0){ // Write valid user input sequence to the client
            fprintf(stderr, "Write to Client Failed -> %s", strerror(errno));
        }

        if (read(server[0], answer, sizeof(char) * 2) < 0){ // Read magic square T/F result from server
            fprintf(stderr, "Read From Pipe Failed: %s", strerror(errno));
        }

        wait(NULL);
        // Print Result
        printf("Sequence Tested: %s\nMagic Square Result: %s\n\n", inputSquare, answer);

        //Display various sys info
        DisplaySystemInfo();

        close(server[0]);
        close(client[1]);
    }

    return 0;
}
