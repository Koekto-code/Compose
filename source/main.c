#include "../include/Modular/Lexer.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc < 2)
		return 1;
	
	CmLexerData data = CmLexerProcessFile(argv[1]);
	if (!data.success) {
		printf("Failed to process %s\n", argv[1]);
		return 1;
	}
	printf("Syntax errors: %d\n", data.syntaxErrors);
	
	CmToken *tokens = (CmToken *)data.tokens.data;
	for (int i = 0; i < data.tokens.length; ++i)
	{
		printf("token type: %d, ", (int)tokens[i].type);
		if (tokens[i].type == Ttp_Oper)
			printf("data: %s\n", CmOperString(tokens[i].data));
		else if (tokens[i].type == Ttp_Control)
			printf("data: %s\n", CmControlString(tokens[i].data));
		else printf("data: %d\n", (int)tokens[i].data);
	}
	CmVectorDestroy(&data.tokens);
}
