#pragma once
#include <fstream>
#include <map>
#include <string>
#include <vector>

class CAssembler {

public:
    CAssembler();
    void Compile( const std::string& codeFileName, const std::string& outFileName = "NewProgramm.bin" );

private:
    std::ifstream codeFile; // File with assembler code
    std::vector<char> out;  // Binary file for output
    int dataPtr;    // Address of the end of data section
    std::map<std::string, int> constants;   // String constants, <name, word number>
    std::map<std::string, int> variables;   // Integer variables, <name, word number>
    std::map<std::string, int> functions;   // Executable functions, <name, word number>
    // Static integer variables for functions, <function name, <variable name, word number>>
    std::map<std::string, std::map<std::string, int>> staticVariables;

    // Load variables from code
    void loadVariables();
    // Load constants from code
    void loadConstants();
    // Load functions from code
    void loadFunctions();
    // Load static variables for function from code
    void loadStatic( const std::string& function );
    // Load code of function
    void loadCode( const std::string& function );
    // Load assembler command with arguments
    void loadCommand( const std::string& function, const std::string& command );
    // Get word reference
    int& getWord( int ptr );

    // Checks if test == original
    static void checkSyntax( const std::string& test, const std::string& original );
};