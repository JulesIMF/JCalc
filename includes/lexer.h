/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    lexer.cpp

Abstract:

    Lexer proceeds lexical analysis.

Author / Creation date:

    JulesIMF / 27.06.2021

Revision History:

--*/

#ifndef JCALC_LEXER
#define JCALC_LEXER

#include <vector>
#include <vertex.h>
#include <err.h>

struct Lexed
{
    std::vector<Vertex*> tokens;
    LexerError lexerError = LexerError::NO_ERROR;
};

Lexed lexer(char const *src);

#endif // !JCALC_LEXER