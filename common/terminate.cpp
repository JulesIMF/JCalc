/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    terminate.h

Abstract:

    Defines termination function.

Author / Creation date:

    JulesIMF / 30.06.2021

Revision History:

--*/

#include <cstdio>
#include <err.h>
#include <colors.h>
#include <cmd.h>
#include <cstdlib>

//
// Includes / usings
//

//
// Defines
//

void terminate(int param)
{
    printf("%s\n\tTerminating with param = %d%s\n", TerminalColor::BlueB, param, TerminalColor::Default);
    auto error = ExitCode::NO_ERROR;
    _Exit((int)error);
}