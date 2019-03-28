#pragma once
#include <map>

extern const int DataSize;  // Max size of data section in out file
extern const int WordSize;  // Machine word size
extern const int MaxConstSize;  // Max size of string constant
extern const char NullChar; // Filling of useless bytes in bin file
extern const char HeaderChar;   // First word of bin file consists of these chars
extern const int InstructionPtr;
extern const int StackPtr;
extern const int EntryPtr;
// Assembler commands' codes
extern const std::map<std::string, char> CommandCodes;