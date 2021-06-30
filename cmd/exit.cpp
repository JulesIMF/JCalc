/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    exit.cpp

Abstract:

    Defines exit handler.

Author / Creation date:

    JulesIMF / 30.06.2021

Revision History:

--*/

//
// Includes / usings
//

#include <cmd.h>
#include <err.h>

//
// Defines
//

void* exitHandler(char const*)
{
    terminate((int)ExitCode::NO_ERROR);
    return nullptr; // this won`t be reached normally, but either way...
}