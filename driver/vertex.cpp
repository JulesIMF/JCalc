/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    vertex.h

Abstract:

    Defines expression tree vertex (lexer tokenizes
    the expression directly into vertexes sequence)

Author / Creation date:

    JulesIMF / 27.06.2021

Revision History:

--*/

//
// Includes / usings
//

#include <vertex.h>

//
// Defines
//


Operator::Operator(char opType_)
{
    type = VertexType::Operator;
    opType = opType_;
}

Asgn::Asgn(char asgnType_)
{
    type = VertexType::Asgn;
    asgnType = asgnType_;
}

Imm::Imm(ImmType immType_)
{
    type = VertexType::Imm;
    immType = immType_;
}

Id::Id(std::string str)
{
    type = VertexType::Id;
    id = str;
}

Delimiter::Delimiter(char delim)
{
    type = VertexType::Delimiter;
    delimeter = delim;
}

Linker::Linker()
{
    type = VertexType::Linker;
}

Call::Call()
{
    type = VertexType::Call;
}


Vertex::~Vertex()       = default;

Operator::~Operator()   = default;

Asgn::~Asgn()           = default;

Imm::~Imm()             = default;

Id::~Id()               = default;

Delimiter::~Delimiter() = default;

Linker::~Linker()       = default;

Call::~Call()           = default;
