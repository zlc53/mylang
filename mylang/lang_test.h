#pragma once

//一个只有全局变量的简单语言。

#include <stack>
#include <map>
#include "token_parser.h"

struct Var
{
	union
	{
		double number;
		char* s;
	};
};

class MyLang
{
public:
	void Run(const char* str, int len);
	void ExcOP(int op);
private:
	std::stack<int> m_stack_op;
	std::stack<Var> m_stack_values;
};