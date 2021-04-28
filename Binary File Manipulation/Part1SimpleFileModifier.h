//
// Created by erik on 2/2/21.
//

#ifndef INC_21S_CS3377_PROJECT_PART1SIMPLEFILEMODIFIER_H
#define INC_21S_CS3377_PROJECT_PART1SIMPLEFILEMODIFIER_H


#include "Modifier.h"

class Part1SimpleFileModifier : public Modifier {
public:
    // You shouldn't need to do any setup, but fill in if needed
    void doSetup(IOType ioType) override {}
    void modifyAndCopyFile(const char* sourceFile,
                          const char* destFile) override;
};


#endif //INC_21S_CS3377_PROJECT_PART1SIMPLEFILEMODIFIER_H