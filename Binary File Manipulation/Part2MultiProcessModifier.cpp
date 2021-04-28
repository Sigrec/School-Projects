#include <unistd.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <sys/wait.h>
#include "Part2MultiProcessModifier.h"
#include "PipeMaker.h"
#include "FileModifyException.h"
#include "FileManipulation.h"
#include "Util.h"

using namespace std;
int childFD, parentFD, pid;

void Part2MultiProcessModifier::doSetup(IOType ioType) {
    if (ioType == IOType::WRITE){
        return;
    }

    PipeMaker pipe;
    pid = fork();
    if (pid < 0){
        std::cerr << "Fork Failed: " << strerror(errno) << std::endl; exit(1);
    }
    else if (pid == 0){ //Child process
        childFD = pipe.setUpToRead();
        execl("21S_CS3377_Project", "21S_CS3377_Project", "2", "3", NULL);
    }
    else{ //Parent process
        parentFD = pipe.setUpToWrite();
    }
}

void Part2MultiProcessModifier::modifyAndCopyFile(const char *sourceFile, const char *destFile) {
    if (pid == 0){ //Child process
        //Read from standard input and write to destFile
        FileManipulation::writeToFile(FileManipulation::getWriteFD(destFile), FileManipulation::readFromFile(STDIN_FILENO, true));
        close(childFD);
        exit(0);
    }
    else if (pid > 0) { //Parent process
        std::vector<EntryInfo> entries = FileManipulation::readFromFile(FileManipulation::getReadFD(sourceFile), false);  //Read from the source file
        FileManipulation::writeToFile(parentFD, entries); //Write to pipe

        //wait for child
        wait(NULL);
//        int code;
//        int rc = wait(&code);
//        if (code != 0){
//            std::cerr << "Child Exit/Wait Error: " << strerror(errno) << std::endl;
//        }
        close(parentFD);
    