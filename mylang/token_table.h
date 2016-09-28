#pragma once

struct token_s
{
	int prio;
	const char* name;
};

extern token_s token_tbl[];