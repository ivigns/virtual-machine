#include "Constants.h"

const int DataSize = 128 * 1024;
const int WordSize = 4;
const int MaxConstSize = 128;
const char NullChar = 'N';
const char HeaderChar = 0xd0;
const int InstructionPtr = 1;
const int StackPtr = 2;
const int EntryPtr = 3;
const std::map<std::string, char> CommandCodes = {
    { "push", 0 },
    { "pop", 1 },
    { "call", 2 },
    { "copy", 3 },
    { "load", 4 },
    { "add", 5 },
    { "sub", 6 },
    { "isequal", 7 },
    { "if", 8 },
    { "return", 9 },
    { "exit", 10 },
    { "print", 11 },
    { "printconst", 12 },
    { "scan", 13 },
    { "exec", 14 }
};