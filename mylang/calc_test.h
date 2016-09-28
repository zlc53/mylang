#pragma once

#include <stack>
#include "token_parser.h"

inline int get_toke_priority(int toke)
{
	switch(toke)
	{
	case TOKEN_BRACKET_BEGIN:
		return 1;
	case TOKEN_BRACKET_END:
		return 1;
	case TOKEN_ADD:
	case TOKEN_SUB:
		return 8;
	case TOKEN_MUL:
	case TOKEN_DIV:
		return 9;
	}
	return 0;
}

inline bool iscommand(int toke)
{
	switch(toke)
	{
		case TOKEN_NUM:
		case TOKEN_STR:
			return false;
	}
	return true;

}

extern void calc_exec_op(int op, std::stack<int>& stack_value);
extern void calc_test();
extern void calc_test_vm();
