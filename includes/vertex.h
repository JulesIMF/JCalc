/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    vertex.h

Abstract:

    Describes expression tree vertex (lexer tokenizes
    the expression directly into vertexes sequence)

Author / Creation date:

    JulesIMF / 27.06.2021

Revision History:

--*/

#ifndef JCALC_VERTEX
#define JCALC_VERTEX

//
// Includes / usings
//

#include <string>

//
// Defines
//

enum class VertexType
{
    None,
    Operator,
    Imm,
    Id,
    Delimiter,
    Linker,
    Asgn,
    Call,
};

struct Vertex
{
    VertexType type = VertexType::None;
    Vertex* left    = nullptr;
    Vertex* right   = nullptr;
    virtual ~Vertex();
};

struct Operator : public Vertex
{
    char opType;
    Operator(char opType_ = 0);
    virtual ~Operator();
};

struct Asgn : public Vertex
{
    char asgnType;
    Asgn(char asgnType_ = 0);
    virtual ~Asgn();
};

enum class ImmType
{
    None,
    Dec,
    Bin,
    Hex,
    Float,
    Char,
};

struct Imm : public Vertex
{
    ImmType immType;
    Imm(ImmType immType_ = ImmType::None);
    long long intValue = 0;
    double    floatValue = 0.0;
    virtual ~Imm();
};

struct Id : public Vertex
{
    std::string id;
    Id(std::string str = "");
    virtual ~Id();
};

struct Delimiter : public Vertex
{
    char delimeter;
    Delimiter(char delim = 0);
    virtual ~Delimiter();
};

struct Linker : public Vertex
{
    Linker();
    virtual ~Linker();
};

struct Call : public Vertex
{
    Call();
    virtual ~Call();
};


#endif // !JCALC_VERTEX