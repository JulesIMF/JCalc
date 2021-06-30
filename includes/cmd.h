/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    cmd.h

Abstract:

    Describes cmd common things.

Author / Creation date:

    JulesIMF / 30.06.2021

Revision History:

--*/

#ifndef JCALC_CMD
#define JCALC_CMD

//
// Includes / usings
//

//
// Defines
//

/**
 * Termination function.
 * TODO: history savings
*/

typedef void* (*CMD_HANDLER_PTR)(char const*);

struct CmdHandler
{
    char const*     cmd;
    CMD_HANDLER_PTR handler;
};

bool handleCmd(char const* input);

void terminate(int param);

void* exitHandler(char const* input);
void* helpHandler(char const* input);

#endif // !JCALC_CMD