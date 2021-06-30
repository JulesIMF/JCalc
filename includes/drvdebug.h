/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    drvdebug.h

Abstract:
    
    Driver debug functions implementations.

Author / Creation date:

    JulesIMF / 27.06.2021

Revision History:

--*/

#ifndef JCALC_DRVDEBUG
#define JCALC_DRVDEBUG

//
// Includes / usings
//

#include <lexer.h>
#include <parser.h>

//
// Defines
//

void dumpLexer(Lexed lexed);
void dumpParser(Parsed parsed);

#endif // !JCALC_DRVDEBUG