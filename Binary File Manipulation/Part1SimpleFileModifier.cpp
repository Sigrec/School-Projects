#include "Part1SimpleFileModifier.h"
#include "FileModifyException.h"
#include "FileManipulation.h"
#include <vector>
#include "Util.h"

using namespace std;

void Part1SimpleFileModifier::modifyAndCopyFile(const char *sourceFile, const char *destFile) {
    std::vector<EntryInfo> entries = FileManipulation::readFromFile(FileManipulation::getReadFD(sourceFile), true);
    FileManipulation::writeToFile(FileManipulation::getWriteFD(destFile), entries);
}