/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    help.cpp

Abstract:

    Defines help handler.

Author / Creation date:

    JulesIMF / 30.06.2021

Revision History:

--*/

//
// Includes / usings
//

#include <cstdio>
#include <cmd.h>

//
// Defines
//

void* helpHandler(char const*)
{
    printf("Soon there will be help... :)\n");
    return nullptr;
}