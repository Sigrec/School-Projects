#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Part4SocketModifier.h"
#include "FileModifyException.h"
#include "PipeMaker.h"
#include "FileManipulation.h"
#include "Util.h"

void Part4SocketModifier::doSetup(IOType ioType) {
    this->ioType = ioType;
    if (ioType == IOType::WRITE){
        return;
    }

    PipeMaker pipe;
    int pid2 = fork();
    if (pid2 > 0){
        pipe.setUpToWrite();
    }
    else if (pid2 == 0){ //Child process
        pipe.setUpToRead();
        execl("21S_CS3377_Project", "21S_CS3377_Project", "4", "3", nullptr);
    }
    else{ //Parent process
        std::cerr << "Fork Failed: " << strerror(errno) << std::endl; exit(1);
    }
}

void Part4SocketModifier::modifyAndCopyFile(const char *sourceFile, const char *destFile) {
    if (ioType == IOType::READ){ //Parent process
        int sockFD;
        struct sockaddr_in serverAddress;
        if ((sockFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) { perror("Parent Socket Failed"); };

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(Util::portNumber);
        if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) { perror("Inet Pton Failed"); }

        int waitTime = 1;
        while (connect(sockFD, (struct sockaddr*) &serverAddress, sizeof(serverAddress))) {
            if (errno != ECONNREFUSED) {
                throw FileModifyException("Error connecting");
            }
            std::cout << "Not ready to connect yet...\n";
            sleep(waitTime);
            waitTime = waitTime * 2;
        }

        FileManipulation::writeToFile(sockFD, FileManipulation::readFromFile(FileManipulation::getReadFD(sourceFile), false)); //Write to socket
        close(sockFD);
    }
    else{ //Child process
        int finalSockFD, childSockFD;
        struct sockaddr_in childAddress;
        if ((childSockFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) { perror("Child Socket Failed"); };

        childAddress.sin_family = AF_INET;
        childAddress.sin_addr.s_addr = INADDR_ANY;
        childAddress.sin_port = htons(Util::portNumber);
        int addrSize = sizeof(childAddress);

        if (bind(childSockFD, (struct sockaddr*) &childAddress, addrSize) < 0) { perror("Bind Failed"); }
        if (listen(childSockFD, 1) < 0) { perror("Listen Failed"); }
        if ((finalSockFD = accept(childSockFD, (struct sockaddr*) &childAddress, (socklen_t*) &addrSize)) < 0) { perror("Accept Failed"); };

        FileManipulation::writeToFile(FileManipulation::getWriteFD(destFile), FileManipulation::readFromFile(finalSockFD, true));
        close(finalSockFD); close(childSockFD); exit(0);
    }
}