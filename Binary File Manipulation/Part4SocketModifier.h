#ifndef INC_21S_CS3377_PROJECT_PART4SOCKETMODIFIER_H
#define INC_21S_CS3377_PROJECT_PART4SOCKETMODIFIER_H


#include "Modifier.h"

class Part4SocketModifier : public Modifier {
public:
    void doSetup(IOType ioType) override;
    void modifyAndCopyFile(const char* sourceFile,
                           const char* destFile) override;
private:
    IOType ioType;
};


#endif //INC_21S_CS3377_PROJECT_PART4SOCKETMODIFIER_H