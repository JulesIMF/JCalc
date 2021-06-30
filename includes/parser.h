/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    parser.h

Abstract:

    Parser glues tokens into a tree.

Author / Creation date:

    JulesIMF / 28.06.2021

Revision History:

--*/

#ifndef JCALC_PARSER
#define JCALC_PARSER

//
// Includes / usings
//

#include <vertex.h>
#include <lexer.h>

//
// Defines
//

struct Parsed
{
    Vertex* ast;
    ParserError parserError = ParserError::NO_ERROR;

    std::vector<Vertex*>* trash; // vertexes that need to be deleted but do not belong to the AST

    static void deleteTree(Vertex* vertex);
    void freeParsed(void);
};

class Parser
{
private:
    unsigned index      = 0;
    Vertex* current     = nullptr;
    ParserError error   = ParserError::NO_ERROR;  
    bool notEnd         = true;
    std::vector<Vertex*>* tokens;
    std::vector<Vertex*>* trash;

public:
    Parser(Lexed lexed_);
    Parsed getLine(void);

private:
    Vertex* getReq  (void);
    Vertex* getExpr (void);
    Vertex* getTerm (void);
    Vertex* getPrim (void);
    Vertex* getCall (void);
    Vertex* getAsgn (void);

    void inc(int shift = 1);
    Vertex* atShiftOf(int shift);
};

char const *parserErrorToStr(ParserError error);

#endif // !JCALC_PARSER