#ifndef PROJECTTEMPLATE_FILEMANIPULATION_H
#define PROJECTTEMPLATE_FILEMANIPULATION_H

#include "Util.h"
#include <vector>

class FileManipulation {
public:
    static int getReadFD(const char *sourceFile);
    static std::vector<EntryInfo> readFromFile(int fd, bool newEntries);
    static int getWriteFD(const char* destFile);
    static void writeToFile(int fd, std::vector<EntryInfo> entries);
};


#endif //PROJECTTEMPLATE_FILEMANIPULATION_H