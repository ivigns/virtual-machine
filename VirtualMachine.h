#pragma once
#include <iostream>
#include <string>
#include <vector>

class CVirtualMachine {

public:
    CVirtualMachine() {}

    // Load binary file
    void Load( const std::string& binFileName );
    // Execute loaded binary file and return 0 if success
    int Exec();

private:
    std::vector<char> bin;  // Binary file to execute

    // Execute next command on position of instruction ptr
    bool doStep();

    // Get word reference
    int& getWord( int ptr );
};