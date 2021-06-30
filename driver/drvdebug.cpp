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

#include <cassert>
#include <drvdebug.h>
#include <colors.h>

//
// Defines
//

void dumpLexer(Lexed lexed)
{
    printf("%sLexer dump:\n%s", TerminalColor::Blue, TerminalColor::Yellow);
    for(auto vertex : *lexed.tokens)
    {
        if(vertex->type == VertexType::Linker)
        {
            printf("Linker\n");
        }

        if (vertex->type == VertexType::Operator)
        {
            printf("Op\t%c\n", ((Operator*)vertex)->opType);
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
                type = "?VertexType";
                break;
            }

            printf("Imm\t%s\t%lld\t%lf\n", type, imm->intValue, imm->floatValue);
        }

        if (vertex->type == VertexType::Delimiter)
        {
            printf("Del\t%c\n", ((Delimiter *)vertex)->delimeter);
        }

        if (vertex->type == VertexType::Asgn)
        {
            printf("Asgn\t=%c\n", ( ((Asgn *)vertex)->asgnType == '=') ? ' ' : ( ((Asgn *)vertex)->asgnType) );
        }

        if (vertex->type == VertexType::Id)
        {
            printf("Id\t\"%s\"\n", ((Id*)vertex)->id.c_str());
        }
    }
    
    printf("%s", TerminalColor::Default);
}

void putVertex(FILE* file, Vertex* vertex)
{
    static int visited = 0;
    //printf("visit = %lu\n", visited++);
    
    switch (vertex->type)
    {
    case VertexType::Id:
        fprintf(file, "v%p[label = \"Id\\n\\\"%s\\\"\", shape = \"record\", style = \"bold\", fontsize = 15]; ",
            vertex, ((Id*)vertex)->id.c_str()); break;

    case VertexType::Operator:
        fprintf(file, "v%p[label = \"Operator\\nopType = '%c'\", shape = \"octagon\", style = \"filled, bold\", fillcolor = \"yellow\", fontsize = 15];",
            vertex, ((Operator*)vertex)->opType); break;

    case VertexType::Delimiter:
        fprintf(file, "v%p[label = \"Delimiter\\ndelimiter = '%c'\", shape = \"parallelogram\", fillcolor = \"orange\", style = \"filled, bold\", fontsize = 15];",
                vertex, ((Delimiter *)vertex)->delimeter);
        break;

    case VertexType::Call:
        fprintf(file, "v%p[label = \"Call\", shape = \"octagon\", style = \"filled, bold\", fillcolor = \"yellow\", fontsize = 15];",
            vertex); break;

    case VertexType::Imm:
    {
        char const *type = nullptr;

        Imm *imm = (Imm *)vertex;
        switch (imm->immType)
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
            type = "?VertexType";
            break;
        }

        fprintf(file, "v%p[label = \"Imm\\nint = %lu, float = %lf, %s\", shape = \"record\", style = \"bold\", fontsize = 15];",
                vertex, imm->intValue, imm->floatValue, type);
    }
        break;

    case VertexType::Linker:
        fprintf(file, "v%p[label = \"Linker\", shape = \"parallelogram\", fillcolor = \"orange\", style = \"filled, bold\", fontsize = 15];",
            vertex); break;

    case VertexType::Asgn:
        fprintf(file, "v%p[label = \"Asgn\\nasgnType = '%c'\", shape = \"diamond\", style = \"filled, bold\", fillcolor = \"darkorchid1\", fontsize = 15];",
            vertex, ((Asgn*)vertex)->asgnType); break;

    default:
        printf("Unknown type");
    }
    fprintf(file, "\n");
    if (vertex->left)
    {
        fprintf(file, "v%p->v%p [color = \"red\"]\n", vertex, vertex->left);
        putVertex(file, vertex->left);
    }

    if (vertex->right)
    {
        fprintf(file, "v%p->v%p [color = \"blue\"]\n", vertex, vertex->right);
        putVertex(file, vertex->right);
    }
}

void dumpParser(Parsed parsed)
{
    FILE *file = fopen("dbg/astsrc/ast.dot", "w");
    assert(file);
    fprintf(file, "digraph\n{\ndpi = 300;\n");
    putVertex(file, parsed.ast);
    fprintf(file, "}");
    fclose(file);
    system("dot -Tpng dbg/astsrc/ast.dot -o ast.png > /dev/null");

    // Следующий трюк работает только под Шиндовс
    //system("start ast.png");
}
