#include "Assembler.h"

#include <algorithm>
#include "Constants.h"

CAssembler::CAssembler() :
    out( DataSize, NullChar ),
    dataPtr( 5 )
{
    for( int i = 0; i < WordSize; ++i ) {
        out[i] = HeaderChar;
    }
    getWord( InstructionPtr ) = 0;
    getWord( StackPtr ) = DataSize / WordSize - 1;
}

void CAssembler::Compile( const std::string& codeFileName, const std::string& outFileName )
{
    codeFile = std::ifstream( codeFileName );
    
    std::string section;
    while( !codeFile.eof() ) {
        codeFile >> section;
        if( section == "var" ) {
            loadVariables();
        } else if( section == "const" ) {
            loadConstants();
        } else if( section == "def" ) {
            loadFunctions();
        } else {
            throw std::runtime_error( "unknown section detected" );
        }
    }

    std::ofstream outFile( outFileName );
    for( int i = 0; i < out.size(); ++i ) {
        outFile << out[i];
    }
}

void CAssembler::loadVariables()
{
    std::string name, var;
    codeFile >> var;
    checkSyntax( var, "{" );
    
    codeFile >> name;
    while( name != "}" ) {
        codeFile >> var;
        variables[name] = dataPtr;

        getWord( dataPtr ) = std::stoi( var );
        dataPtr++;

        codeFile >> name;
    }
}

void CAssembler::loadConstants()
{
    std::string name, cnst;
    codeFile >> cnst;
    checkSyntax( cnst, "{" );

    codeFile >> name;
    while( name != "}" ) {
        cnst.assign( MaxConstSize, 0 );
        codeFile.get();
        codeFile.getline( &cnst[0], MaxConstSize );
        // Erase redundant null chars
        cnst.erase( std::find( cnst.begin(), cnst.end(), 0 ), cnst.end() );
        constants[name] = dataPtr;

        int currentDataPtr = dataPtr * WordSize;    // currentDataPtr is index for 'out' array!
        for( int i = 0; i < cnst.length() + 1; ++i, ++currentDataPtr ) {
            if( currentDataPtr >= dataPtr * WordSize ) {
                dataPtr++;
            }
            out[currentDataPtr] = (i != cnst.length() ? cnst[i] : 0);
        }

        codeFile >> name;
    }
}

void CAssembler::loadFunctions()
{
    std::string name;
    codeFile >> name;
    checkSyntax( name, "{" );

    codeFile >> name;
    while( name != "}" ) {
        functions[name] = out.size() / WordSize;
        if( name == "main" ) {  // Fill entrypoint
            getWord( EntryPtr ) = functions[name];
        }
        loadStatic( name );
        loadCode( name );

        codeFile >> name;
    }
}

void CAssembler::loadStatic( const std::string& function )
{
    std::string name, var;
    codeFile >> var;
    checkSyntax( var, "{" );
    codeFile >> var;
    checkSyntax( var, "static" );
    codeFile >> var;
    checkSyntax( var, "{" );

    codeFile >> name;
    while( name != "}" ) {
        codeFile >> var;
        staticVariables[function]["&" + name] = dataPtr;

        getWord( dataPtr++ ) = std::stoi( var );

        codeFile >> name;
    }
}

void CAssembler::loadCode( const std::string& function )
{
    std::string command;
    codeFile >> command;
    while( command != "}" ) {
        loadCommand( function, command );

        codeFile >> command;
    }
}

void CAssembler::loadCommand( const std::string& function, const std::string& command )
{
    out.push_back( CommandCodes.at( command ) );

    std::string firstArg, secondArg;
    if( command == "push" || command == "pop" || command == "call" || command == "if" ||
        command == "print" || command == "printconst" || command == "scan" || command == "exec") {
        // Commands with one argument
        codeFile >> firstArg;
        int address = 0;
        if( command == "call" || command == "exec" ) {
            address = functions.at( firstArg );
        } else if( command == "printconst" ) {
            address = constants.at( firstArg );
        } else {
            address = firstArg[0] == '&' ? staticVariables.at( function ).at( firstArg ) :
                variables.at( firstArg );
        }
        out.push_back( (address & 0x0000ff) );
        out.push_back( (address & 0x00ff00) >> 8 );
        out.push_back( (address & 0xff0000) >> 16 );
    } else if( command == "copy" || command == "load" || command == "add" || command == "sub" ||
        command == "isequal" ) {
        // Commands with two arguments
        codeFile >> firstArg >> secondArg;
        int source = firstArg[0] == '&' ? staticVariables.at( function ).at( firstArg ) :
            variables.at( firstArg );
        int destination = secondArg[0] == '&' ? staticVariables.at( function ).at( secondArg ) :
            variables.at( secondArg );
        if( command == "load" ) {   // in load first arg is large
            out.push_back( (source & 0x00ff) );
            out.push_back( (source & 0xff00) >> 8 );
            out.push_back( destination & 0xff );
        } else {    // in other commands first arg is small
            out.push_back( source & 0xff );
            out.push_back( (destination & 0x00ff) );
            out.push_back( (destination & 0xff00) >> 8 );
        }
    } else if( command == "return" || command == "exit" ) {
        // Commands without arguments
        out.push_back( 0 );
        out.push_back( 0 );
        out.push_back( 0 );
    }
}

int& CAssembler::getWord( int ptr )
{
    return *reinterpret_cast<int*>(&out[ptr * WordSize]);
}

void CAssembler::checkSyntax( const std::string& test, const std::string& original )
{
    if( test != original ) {
        throw std::runtime_error( "syntax error" );
    }
}
