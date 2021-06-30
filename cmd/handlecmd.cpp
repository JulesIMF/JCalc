/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    handlecmd.cpp

Abstract:

    Defines cmd handler.

Author / Creation date:

    JulesIMF / 30.06.2021

Revision History:

--*/

//
// Includes / usings
//

#include <cmd.h>
#include <cstring>

//
// Defines
//

bool handleCmd(char const* input)
{
    CmdHandler hanlders[] = 
    {
        { "exit", exitHandler },
        { "help", helpHandler }
    };

    constexpr int nHandlers = sizeof(hanlders) / sizeof(CmdHandler);

    for (int i = 0; i != nHandlers; i++)
    {
        if ( !strncmp(hanlders[i].cmd, input, strlen(hanlders[i].cmd)) )
        {
            hanlders[i].handler(input + strlen(hanlders[i].cmd)); // call the handler only with params
            return true;
        }
    }

    return false;
}