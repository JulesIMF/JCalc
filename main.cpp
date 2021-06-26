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
#include <drvdebug.h>


using std::string;

//
// Defines
//
static char const* version = "Indev 1 (builded on " __DATE__ ")";

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
 * Termination function.
 * TODO: history savings
*/
void terminate(int param)
{
    printf("%s\n\tTerminating with param = %d%s\n", TerminalColor::BlueB, param, TerminalColor::Default);
    auto error = ExitCode::NO_ERROR;
    _Exit( (int)error );
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
    string input;
    
    while(true)
    {
        displayPrompt();
        std::getline(std::cin, input);
        auto lexed = lexer(input.c_str());
    }
}