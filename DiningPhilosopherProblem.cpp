/********************************************************************************************************************************************
 * File: Pgrm2.cpp
 * Author: Sean Njenga
 * UTDid: sin170000
 * Section: CE 4348.501
 * Modified on: 20 October 2021
 *  by: Sean Njenga
 * 
 * Procedures:
 * main             - Inits/destroys semaphores, creates/cancels threads, and prints out statistics after 5 min. 
 * Philosopher      - Continually read/write semaphores, and calls eat or think when appropriate. 
 * EatOrThink       - Sleeps for a random time in range [25,49] ms, and returns the entire time spent in the method. 
 * ******************************************************************************************************************************************/

#include <semaphore.h>
#include <chrono>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <thread>

#define MIN_SLEEP_DURATION 25                                   //Min time spent for the operations of eating and sleeping in miliseconds
#define MAX_SLEEP_DURATION 49                                   //Max time spent for the operations of eating and sleeping in miliseconds

sem_t utensil[5];                                               //The 5 sempahores used to represent the available forks on the table
sem_t room;                                                     //Semaphore used to limit the number of philosophers in the room at once
double timeSpentEating[] = {0,0,0,0,0,0,0,0,0,0};               //Stores the time spent and the number of times the philosopher ate
double timeSpentThinking[] = {0,0,0,0,0,0,0,0,0,0};             //Stores the time spent and the number of times the philosopher thought

/********************************************************************************************************************************************
 * double EatOrThink()
 * Author: Sean Njenga
 * Modified on: 20 October 2021
 *  by: Sean Njenga
 * Description: Generates a random long number between in range [25,49], and then sleeps for that number of milliseconds.
 *              Records and returns the entire time spent in the method.
 * 
 * Parameters: 
 *      EatOrThink    O/P     double      The amount of time spent in this method.
 ********************************************************************************************************************************************/
double EatOrThink() {
    auto start = std::chrono::high_resolution_clock::now();                                             //Gets an accurate start time for EatOrThink()
    long timeRan = MIN_SLEEP_DURATION + (rand() % (MAX_SLEEP_DURATION - MIN_SLEEP_DURATION) + 1);       //Generates a random number in range [25,49]
    std::this_thread::sleep_for(std::chrono::milliseconds(timeRan));                                    //Sleeps for a random number of miliseconds in range [25,49]
    auto stop = std::chrono::high_resolution_clock::now();                                              //Gets an accurate stop time for EatOrThink()

    return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();                 //Calculate the time spent eating or thinking
}

/********************************************************************************************************************************************
 * void* Philosopher(void* idNum) 
 * Author: William Stallings
 * Modified on: 20 October 2021
 *  by: Sean Njenga
 * Description: Loops indefinitely, when a philosopher is in the room he eats, and when he is outside the room he thinks. 
 *              It increments semaphores when it moves into the room and picks up forks. 
 *              Decrements semaphores when it puts down the forks and leaves the room.  
 * 
 * Parameters: 
 *      idNum           I/P     void*       Pointer to the ID of the philosopher. 
 *      Philosopher     O/P     void*       Pointer to thread location (not currently used).
 ********************************************************************************************************************************************/
[[noreturn]] void* Philosopher(void* idNum) {
    int count = *((int *)idNum);                                    //Sets count to the value being pointed at by int* idNum.
    while(true) {                                                   //Loop forever (until the thread is cancelled)
        timeSpentThinking[count] += EatOrThink();                   //Think, and store the time spent doing so. 
        timeSpentThinking[count + 5] += 1;                          //Increments the # of times the phil. has thought
                                        
        sem_wait(&room);                                            //Enter the room if there is space
        sem_wait(&utensil[count]);                                  //Pick up the left fork
        sem_wait(&utensil[(count + 1) % 5]);                        //Pick up the right fork
                                      
        timeSpentEating[count] += EatOrThink();                     //Eat, and store the time spent doing so.
        timeSpentEating[count + 5] += 1;                            //Increments the # of times the philosopher have eatin
                                           
        sem_post(&utensil[(count + 1) % 5]);                        //Put down the right fork
        sem_post(&utensil[count]);                                  //Put down the left fork
        sem_post(&room);                                            //Leave the room
    }
}

/********************************************************************************************************************************************
 * int main(int argc, char *argv[]) 
 * Author: Sean Njenga
 * Modified on: 20 October 2021
 *  by: Sean Njenga
 * Description: Creates semaphores and philosopher threads, after 5 minutes it cancels the threads and destroys the semaphores.
 *              Then prints statistics to the console, and records the total running time of the program. 
 * 
 * Parameters: 
 *      argc            I/P     int         The number of arguments on the command line. 
 *      argv            I/P     char *[]    The arguments on the command line.
 *      main            O/P     int         Status code (not currently used).
 ********************************************************************************************************************************************/
int main(int argc, char *argv[]) {
    srand(time(NULL));                                                             
    auto start = std::chrono::high_resolution_clock::now();                 //Stores the time the program begins running

    for(int x = 0; x < 5; x++) {
        sem_init(&utensil[x], 0, 1);                                        //Create 5 utensil semaphores with a value of 1
    }
    sem_init(&room, 0, 4);                                                  //Create the room semaphore with a value of 4

    pthread_t treadId[5];                                                   //Create 5 thread IDs for the philosophers
    
    for(int x = 0; x < 5; x++) {
        pthread_create(&treadId[x], NULL, Philosopher, &x);                 //Create a thread for philosopher #i from 0 to 5
        std::this_thread::sleep_for(std::chrono::microseconds(1));          //Provides a 1us delay before creating the next thread
    }
    std::this_thread::sleep_for(std::chrono::minutes(5));

    for(int i=0; i<5; i++) {
        pthread_cancel(treadId[i]);                                         //Terminate philosopher threads
    }

    for(int x = 0; x < 5; x++) {
        printf("Philosopher #%d thought %.0f number of times, for a total of %.2f seconds, or %.0f ms.\n",
        x + 1, timeSpentThinking[x + 5], timeSpentThinking[x] / 1000, timeSpentThinking[x]);        //Prints out the number of times the philosopher thought, and for how long
        printf("Philosopher #%d ate %.0f number of times, for a total of %.2f seconds, or %.0f ms.\n\n",
        x + 1, timeSpentEating[x + 5], timeSpentEating[x] / 1000, timeSpentEating[x]);              //Prints out the number of times the philosopher ate, and for how long
    }

    for(int x = 0; x < 5; x++) {
        sem_destroy(&utensil[x]);                                                        //Release the resources used for the utensil semaphores
    }
    sem_destroy(&room);                                                                  //Release the resources used for the room semaphore

    auto stop = std::chrono::high_resolution_clock::now();                               //Store the time the program stops running
    printf("Total Running time: %ld minutes \n",
           std::chrono::duration_cast<std::chrono::minutes>(stop - start).count());      //Print the total time the program was running

    return 0;
}