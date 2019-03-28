#include "VirtualMachine.h"

#include <fstream>
#include "Constants.h"

void CVirtualMachine::Load( const std::string& binFileName )
{
    bin.assign( 0, 0 );
    std::ifstream binFile( binFileName );
    
    int c = 0;
    while( !binFile.eof() ) {
        c = binFile.get();
        bin.push_back( static_cast<char>(c) );
    } 
}

int CVirtualMachine::Exec()
{
    for( int i = 0; i < WordSize; ++i ) {   // Check header
        if( bin[i] != HeaderChar ) {
            throw std::exception( "file is not binary file!" );
        }
    }

    // Initialise instruction ptr
    getWord( InstructionPtr ) = getWord( EntryPtr );

    while( doStep() ) {}

    // 
    return getWord( InstructionPtr );
}

bool CVirtualMachine::doStep()
{
    int& instructionPtrCell = getWord( InstructionPtr );
    int& stackPtrCell = getWord( StackPtr );
    int& instruction = getWord( instructionPtrCell );
    // Remember that int reverses bytes
    char commandCode = instruction & 0xffU;

    int source = 0;
    int destination = 0;
    if( commandCode == CommandCodes.at( "push" ) ) {
        source = (instruction & 0xffffff00U) >> 8;

        getWord( stackPtrCell-- ) = getWord( source );    // Programm stack grows to the left

        ++instructionPtrCell;
    } else if( commandCode == CommandCodes.at( "pop" ) ) {
        destination = (instruction & 0xffffff00U) >> 8;

        getWord( destination ) = getWord( ++stackPtrCell );

        ++instructionPtrCell;
    } else if( commandCode == CommandCodes.at( "call" ) ) {
        source = (instruction & 0xffffff00U) >> 8;

        getWord( stackPtrCell-- ) = instructionPtrCell + 1;
        instructionPtrCell = source;
    } else if( commandCode == CommandCodes.at( "copy" ) ) {
        source = (instruction & 0x0000ff00U) >> 8;
        destination = (instruction & 0xffff0000U) >> 16;

        getWord( destination ) = getWord( source );

        ++instructionPtrCell;
    } else if( commandCode == CommandCodes.at( "load" ) ) {
        source = (instruction & 0x00ffff00U) >> 8;
        destination = (instruction & 0xff000000U) >> 24;

        getWord( destination ) = getWord( source );

        ++instructionPtrCell;
    } else if( commandCode == CommandCodes.at( "add" ) ) {
        source = (instruction & 0x0000ff00U) >> 8;
        destination = (instruction & 0xffff0000U) >> 16;

        getWord( destination ) += getWord( source );

        ++instructionPtrCell;
    } else if( commandCode == CommandCodes.at( "sub" ) ) {
        source = (instruction & 0x0000ff00U) >> 8;
        destination = (instruction & 0xffff0000U) >> 16;

        getWord( destination ) -= getWord( source );

        ++instructionPtrCell;
    } else if( commandCode == CommandCodes.at( "isequal" ) ) {
        source = (instruction & 0x0000ff00U) >> 8;
        destination = (instruction & 0xffff0000U) >> 16;

        getWord( stackPtrCell-- ) = getWord( source ) == getWord( destination ) ? 1 : 0;

        ++instructionPtrCell;
    } else if( commandCode == CommandCodes.at( "if" ) ) {
        source = (instruction & 0xffffff00U) >> 8;

        if( getWord( source ) ) {
            ++instructionPtrCell;
        } else {
            instructionPtrCell += 2;
        }
    } else if( commandCode == CommandCodes.at( "return" ) ) {
        instructionPtrCell = getWord( ++stackPtrCell );
    } else if( commandCode == CommandCodes.at( "exit" ) ) {
        instructionPtrCell = 0; // Successfully exit
        return false;
    } else if( commandCode == CommandCodes.at( "print" ) ) {
        source = (instruction & 0xffffff00U) >> 8;

        std::cout << getWord( source ) << "\n";

        ++instructionPtrCell;
    } else if( commandCode == CommandCodes.at( "printconst" ) ) {
        source = (instruction & 0xffffff00U) >> 8;

        std::cout << &bin[source * WordSize] << "\n";

        ++instructionPtrCell;
    } else if( commandCode == CommandCodes.at( "scan" ) ) {
        destination = (instruction & 0xffffff00U) >> 8;

        std::cin >> getWord( destination );

        ++instructionPtrCell;
    } else if( commandCode == CommandCodes.at( "exec" ) ) {
        source = (instruction & 0xffffff00U) >> 8;

        instructionPtrCell = source;
    } else {
        instructionPtrCell = 42; // Bad exit
        return false;
    }

    return true;
}

int& CVirtualMachine::getWord( int ptr )
{
    return *reinterpret_cast<int*>(&bin[ptr * WordSize]);
}
