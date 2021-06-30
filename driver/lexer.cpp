/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    lexer.cpp

Abstract:

    Lexer cuts input into tokens.

Author / Creation date:

    JulesIMF / 27.06.2021

Revision History:

--*/

//
// Includes / usings
//

#include <vector>
#include <string>
#include <cctype>
using std::vector;
using std::string;

#include <vertex.h>
#include <err.h>
#include <messages.h>
#include <lexer.h>

//
// Defines
//


//
// Next functions return true, if they succeded in tokenization
// or return false otherwise
//

static bool trySpace(char const*& src, vector<Vertex*>& tokens)
{
    if (isspace(*src))
    {
        src++;  // ignore it
        return true;
    }

    return false;
}

static bool tryLinker(char const*& src, vector<Vertex*>& tokens)
{
    if(*src == ';')
    {
        src++;
        tokens.push_back(new Linker);
        return true;
    }

    return false;
}

static bool tryOperator(char const*& src, vector<Vertex *> &tokens)
{
    switch (*src)
    {
    case '+':
    case '-':
    case '*':
    case '/':
        tokens.push_back(new Operator(*src)); // setted this char as optype
        src++;                                // these operators described with a single character
        return true;

    default:
        break;
    }

    return false;
}

static bool tryAsgn(char const*& src, vector<Vertex *> &tokens)
{
    char asgnType = 0;

    if (src[1] == '=' || src[0] == '=')
    {
        asgnType = src[0];
        src += 1 + (src[1] == '='); // if src[1] == '=', then asgn takes
                                    // 2 signs, if no -- then one
    }

    else
        return false;

    tokens.push_back(new Asgn(asgnType));
    return true;
}

static bool tryDelimiter(char const*& src, vector<Vertex*>& tokens)
{
    switch(*src)
    {
    case '(':
    case ')':
        tokens.push_back(new Delimiter(*src)); // setted this char as delimiter
        src++;
        return true;

    default:
        break;
    }
    
    return false;
}

static bool tryId(char const*& src, vector<Vertex*> &tokens)
{
    if(!isalpha(*src))
        return false;

    string str;
    while(isalnum(*src))
        str += *(src++);
    
    tokens.push_back(new Id(str));
    return true;
}


/**
 * Transforms a digit into num. If base > 10, then letters become digits with no difference between small and capital.
 * \param digit digit to transform
 * \param base base in which digit is written
 * \return num is digit succesfully translated, -1 if there is no digit in this base
*/
static int digitToNum(char digit, long long base)
{
    if (base <= 0)
        return -1;
    

    // base \in [1; 10] - only digits
    if(base <= 10)
    {
        if ('0' <= digit && digit <= '0' + base)
            return digit - '0';
        else
            return -1;
    }

    // base \in [11; 36] - digits and chars
    if(base <= 36)
    {
        if ('0' <= digit && digit <= '0' + base)
            return digit - '0';

        if ('a' <= digit && digit <= 'a' + base - 10)
            return digit - 'a';

        if ('A' <= digit && digit <= 'A' + base - 10)
            return digit - 'A';

        return -1;
    }

    return -1;
}

/**
 * Tokenizes DEC | BIN | HEX | FLOAT
*/
static bool tryNum(char const*& src, vector<Vertex *> &tokens)
{
    if (!isdigit(*src) && *src != '.') // .012 == 0.012
        return false;
    
    long long base = 10; // dec by default
    long long intResult = 0;
    double floatResult = 0.0;
    ImmType type = ImmType::Dec;

    //
    // Trying to switch the base; 
    // as *src != '0', we have at least two chars defined - *src and next< possible '\0'
    //

    if (src[0] == '0' && src[1] == 'x')
        base = 16,
        type = ImmType::Hex;

    if (src[0] == '0' && src[1] == 'b')
        base = 2,
        type = ImmType::Bin;
    
    if (type != ImmType::Dec)
        src += 2; // if type != ImmType::Dec, then we should skip 0x or 0b

    // Starting to read the num literal

    int digit = 0;
    while ( (digit = digitToNum(*src, base)) != -1 ) // while we recognize digits
    {
        src++;
        intResult *= base;
        intResult += digit;
    }

    //
    // Trying to switch to float
    //

    if (*src == '.')
    {
        src++;
        floatResult = static_cast<double>(intResult);
        type = ImmType::Float;
        double multiplier = 1.0;

        while ( (digit = digitToNum(*src, base)) != -1 ) // while we recognize digits
        {
            src++;
            multiplier /= static_cast<double>(base);
            floatResult += digit * multiplier;
        }
    }

    auto imm = new Imm(type);
    if (type == ImmType::Float)
        imm->floatValue = floatResult;
    else
        imm->intValue = intResult;

    tokens.push_back(imm);
    return true;
}

static char escapeToChar(char escape)
{
    switch(escape)
    {
    case 'a':
        return '\a';
    case 'b':
        return '\b';
    case 'f':
        return '\f';
    case 'n':
        return '\n';
    case 'r':
        return '\r';
    case 't':
        return '\t';
    case 'v':
        return '\v';
    case '\'':
        return '\'';
    case '"':
        return '\"';
    case '\\':
        return '\\';
    case '?':
        return '\?';
    case '0':
        return '\0';

    default:
        return escape;
    }
}

static bool tryChar(char const*& src, vector<Vertex *> &tokens)
{
    if (*src != '\'')
        return false;

    char c = 0;
    src++;

    if (*src == '\0')
    {
        warningMessage("expected char after '");
        return false;
    }

    if (*src == '\\')
    {
        src++;
        if (*src == '\0')
        {
            warningMessage("expected char after \\");
            return false;
        }

        c = escapeToChar(*src);
        src++;
    }

    else
    {
        c = *(src++);
    }

    if (*src != '\'')
    {
        warningMessage("expected closing '");
        return false;
    }
    else
        src++;

    auto imm = new Imm(ImmType::Char);
    imm->intValue = c;
    tokens.push_back(imm);
    return true;
}

void Lexed::freeLexed()
{
    delete tokens;
}

Lexed lexer(char const *src)
{
    vector<Vertex *> *tokens = new vector<Vertex *>;

    while (*src)
    {
        if (trySpace(src, *tokens))
            continue;
        if (tryNum(src, *tokens))
            continue;
        if (tryLinker(src, *tokens))
            continue;
        if (tryId(src, *tokens))
            continue;
        if (tryDelimiter(src, *tokens))
            continue;
        if (tryChar(src, *tokens))
            continue;
        if (tryAsgn(src, *tokens)) // it`s nessesary to place it here
            continue;
        if (tryOperator(src, *tokens)) // cause tryOperator can recognize '+=' as '+'
            continue;

        return {tokens, LexerError::UNKNOWN_TOKEN}; // No suitable interpretation
    }

    return {tokens, LexerError::NO_ERROR};
}
