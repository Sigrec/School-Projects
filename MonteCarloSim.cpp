/********************************************************************************************************************************************
 * File: MonteCarloSim.cpp
 * Author: Sean Njenga
 * UTDid: sin170000
 * Section: CE 4348.501
 * Modified on: 14 November 2021
 *  by: Sean Njenga
 * 
 * Procedures:
 * main                      -  Runs a Monte Carlo Simulation on 3 page replacement algorithm's, providing the randomly generated data
 * LRUReplacementAlgo        -  Performs the LRU Page Replacement Algo on a dataset, and counts pagefaults after frame allocation
 * FIFOReplacementAlgo       -  Performs the FIFO Page Replacement Algo on a dataset, and counts pagefaults after frame allocation
 * ClockReplacementAlgo      -  Performs the Clock Page Replacement Algo on a dataset, and counts pagefaults after frame allocation
 * ******************************************************************************************************************************************/

#include <iostream>
#include <random>
#include <deque>
#include <algorithm>
#include <cmath>

const int MU = 10;                    // Mean value for the normal distribution
const int SIGMA = 2;                  // Standard deviation value for the normal distribution
const int MIN_WORKING_SET_SIZE = 4;   // The minimum working set size or minimum frame size to be tested
const int MAX_WORKING_SET_SIZE = 20;  // The maximum working set size or maximum frame size to be tested
const int TOTAL_MEMORY_PAGES = 1000;  // The total number of random memory page values to generate
const int NUM_EXPERIMENTS = 1000;     // The total number of experiments to run


/********************************************************************************************************************************************
 * int LRUReplacementAlgo(int &wss, int data[])
 * Author: Sean Njenga
 * Modified on: 14 November 2021
 *  by: Sean Njenga
 * Description: Performs the LRU page replacement algorithm on the provided dataset. Where if a new memory page needs to be allocated in the existing frame, it 
 *              removes the memory page that has not been the most used recently in the frame with the newest memory page at the end of the 
 *              queue. Signaling that the newest page is the most recently used memory page
 * Parameters: 
 *      LRUReplacementAlgo      O/P     int     Returns the total number of page faults caused by the LRU algorithm givin the frame size
 *      wss                     I/P     int     The current working set size or frame size to store pages
 *      data                    I/P     int[]   The randomly generated memory page values to test                
 ********************************************************************************************************************************************/
int LRUReplacementAlgo(int &wss, int data[]){
    int pageFaultCount = 0;
    std::vector<int> process(wss, 0);                           // The frane containing the memory pages
    std::vector<int> timeSpentInFrame(wss, 1);                  // Contains how long each memory page in frame has been in the frame
    int mostTimeSpentInFramePage = 0;                           // The memory page that has been in the frame the longest
    int mostTimeSpentInFrame = 1;                               // The current time the least recently used memory page has been in frame
    bool replacePage;                                           // Determines whether a memory page in frame needs to be replaced

    for (int x = 0; x < TOTAL_MEMORY_PAGES; x++)                
    {
        replacePage = true;
        for (int y = 0; y < wss; y++)                           // Compare the current memory page to every existing memory page in frame
        {
            if (data[x] == process[y])                          // If the page is already in the frame decrease time spent by 1 and break out of while so it can go to next value (hit)
            {
                timeSpentInFrame[y] -= 1;                       // Decrease the amount of time the memory page has been in frame
                replacePage = false;                            // No value needs to be replaced
                continue;
            }
            else if (process[y] == 0 && replacePage == true)    // Frame not fully allocated yet (hit)
            {
                process[y] = data[x];
                replacePage = false;                            // Value already replaced
                break;
            }
            
            timeSpentInFrame[y] += 1;                           // Page isn't present in the frame and frame fully allocated so increase time memory page has spent in frame
            
            if (timeSpentInFrame[y] >= mostTimeSpentInFrame)    // Check to see if there is a new least recently used memory page
            {
                mostTimeSpentInFramePage = y;                   // The new frame page that we want to replace since it is the LRU now
                mostTimeSpentInFrame = timeSpentInFrame[y];     // The new highest amount of time a memory page has been in the frame
            }
        }

        if (replacePage == true)                                // The page is not present in the frame so it needs to be replaced (fault)
        {
            pageFaultCount++;
            process[mostTimeSpentInFramePage] = data[x];        // Replaced the LRU memory page with the new memory page
            timeSpentInFrame[mostTimeSpentInFramePage] = 1;     // Set the new memory page's time spent in frame to 1
            mostTimeSpentInFrame = 1;                           // Reset the value to check for next LRU since the previous LRU is now gone
        }

        /*std::cout << "[";
        for (int x = 0; x < wss; x++){                          // Print arrays to better visualize if the algorithm works
            std::cout << process[x];
            (x != wss - 1) ? (std::cout << ",") : (std::cout << "]\n");
        }*/
    }

    return pageFaultCount;
}


/********************************************************************************************************************************************
 * int FIFOReplacementAlgo(int &wss, int data[])
 * Author: Sean Njenga
 * Modified on: 10 November 2021
 *  by: Sean Njenga
 * Description: Performs the FIFO page replacement algorithm on the provided dataset. Where if a new memory page needs to be allocated it 
 *              removes the memory page (first-in or front of queue) that has been in the frame the longest and replaces it with the newest
 *              memory page at the end of the queue.
 * Parameters: 
 *      FIFOReplacementAlgo      O/P     int     Returns the total number of page faults caused by the FIFO algorithm givin the frame size
 *      wss                      I/P     int     The current working set size or frame size to store pages
 *      data                     I/P     int[]   The randomly generated memory page values to test                
 ********************************************************************************************************************************************/
int FIFOReplacementAlgo(int &wss, int data[]){
    int pageFaultCount = 0;
    std::deque<int> process(wss);       // Holds the current # of pages or processes in the frame

    for (int x = 0; x < TOTAL_MEMORY_PAGES; x++)
    {
        auto search = std::find(process.begin(), process.end(), data[x]);
        if (search == process.end())    // Frame does not contain page (fault) 
        {
            pageFaultCount++;
            process.pop_front();        // Remove first element in frame since it was the first one in
            process.push_back(data[x]); // Push the new page into the back of the deque, since it's the last visited memory page
        }
    }
    return pageFaultCount - wss;        //Returns number of page faults that occured after the frame is fully allocated
}

/********************************************************************************************************************************************
 * int ClockReplacementAlgo(int wss, int data[])
 * Author: Sean Njenga
 * Modified on: 11 November 2021
 *  by: Sean Njenga
 * Description: Performs the Clock page replacement algorithm on the provided dataset. Where every memory page has a "flag" and if it is 0
 *              it means it can be replaced by a new memory page. If it is 1 then it resets that memory page to 0 and moves to the next 
 *              nemory page in the frame.
 * Parameters: 
 *      ClockReplacementAlgo      O/P     int     Returns the total number of page faults caused by the Clock algorithm givin the frame size (wss)
 *      wss                       I/P     int     The current working set size or frame size to store pages
 *      data                      I/P     int[]   The randomly generated memory page values to test                
 ********************************************************************************************************************************************/
int ClockReplacementAlgo(int wss, int data[]){
    std::deque<int> process(wss);                                   
    std::vector<int> flagCache(wss, 0);                                   // Creates an array to track the use bit of each frame. 
    int pageFaultCount = 0;

    int pointer = 0;                                                      // Used as the pointer for the "circular buffer"
    bool flag = false;  

    for(int x = 0; x < TOTAL_MEMORY_PAGES; x++)
    {                   
        flag = false;                                                     // Used to keep track if a memory page is in the frame already

        for(int y = 0; y < wss; y++)                                      // Check to see if the memory page exists in the frame already
        {
            if(process.at(y) == data[x]) 
            {
                flag = true;                            
                flagCache[y] = 1;                                         // Sets the use bit of the frame to 1. 
            }
        }

        if(flag == false)                                                 // Checks to see if the current memory page can be replaced                           
        {
            while(true)                                                   // Loops until a new page has been loaded into a frame. 
            {                                           
                if(flagCache[pointer] == 0)                               // Checks if the current page is eligble for replacement.
                {                         
                    process.erase(process.begin() + pointer);             // Removes the current memory page in the frame pointed to
                    process.insert(process.begin() + pointer, data[x]);   // Inserts a new memory page into the frame in the pointed spot
                    pageFaultCount++;                                       
                    flagCache[pointer] = 1;                               // Sets flag of the memory page to 1, indicating it's been visited

                    if(pointer == wss -1 )                                // Checks if the pointer is at the end of the wss index
                    {                              
                        pointer = 0;                                      // Reset flag to 0
                    } else 
                    {
                        pointer++;                              
                    }
                    break;                                              
                } 
                else if (flagCache[pointer] == 1)                         // Checks if a memory page has not been flagged to replace
                {                          
                    flagCache[pointer] = 0;                               // Sets the flag to 0 if it was visited
                    if(pointer == wss - 1)                                // Check to see if the pointer is at the last frame to reset
                    {                              
                        pointer = 0;                                      // Reset all pages flag to 0
                    } else 
                    {
                        pointer++;
                    }
                }
            }
        }
    }
    return pageFaultCount - wss;                                              //Returns number of page faults that occured after the frame is fully allocated
}


/********************************************************************************************************************************************
 * int FIFOReplacementAlgo(int &wss, int data[])
 * Author: Sean Njenga
 * Modified on: 10 November 2021
 *  by: Sean Njenga
 * Description: Creates the arrays to store the generated data and runs a "Monte Carlo Simulation" against three different page replacement
 *              algorithms, Least Recently Used (LRU), First In First Out (FIFO), and Clock over 1000 experiments. Each randomly generated 
 *              memory page must contain one-thousand page numbers separated into ten regions where each region has a base page number equal
 *              to ten times its region number. Then memory page value is adjusted using a normal, random number distribution with a mean of 
 *              ten and a standard deviation of two.
 * 
 * Parameters: 
*      main            O/P     int         Status code (not currently used).            
 ********************************************************************************************************************************************/
int main(){
    int lruResults[MAX_WORKING_SET_SIZE - MIN_WORKING_SET_SIZE + 1] = { };
    int fifoResults[MAX_WORKING_SET_SIZE - MIN_WORKING_SET_SIZE + 1] = { };
    int clockResults[MAX_WORKING_SET_SIZE - MIN_WORKING_SET_SIZE + 1] = { };
    int data[TOTAL_MEMORY_PAGES];                                                           // Array of pages or data to use for testing algorithms.

    std::default_random_engine gen;
    std::normal_distribution<float> normalDis(MU, SIGMA);                                   // Generate normal distribution given mean and standard deviation

    for (int experiments = 0; experiments < NUM_EXPERIMENTS; experiments++)                 // Loops for however many number of experiments the user wants to run
    {
        for (int trace = 0; trace < TOTAL_MEMORY_PAGES; trace++)                            // Generates the 1000 randomly generated memory page values
        {
            data[trace] = ( 10 * (int) ( trace / 100 ) ) + (int) normalDis(gen);
        }

        for (int wss = MIN_WORKING_SET_SIZE; wss <= MAX_WORKING_SET_SIZE; wss++)            // Tests the algorithms against the randomly generated data against multiple frame size in range [4,20]
        {
            lruResults[wss - MIN_WORKING_SET_SIZE] += LRUReplacementAlgo(wss, data);        // Run the LRU algorithm and total up the number of page faults for each given frame size (wss)
            fifoResults[wss - MIN_WORKING_SET_SIZE] += FIFOReplacementAlgo(wss, data);      // Run the FIFO algorithm and total up the number of page faults for each given frame size (wss)
            clockResults[wss - MIN_WORKING_SET_SIZE] += ClockReplacementAlgo(wss, data);    // Run the Clock algorithm and total up the number of page faults for each given frame size (wss)
        }
    }

    std::cout << "LRU Replacement Algo Sim Using Queue" << std::endl;
    for (int wss = MIN_WORKING_SET_SIZE; wss <= MAX_WORKING_SET_SIZE; wss++)                // Print the average page fault count for the LRU page replacement algorthim after the monte carlo sim for each working set size in range [4,20]
    {
        std::cout << "Working Set Size (" << wss << ") -> PFC = " << lruResults[wss - MIN_WORKING_SET_SIZE] / TOTAL_MEMORY_PAGES << std::endl;
    }

    std::cout << "\nFIFO Replacement Algo Sim" << std::endl;
    for (int wss = MIN_WORKING_SET_SIZE; wss <= MAX_WORKING_SET_SIZE; wss++)                // Print the average page fault count for the FIFO page replacement algorthim after the monte carlo sim for each working set size in range [4,20]
    {
        std::cout << "Working Set Size (" << wss << ") -> PFC = " << fifoResults[wss - MIN_WORKING_SET_SIZE] / TOTAL_MEMORY_PAGES << std::endl;
    }

    std::cout << "\nClock Replacement Algo Sim" << std::endl;
    for (int wss = MIN_WORKING_SET_SIZE; wss <= MAX_WORKING_SET_SIZE; wss++)                // Print the average page fault count for the Clock page replacement algorthim after the monte carlo sim for each working set size in range [4,20]
    {
        std::cout << "Working Set Size (" << wss << ") -> PFC = " << clockResults[wss - MIN_WORKING_SET_SIZE] / TOTAL_MEMORY_PAGES << std::endl;
    }
}