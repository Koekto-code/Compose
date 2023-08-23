#ifndef COMPOSE_COMPOSER_H
#define COMPOSE_COMPOSER_H

#include "Vector.h"
#include "Lexer.h"

// annotations
// <st> -- a slot for a statement
// <st:val> -- one that gives value
// <st:ref> -- one that gives reference
// <id> -- would be identified by Lexer as Ttp_Var

typedef enum
{
	Stt_Undefined = 0, // -- no value (reserved)
	Stt_Reference, // <id>; -- gives value
	Stt_LiteralEval, // <literal>; -- gives value
	Stt_Grouping, // (<st>, ...); -- gives value
	Stt_AssignOper, // <st:ref> [= += -= *= /= ...] <st:val>; -- gives value
	Stt_ValueOper, // <st:val> [+ - * / ...] <st:val>; -- gives value
	Stt_Sequence, // { <st>; ... } -- gives (may give) value
	Stt_ForLoopDecl, // for (<st>; <st:val>; <st>) <st> -- no value (@todo: give value as array if body returns smth)
	Stt_BranchDecl, // if (<st:val>) { <st>; ... } [else <st:branch>] -- no value (@todo: give value)
	Stt_FuncDecl, // function <id> (<id>, ...) <st> -- gives value
	Stt_FuncCall // <st:val> (<st:val>, ...); -- gives value
}
CmStatementType;

typedef struct
{
	CmStatementType type;
	void *data;
}
CmStatement;

typedef struct
{
	const char *beg;
	const char *end;
	size_t id; // ?
}
CmStatementRef;

typedef struct
{
	CmTokenData operType;
	CmStatement *first;
	CmStatement *second;
}
CmStatementBinaryOper;

typedef struct
{
	CmVector statements; // of CmStatement *
}
CmStatementSequence;

typedef struct
{
	CmStatement *init;
	CmStatement *cond;
	CmStatement *cont;

	CmStatement *body; // may be sequence
}
CmStatementForLoop;

typedef struct
{
	CmStatement *cond;
	CmStatement *elseBr;

	CmStatement *body;
}
CmStatementBranch;

typedef struct
{
	CmStatement *func;

	// grouping or reference statements
	CmStatement *args;
}
CmStatementFunction;

// ==========================================

typedef struct
{
	CmVector statements;
	CmVector toplevel;

	CmVector references;
	CmVector binOpers;
	CmVector sequences;
	CmVector forLoops;
	CmVector branches;
	CmVector functions;

	unsigned errors;
}
CmComposerData;

int CmIsAssignOper(CmTokenData data);

int CmIsValueOper(CmTokenData data);

int CmIsValueStatement(CmStatementType tp);

CmComposerData CmCompose(CmLexerData const *inp);

#endif
