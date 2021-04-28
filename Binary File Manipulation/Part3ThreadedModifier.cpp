#include <iostream>
#include <vector>
#include "Part3ThreadedModifier.h"
#include "FileModifyException.h"
#include "FileManipulation.h"
#include "Util.h"
#include "Modifier.h"

using namespace std;
std::vector<EntryInfo> outputEntries;
const char* outputFile;

void Part3ThreadedModifier::doSetup(IOType ioType) {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&condition, nullptr);
}

void Part3ThreadedModifier::modifyAndCopyFile(const char *sourceFile, const char *destFile) {
    outputFile = destFile;
    std::vector<EntryInfo> entries = FileManipulation::readFromFile(FileManipulation::getReadFD(sourceFile), true);
    pthread_mutex_lock(&mutex);

    pthread_t outputThread;
    pthread_create(&outputThread, nullptr, threadEntry, this);

    for (int x = 0; x < entries.size(); x++){
        pthread_cond_wait(&condition, &mutex);
        infoBtwThreads = entries[x];;
        pthread_cond_signal(&condition);
    }
    pthread_mutex_unlock(&mutex);
    pthread_join(outputThread, nullptr);

    //Clean-up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
}

void Part3ThreadedModifier::outputThread() {
    pthread_mutex_lock(&mutex);
    for (int x = 0; x < 7; x++){
        pthread_cond_signal(&condition);
        pthread_cond_wait(&condition, &mutex);
        outputEntries.push_back(infoBtwThreads);
    }
    pthread_mutex_unlock(&mutex);
    FileManipulation::writeToFile(FileManipulation::getWriteFD(outputFile), outputEntries);
}

// Use this as the starting point for the thread you create
void *Part3ThreadedModifier::threadEntry(void* arg) noexcept{
    Part3ThreadedModifier* modifier = (Part3ThreadedModifier*) arg;
    try{
        modifier->outputThread();
    } catch (FileModifyException e){
        cerr << "Receiving thread failed: " << e.what() << endl;
        exit(1);
    } catch (exception e){
        cerr << "Receiving thread failed: " << e.what() << endl;
        exit(1);
    } catch (...){
        cerr << "Unknown error in receiving thread: " << endl;
        exit(1);
    }
    return nullptr;
}