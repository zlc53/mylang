#include <string.h>
#include "token_parser.h"
#include "token_table.h"

token_s token_tbl[] = {

	{TOKEN_EOF, "eof"},
	{TOKEN_ADD, "add"},
	{TOKEN_SUB, "sub"},
	{TOKEN_MUL, "mul"},
	{TOKEN_DIV, "div"},
	{TOKEN_EQUAL, "equal"},
	{TOKEN_LINEEND, "lineend"},
	{TOKEN_IDENT, "ident"},
	{TOKEN_STR, "str"},
	{TOKEN_NUM, "num"},
	{TOKEN_BRACKET_BEGIN, "bracket_begin"},
	{TOKEN_BRACKET_END, "bracket_end"},
	{TOKEN_IF, "if"},
	{TOKEN_ELSE, "else"},
	{TOKEN_WHILE, "while"},
};