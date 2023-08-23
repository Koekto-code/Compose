#ifndef COMPOSE_LEXER_H
#define COMPOSE_LEXER_H

#define MODULAR_OPERATORS_NUM 14
#define MODULAR_CONTROLS_NUM 12

#include "Vector.h"

#include <stdint.h>
#include <stddef.h>


typedef enum CmTokenType
{
	Ttp_Undefined = 0x0,
	Ttp_Oper      = 0x1,
	Ttp_Control   = 0x2,
	Ttp_Var       = 0x4,
	Ttp_Literal   = 0x8
}
CmTokenType;

typedef enum
{
	Tdt_Undefined = 0,
	
	Tdt_Oper_Equals,
	Tdt_Oper_IsEqual,
	Tdt_Oper_IsNEqual,
	Tdt_Oper_IsGreater,
	Tdt_Oper_IsGreaterEqual,
	Tdt_Oper_IsLess,
	Tdt_Oper_IsLessEqual,
	Tdt_Oper_Add,
	Tdt_Oper_Sub,
	Tdt_Oper_Mul,
	Tdt_Oper_Div,
	Tdt_Oper_Dot,
	Tdt_Oper_Colon,
	Tdt_Oper_Comma,
	
	Tdt_Control_Return,
	Tdt_Control_Function,
	Tdt_Control_ScopeBeg,
	Tdt_Control_ScopeEnd,
	Tdt_Control_ParBeg,
	Tdt_Control_ParEnd,
	Tdt_Control_If,
	Tdt_Control_Else,
	Tdt_Control_Break,
	Tdt_Control_Continue,
	Tdt_Control_For,
	Tdt_Control_Semicolon,
	
	// Tdt_Var_Value,
	// Tdt_Var_Reference,
	
	Tdt_Literal_String,
	Tdt_Literal_Int,
	Tdt_Literal_Float,
}
CmTokenData;

typedef struct
{
	CmTokenType type;
	CmTokenData data;

	unsigned line;
	unsigned symbol;

	const char *beg;
	const char *end;
}
CmToken;

typedef struct
{
	CmVector tokens;
	int success;
	int syntaxErrors;
}
CmLexerData;

// @deprecated
// CmToken CmRecognizeToken(const char *beg, const char *end);

const char *CmOperString(CmTokenData data);

const char *CmControlString(CmTokenData data);

CmLexerData CmLexerProcessFile(const char *filename);

#endif
