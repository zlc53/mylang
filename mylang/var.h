#pragma once

class Var
{
public:
	enum{
		None,
		Num,
		Str
	};

private:
	char type;
	union
	{
		double number;
		char* str;
	}value;
};

