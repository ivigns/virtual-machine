#include <iostream>
#include <string>

#include "Assembler.h"
#include "VirtualMachine.h"

int main( int argc, char** argv )
{
    int returnCode = 0;

    if( argc < 2 || std::string( argv[1] ) == "-h" || std::string( argv[1] ) == "--help" ) {
        std::cout << "Virtual Machine based on Von Neumann architecture\n"
            "Usage:\n\tvnvm assembler <code.asm> [out.bin]- build binary file from assembler code\n"
            "\tvnvm exec <filename.bin> - execute binary file\n"
            "\nMore info on https://github.com/ivigns/virtual-machine\n";
    } else if( std::string( argv[1] ) == "assembler" ) {
        if( argc < 3 ) {
            std::cerr << argv[1] << ": Not enough arguments.\n";
            return 1;
        }

        CAssembler assembler;
        try {
            if( argc < 4 ) {
                assembler.Compile( argv[2] );
            } else {
                assembler.Compile( argv[2], argv[3] );
            }
        } catch( std::exception& exception ) {
            std::cerr << argv[1] << ": " << exception.what() << "\n";
            return 1;
        }
    } else if( std::string( argv[1] ) == "exec" ) {
        if( argc < 3 ) {
            std::cerr << argv[1] << ": not enough arguments\n";
            return 1;
        }

        CVirtualMachine vm;
        vm.Load( argv[2] );
        try {
            returnCode = vm.Exec();
        } catch( std::exception& exception ) {
            std::cerr << argv[1] << ": " << exception.what() << "\n";
            return 1;
        }
    } else {
        std::cerr << argv[1] << ": unknown command\n";
        return 1;
    }

    return returnCode;
}