#include "FileManipulation.h"
#include "FileModifyException.h"
#include "Util.h"
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <cstring>

int numItems;
time_t dateAndTime;
int itemID;
int quantity;
float price;
const int bufferSize = 50;
char itemName[bufferSize];
char* copy;

int FileManipulation::getReadFD(const char *sourceFile){
    int fd = open(sourceFile, O_RDONLY, S_IRUSR);
    if (fd < 0) {
        std::cerr << "Source File Error: " << strerror(errno) << std::endl; exit(1);
    }
    return fd;
}
std::vector<EntryInfo> FileManipulation::readFromFile(int fd, bool newEntries){
    read(fd, &numItems, sizeof(int));
    if (numItems < 0){
        close(fd);
        throw FileModifyException("File is Empty");
    }

    int count = 1;
    std::vector<EntryInfo> entries;
    while (count <= numItems){
        read(fd, &dateAndTime, sizeof(time_t));
        read(fd, &itemID, sizeof(int));
        read(fd, itemName, bufferSize * sizeof(char));
        copy = new char[bufferSize];
        strncpy(copy, itemName, bufferSize);
        read(fd, &quantity, sizeof(int));
        read(fd, &price, sizeof(float));
        EntryInfo entry = {.timestamp = dateAndTime, .itemID = itemID, .itemName = copy + '\0', .quantity = quantity, .price = price};
        entries.insert(entries.end(), entry);
        count++;
    }

    if (newEntries){
        numItems += 2;
        EntryInfo newEntry1 = {.timestamp = 1612195200, .itemID = 4636152, .itemName = "A Programming Guide to Linux Commands, Editors, and Shell Programming by Sobell", .quantity = 70, .price = 70.99};
        EntryInfo newEntry2 = {.timestamp = 1613412000, .itemID = 6530927, .itemName = "Advanced Programming in the UNIX Environment by Stevens and Rago", .quantity = 68, .price = 89.99};
        entries.push_back(newEntry1); entries.push_back(newEntry2);
    }

    for (auto& entry : entries){
        copy = new char[bufferSize];
        strncpy(copy, entry.itemName, bufferSize - 1);
        entry.itemName = copy;
    }

    close(fd);
    return entries;
}

int FileManipulation::getWriteFD(const char *destFile){
    int fd = open(destFile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0){
        std::cerr << "Destination File Error: " << strerror(errno) << std::endl; exit(1);
    }
    return fd;
}
void FileManipulation::writeToFile(int fd, std::vector<EntryInfo> entries){
    write(fd, &numItems, sizeof(int));
    for (auto const& entry : entries){
        write(fd, &entry.timestamp, sizeof(time_t));
        write(fd, &entry.itemID, sizeof(int));
        write(fd, entry.itemName, bufferSize * sizeof(char));
        write(fd, &entry.quantity, sizeof(int));
        write(fd, &entry.price, sizeof(float));
    }

    close(fd);
}