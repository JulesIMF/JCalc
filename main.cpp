/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    main.cpp

Abstract:

    Main section, where the programm starts.

Author / Creation date:

    JulesIMF / 26.06.2021     

Revision History:

--*/


//
// Includes / usings
//
#include <iostream>
#include <string>
#include <csignal>
#include <err.h>
#include <colors.h>
#include <lexer.h>
#include <parser.h>
#include <drvdebug.h>
#include <messages.h>
#include <cmd.h>


using std::string;

//
// Defines
//
static char const* version = "Indev 3 (builded on " __DATE__ ")";

/**
 * Displays information about the app.
*/
void displayInfo(void)
{
    string info = 
        static_cast<string>("JCalc - console calculator. Version ") + version + ".\n" +
        "[Type \"exit\" to exit, or \"help\" for help.]"; // don`t want to concat it manually
        
    printf("%s%s%s\n", TerminalColor::BlueB, info.c_str(), TerminalColor::Default);
}


/**
 * Displays information about the app.
*/
void displayPrompt(void)
{
    char const* prompt = ";\t";
    printf("%s%s%s", TerminalColor::Yellow, prompt, TerminalColor::Default);
}

/**
 * Application entry point.
*/
int main()
{
    
    signal(SIGINT, terminate);  //
    signal(SIGSTOP, terminate); // If SIG_ERR returned - so what? Default handler still exists
    signal(SIGTERM, terminate); //
    displayInfo();
    
    while(true)
    {
        setDebug(true);
        string input;
        displayPrompt();
        std::getline(std::cin, input);

        if (handleCmd(input.c_str()))
            continue;

        auto lexed = lexer(input.c_str());
        if (lexed.lexerError == LexerError::UNKNOWN_TOKEN)
        {
            errorMessage("unknown token!");
            continue;
        }

        dumpLexer(lexed); 
        
        Parser parser(lexed);
        auto parsed = parser.getLine();

        if (parsed.parserError != ParserError::NO_ERROR)
        {
            errorMessage("%s!", parserErrorToStr(parsed.parserError));
            continue;
        }

        dumpParser(parsed);
    }
}