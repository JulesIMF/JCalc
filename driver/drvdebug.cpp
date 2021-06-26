/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    drvdebug.cpp

Abstract:
    
    Driver debug functions implementations.

Author / Creation date:

    JulesIMF / 27.06.2021

Revision History:

--*/


//
// Includes / usings
//

#include <drvdebug.h>
#include <colors.h>

//
// Defines
//

void dumpLexer(Lexed lexed)
{
    printf("%sLexer dump:\n%s", TerminalColor::BlueB, TerminalColor::Yellow);
    for(auto vertex : lexed.tokens)
    {
        if(vertex->type == VertexType::Linker)
        {
            printf("Linker\n");
        }

        if (vertex->type == VertexType::Operator)
        {
            printf("Operator %c\n", ((Operator*)vertex)->opType);
        }

        if (vertex->type == VertexType::Imm)
        {
            char const* type = nullptr;
            
            Imm* imm = (Imm*)vertex;
            switch(imm->immType)
            {
            case ImmType::Bin:
                type = "Bin";
                break;
            case ImmType::Char:
                type = "Char";
                break;
            case ImmType::Dec:
                type = "Dec";
                break;
            case ImmType::Float:
                type = "Float";
                break;
            case ImmType::Hex:
                type = "Hex";
                break;
            default:
                type = "?Type";
                break;
            }

            printf("Imm %s\t%lld\t%lf\n", type, imm->intValue, imm->floatValue);
        }

        if (vertex->type == VertexType::Delimiter)
        {
            printf("Delimiter %c\n", ((Delimiter *)vertex)->delimeter);
        }

        if (vertex->type == VertexType::Asgn)
        {
            printf("Asgn =%c\n", ( ((Asgn *)vertex)->asgnType == '=') ? ' ' : ( ((Asgn *)vertex)->asgnType) );
        }

        if (vertex->type == VertexType::Id)
        {
            printf("Id \"%s\"\n", ((Id*)vertex)->id.c_str());
        }
    }
    
    printf("%s", TerminalColor::Default);
}
