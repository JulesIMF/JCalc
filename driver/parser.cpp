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

#define IMM         static_cast<Imm*>(current)
#define OPERATOR    static_cast<Operator*>(current)
#define ID          static_cast<Id*>(current)
#define DELIMITER   static_cast<Delimiter*>(current)
#define ASGN        static_cast<Asgn*>(current)


void Parsed::deleteTree(Vertex* vertex)
{
    if (!vertex)
        return;
    
    deleteTree(vertex->left);
    deleteTree(vertex->right);
    delete vertex;
}

void Parsed::freeParsed(void)
{
    delete trash;
    deleteTree(ast);
}

Parser::Parser(Lexed lexed) : tokens(lexed.tokens),
                              trash(new std::vector<Vertex *>),
                              current(lexed.tokens->at(0)) {}

Parsed Parser::getLine(void)
{
    if (!notEnd) return {};

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

    if (notEnd)
        error = ParserError::UNEXPECTED_END;

    return { .ast           = line,
             .parserError   = error,
             .trash         = trash};
}

Vertex* Parser::getReq(void)
{
    if (!notEnd) return nullptr;
    /**
     * EXPR | ASGN | _epsilon
    */

    if (current->type == VertexType::Linker) // _epsilon
        return nullptr;
    

    auto req = getAsgn();   // ASGN
    if (error != ParserError::NO_ERROR) return nullptr;

    if (req)
        return req;
    
    req = getExpr();   // EXPR
    if (error != ParserError::NO_ERROR) return nullptr;

    if (req)
        return req;

    error = ParserError::EXPECTED_REQ;
    return nullptr;
}

Vertex* Parser::getExpr(void)
{
    if (!notEnd) return nullptr;

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
           (OPERATOR->opType == '+' || 
            OPERATOR->opType == '-'))
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
    if (!notEnd) return nullptr;

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
           (OPERATOR->opType == '*' || 
            OPERATOR->opType == '/'))
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

Vertex* Parser::getPrim(void)
{
    if (!notEnd) return nullptr;

    /**
     * CALL | '-' PRIM | '(' EXPR ')' | IMM | ID
    */

   // ---------------------------------------------

    /**
     * CALL
    */

    auto prim = getCall();

    if (error != ParserError::NO_ERROR)
        return nullptr;

    if (prim)
        return prim;


    /**
     * '-' PRIM
    */
    if (current->type == VertexType::Operator &&
        OPERATOR->opType == '-')
    {
        prim = current;
        inc();

        prim->left = new Imm(ImmType::Dec); // Imm is created with both values == 0;
                                            // "-expr" == "0 - expr", so we are to
                                            // represent it in this form
        
        auto rightPrim = getPrim();
        if (error != ParserError::NO_ERROR)
            return nullptr;
        
        if (!rightPrim)
        {
            error = ParserError::EXPECTED_PRIM;
            return nullptr;
        }

        prim->right = rightPrim;
        return prim;
    }

    /**
     * '(' EXPR ')'
    */

    if (current->type == VertexType::Delimiter &&
        DELIMITER->delimeter == '(')
    {
        trash->push_back(current); // this bracket won`t be in the ast
        inc();
        prim = getExpr();
        if (error != ParserError::NO_ERROR)
            return nullptr;

        if (!notEnd                                ||
            current->type != VertexType::Delimiter ||
            DELIMITER->delimeter != ')')
        {
            error = ParserError::EXPECTED_BRACKET;
            return nullptr;
        }

        trash->push_back(current); // this bracket won`t be in the ast
        inc();

        return prim;
    }

    /**
     * IMM | ID
    */
    if (current->type == VertexType::Imm ||
        current->type == VertexType::Id)
    {
        prim = current;
        inc();
        return prim;
    }

    // No more possible variants - there is
    // no prim you`re looking for

    return nullptr;
}

Vertex* Parser::getCall(void)
{
    if (!notEnd) return nullptr;

    /**
     * ID '(' EXPR ')'
    */

    if (index + 2 > tokens->size())
        return nullptr;

    Vertex* openBracket = atShiftOf(1); // At least we expect to see open bracket here
    
    if (!
        (
           current->type == VertexType::Id             &&
           openBracket->type == VertexType::Delimiter  &&
           static_cast<Delimiter*>(openBracket)->delimeter == '('
        ))
    {
        return nullptr;
    }

    auto id = current;
    inc(2);

    trash->push_back(openBracket); // no more need the bracket

    Vertex* expr = nullptr;

    // Here we look if we haven`t reached the end,
    // and if haven`t, then try to get expr
    if (!notEnd || 
        (expr = getExpr(), expr == nullptr))
    {
        // In case if error was setted in getExpr()
        if (error == ParserError::NO_ERROR)
            error = ParserError::EXPECTED_EXPR;

        return nullptr;
    }

    if (!notEnd                                 ||
        current->type != VertexType::Delimiter  ||
        DELIMITER->delimeter != ')')
    {
        error = ParserError::EXPECTED_BRACKET;
        return nullptr;
    }
    
    trash->push_back(current);
    inc();    

    auto call = new Call();
    call->left = id;
    call->right = expr;
    
    return call;
}

Vertex* Parser::getAsgn(void)
{
    // ID ['=' '+=' '-=' '*=' '/='] EXPR

    if (index + 2 > tokens->size())
        return nullptr;

    Vertex* asgn = atShiftOf(1); // At least we expect to see open bracket here
    
    if (!
        (
           current->type == VertexType::Id &&
           asgn->type == VertexType::Asgn
        ))
    {
        return nullptr;
    }

    auto id = current;
    inc(2);

    auto expr = getExpr();
    if (error != ParserError::NO_ERROR)
        return nullptr;

    if (expr == nullptr)
    {
        error = ParserError::EXPECTED_EXPR;
        return nullptr;
    }

    asgn->left = id;
    asgn->right = expr;

    return asgn;
}

void Parser::inc(int shift)
{
    index += shift;
    notEnd = index < tokens->size();
    current = notEnd ? tokens->at(index) : nullptr;
}

Vertex* Parser::atShiftOf(int shift)
{
    return (index + shift < tokens->size()) ? tokens->at(index + shift) : nullptr;
}

char const* parserErrorToStr(ParserError error)
{
    switch (error)
    {
    case ParserError::EXPECTED_BRACKET:
        return "expected bracket";

    case ParserError::EXPECTED_EXPR:
        return "expected expression";
    
    case ParserError::EXPECTED_PRIM:
        return "expected primary expression";
    
    case ParserError::EXPECTED_REQ:
        return "expected request";
    
    case ParserError::EXPECTED_TERM:
        return "expected term";   

    default:
        return "unknown error";
    }
}