#include "../../include/Compose/Composer.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void printToken(CmToken const *tk)
{
	for (const char *s = tk->beg; s != tk->end; ++s)
		putchar(*s);
}

int CmIsAssignOper(CmTokenData data)
{
	switch (data)
	{
		case Tdt_Oper_Equals:
			return 1;
		default:
			return 0;
	}
}

int CmIsValueOper(CmTokenData data)
{
	switch (data)
	{
		case Tdt_Oper_IsEqual:
		case Tdt_Oper_IsNEqual:
		case Tdt_Oper_IsGreater:
		case Tdt_Oper_IsGreaterEqual:
		case Tdt_Oper_IsLess:
		case Tdt_Oper_IsLessEqual:
		case Tdt_Oper_Add:
		case Tdt_Oper_Sub:
		case Tdt_Oper_Mul:
		case Tdt_Oper_Div:
		case Tdt_Oper_Dot:
		case Tdt_Oper_Colon:
			return 1;
		default:
			return 0;
	}
}

int CmIsValueStatement(CmStatementType tp)
{
	switch (tp)
	{
		case Stt_Reference:
		case Stt_LiteralEval:
		case Stt_Grouping:
		case Stt_AssignOper:
		case Stt_ValueOper:
		case Stt_Sequence:
		case Stt_FuncDecl:
		case Stt_FuncCall:
			return 1;
		default:
			return 0;
	}
}

static size_t processStatement(CmLexerData const *inp, CmComposerData *outp, size_t beg, size_t len, CmStatement **stat);

static size_t processSingleStatement(CmLexerData const *inp, CmComposerData *outp, size_t beg, CmStatement **stat)
{
	CmToken const *data = inp->tokens.data;
	size_t const tklen = inp->tokens.length - beg;
	CmVector *statv = &outp->statements;

	if (!tklen)
	{
		if (stat) *stat = NULL;
		return 0;
	}

	if (data[beg].data == Tdt_Control_Semicolon)
	{
		if (stat) *stat = NULL;
		return beg + 1;
	}

	if (data[beg].type == Ttp_Var)
	{
		CmVector *statvref = &outp->references;
		CmVectorResize(statvref, statvref->length + 1);
		CmStatementRef *statref = CmVectorBack(statvref);

		CmVectorResize(statv, statv->length + 1);
		CmStatement *newst = CmVectorBack(statv);
		newst->data = statref;

		statref->beg = data[beg].beg;
		statref->end = data[beg].end;
		statref->id = 0;

		newst->type = Stt_Reference;

		if (stat) *stat = newst;
		return beg + 1;
	}

	// parentheses grouping logic
	if (data[beg].data == Tdt_Control_ParBeg)
	{
		// the depth level of parentheses
		int depth = 1;

		// commas on current depth level
		int commas = 0;

		// statements that are separated with commas
		CmVector innerStatv = CmVectorCreate(sizeof(CmStatement *));

		size_t parIter;
		size_t curBeg = beg + 1;
		for (parIter = beg + 1; parIter != beg + tklen; ++parIter)
		{
			if (data[parIter].data == Tdt_Control_ParEnd)
				--depth;
			else if (data[parIter].data == Tdt_Control_ParBeg)
				++depth;

			if (!depth || (data[parIter].data == Tdt_Oper_Comma && depth == 1))
			{
				CmStatement *newst;
				(void)processStatement(inp, outp, curBeg, parIter - curBeg, &newst);

				if (newst)
					CmVectorPushBack(&innerStatv, &newst);
				// otherwise it could be (somevar, )

				curBeg = parIter + 1;
			}

			if (!depth)
				break;

			if (data[parIter].data == Tdt_Oper_Comma && depth == 1)
				++commas;
		}
		if (parIter == beg + tklen)
		{
			++outp->errors;
			printf("%zu: error: missing closing bracket\n", parIter);
			CmVectorDestroy(&innerStatv);
			return parIter;
		}

		if (!commas)
		{
			CmStatement **single = CmVectorBack(&innerStatv);
			if (stat) *stat = *single;
			CmVectorDestroy(&innerStatv);
		}
		else
		{
			CmVector *statvseq = &outp->sequences;
			CmVectorResize(statvseq, statvseq->length + 1);
			CmStatementSequence *statseq = CmVectorBack(statvseq);
			statseq->statements = innerStatv;

			CmVectorResize(statv, statv->length + 1);
			CmStatement *newst = CmVectorBack(statv);
			newst->type = Stt_Grouping;
			newst->data = statseq;
			if (stat) *stat = newst;
		}
		return parIter;
	}

	++outp->errors;
	printf("%zu: error: unexpected token\n", beg);
	printToken(&data[beg]);
	printf("'\n");

	if (stat) *stat = NULL;
	return beg + 1;
}

static size_t processStatement(CmLexerData const *inp, CmComposerData *outp, size_t beg, size_t len, CmStatement **stat)
{
	CmToken const *data = inp->tokens.data;
	size_t const tklen = len ? len : inp->tokens.length - beg;

	CmStatement *first;
	size_t firstend = processSingleStatement(inp, outp, beg, &first);

	if (!first || firstend == beg + tklen)
	{
		if (stat) *stat = first;
		return firstend;
	}

	if (data[firstend].type == Ttp_Oper)
	{
		CmStatement *sec;
		size_t secend = processStatement(inp, outp, firstend, beg + tklen - firstend, &sec);
		
		if (!sec || !CmIsValueStatement(sec->type))
		{
			printf("%zu: error: value statement expected\n", firstend);
			return secend;
		}
		
		CmVectorResizeM(outp->binOpers, + 1);
		CmStatementBinaryOper *opst = CmVectorBack(&outp->binOpers);
		opst->first = first;
		opst->second = sec;
		opst->type = data[firstend].data;
		
		CmVectorResizeM(outp->statements, + 1);
		CmStatement *newst = CmVectorBack(&outp->statements);
		newst->data = opst;
		newst->type = CmIsAssignOper(opst->type) ? Stt_AssignOper : Stt_ValueOper;
		assert(newst->type == Stt_AssignOper || CmIsValueOper(opst->type));
		
		return secend;
	}
	
	CmStatement *second;
	size_t secondend = processSingleStatement(inp, outp, firstend, &second);
	if (!second)
	{
		if (stat) *stat = second;
		return secondend;
	}
	
	if (second->type & (Stt_Grouping | Stt_Reference))
	{
		if (!CmIsValueStatement(first->type))
		{
			printf("%zu: error: value statement expected\n", beg);
			return secondend;
		}
			
		CmVectorResizeM(outp->functions, + 1);
		CmStatementFunction *funcst = CmVectorBack(&outp->functions);
		funcst->func = first;
		funcst->args = second;
		
		CmVectorResizeM(outp->statements, + 1);
		CmStatement *newst = CmVectorBack(&outp->statements);
		newst->type = Stt_FuncCall;
		newst->data = funcst;
		
		return secondend;
	}

	// @todo clear last 2 statements
	if (stat) *stat = NULL;
	return secondend;
}

CmComposerData CmCompose(CmLexerData const *inp)
{
	assert(inp->success && !inp->syntaxErrors);

	CmComposerData outp;
	outp.statements = CmVectorCreate(sizeof(CmStatement));
	outp.toplevel = CmVectorCreate(sizeof(CmStatement *));

	outp.references = CmVectorCreate(sizeof(CmStatementRef));
	outp.sequences = CmVectorCreate(sizeof(CmStatementSequence));
	outp.binOpers = CmVectorCreate(sizeof(CmStatementBinaryOper));
	outp.forLoops = CmVectorCreate(sizeof(CmStatementForLoop));
	outp.branches = CmVectorCreate(sizeof(CmStatementBranch));
	outp.functions = CmVectorCreate(sizeof(CmStatementFunction));

	outp.errors = 0;

	size_t stbeg = 0;
	while (stbeg != inp->tokens.length)
	{
		CmStatement *st;
		stbeg = processStatement(inp, &outp, stbeg, 0, &st);
		if (st)
			CmVectorPushBack(&outp.toplevel, &st);
		else ++outp.errors;
	}
	return outp;
}
