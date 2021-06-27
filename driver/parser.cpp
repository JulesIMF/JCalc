/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    parser.cpp

Abstract:

    Parser glues tokens into a tree.

Author / Creation date:

    JulesIMF / 28.06.2021

Revision History:

--*/

//
// Includes / usings
//

#include <vertex.h>
#include <err.h>
#include <messages.h>
#include <lexer.h>
#include <parser.h>

//
// Defines
//

Parser::Parser(Lexed lexed) : tokens(lexed.tokens) {}

Parsed Parser::getLine(void)
{
    /**
     * REQ
    */
    auto line = getReq();

    /**
     * {';' REQ}*
    */
    while (error == ParserError::NO_ERROR       &&
           notEnd                               &&
           current->type == VertexType::Linker)
    {
        auto linker = current;
        inc();
        auto newReq = getReq();
        linker->left = line;
        linker->right = newReq;
        line = linker;
    }

    return { .ast           = line,
             .parserError   = error};
}

Vertex* Parser::getReq(void)
{
    /**
     * EXPR | ASGN | _epsilon
    */

    if (current->type == VertexType::Linker) // _epsilon
        return nullptr;
    

    auto req = getExpr();   // EXPR
    if (error != ParserError::NO_ERROR) return nullptr;

    if (req)
        return req;
    
    req = getAsgn();   // ASGN
    if (error != ParserError::NO_ERROR) return nullptr;

    if (req)
        return req;

    error = ParserError::EXPECTED_REQ;
    return nullptr;
}

Vertex* Parser::getExpr(void)
{
    /**
     * TERM {['+' '-'] TERM}*
    */

    auto expr = getTerm(); // TERM
    if (error != ParserError::NO_ERROR) return nullptr;
    if (expr == nullptr)
    {
        return nullptr;
    }


    /**
     * {['+' '-'] TERM}*
    */
    while (notEnd && 
           current->type == VertexType::Operator &&
           (static_cast<Operator*>(current)->opType == '+' || 
            static_cast<Operator*>(current)->opType == '-'))
    {
        auto op = current;
        inc();
        auto newTerm = getTerm();
        if (error != ParserError::NO_ERROR) return nullptr;

        if (newTerm == nullptr)
        {
            error = ParserError::EXPECTED_TERM;
            return nullptr;
        }

        op->left = expr;
        op->right = newTerm;
        expr = op;
    }

    return expr;
}

Vertex* Parser::getTerm(void)
{
    /**
     * PRIM {['*' '/'] PRIM}*
    */

    auto term = getPrim(); // PRIM
    if (error != ParserError::NO_ERROR) return nullptr;
    if (term == nullptr)
    {
        return nullptr;
    }


    /**
     * {['*' '/'] PRIM}*
    */
    while (notEnd && 
           current->type == VertexType::Operator &&
           (static_cast<Operator*>(current)->opType == '*' || 
            static_cast<Operator*>(current)->opType == '/'))
    {
        auto op = current;
        inc();
        auto newPrim = getPrim();
        if (error != ParserError::NO_ERROR) return nullptr;

        if (newPrim == nullptr)
        {
            error = ParserError::EXPECTED_PRIM;
            return nullptr;
        }

        op->left = term;
        op->right = newPrim;
        term = op;
    }

    return term;
}

