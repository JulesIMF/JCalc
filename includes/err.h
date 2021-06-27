/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    errors.h

Abstract:

    Error codes definitions and descriptions.

Author / Creation date:

    JulesIMF / 26.06.2021

Revision History:

--*/

#ifndef JCALC_ERRORS
#define JCALC_ERRORS

//
// Includes / usings
//


//
// Defines
//

enum class ExitCode
{
    NO_ERROR = 0,
};

enum class LexerError
{
    NO_ERROR,
    UNKNOWN_TOKEN,
};

enum class ParserError
{
    NO_ERROR,
    EXPECTED_REQ,
    EXPECTED_TERM,
    EXPECTED_PRIM,
};

#endif // !JCALC_ERRORS
