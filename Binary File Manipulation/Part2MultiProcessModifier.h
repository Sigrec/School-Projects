//
// Created by Erik Peterson on 2/10/21.
//

#ifndef INC_21S_CS3377_PROJECT_PART2MULTIPROCESSMODIFIER_H
#define INC_21S_CS3377_PROJECT_PART2MULTIPROCESSMODIFIER_H


#include "Modifier.h"

class Part2MultiProcessModifier : public Modifier {
public:
    void doSetup(IOType ioType) override;
    void modifyAndCopyFile(const char* sourceFile,
                           const char* destFile) override;

private:
};


#endif //INC_21S_CS3377_PROJECT_PART2MULTIPROCESSMODIFIER_H