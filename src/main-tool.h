#ifndef MAINTOOL_H
#define MAINTOOL_H

enum ReturnValues {
    InputExpected = 100,
    InputReadError = 101,
    OutputExists = 200,
    OutputWriteError = 201,
    OutputRemoveFailed = 202,
};

#endif // MAINTOOL_H
