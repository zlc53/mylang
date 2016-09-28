#pragma once

enum TOKEN
{
	TOKEN_EOF = 1,
	
	TOKEN_ADD,
	TOKEN_SUB,
	TOKEN_MUL,
	TOKEN_DIV,
	TOKEN_EQUAL,
	TOKEN_LINEEND,
	TOKEN_IDENT,
	TOKEN_STR,
	TOKEN_NUM,
	TOKEN_BRACKET_BEGIN,
	TOKEN_BRACKET_END,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_WHILE,
};

class TokenParser
{
public:
	TokenParser()
	{
		m_cur = 0;
	}

	int Parse(const char* str, int len)
	{
		m_str = str;
		m_len = len;
		return Parse();
	}

	int Parse()
	{
		while(m_str[m_cur] < 33)
		{
			m_cur++;
		}
		
		if(m_cur >= m_len)
			return TOKEN_EOF;

		switch(m_str[m_cur])
		{
		case '+':
			m_cur++;
			return TOKEN_ADD;
		case '-':
			m_cur++;
			return TOKEN_SUB;
		case '*':
			m_cur++;
			return TOKEN_MUL;
		case '/':
			m_cur++;
			return TOKEN_DIV;
		case '"':
			m_cur++;
			return TOKEN_STR;
		case '(':
			m_cur++;
			return TOKEN_BRACKET_BEGIN;
		case ')':
			m_cur++;
			return TOKEN_BRACKET_END;
		case '=':
			m_cur++;
			return TOKEN_EQUAL;
		case ';':
			{
				m_cur++;
				return TOKEN_LINEEND;
			}break;
		default:
			if(ident_begin(m_str[m_cur]))
			{
				//±êÊ¶·û
				int ident_start = m_cur;
				int ident_end = m_cur+1;
				while(true)
				{
					if(ident_begin(m_str[ident_end]) || num_begin(m_str[ident_end]))
					{
						ident_end++;
					}
					else
					{
						break;
					}
				}
				m_cur = ident_end;

				if(strncmp(m_str+ident_start, "if", ident_end - ident_start) == 0)
					return TOKEN_IF;
				else if(strncmp(m_str+ident_start, "else", ident_end - ident_start) == 0)
					return TOKEN_ELSE;

				m_toke_arg1 = ident_start;
				m_toke_arg2 = ident_end;
				
				return TOKEN_IDENT;
			}
			else if(num_begin(m_str[m_cur]))
			{
				int num_start = m_cur;
				int num_end = m_cur+1;
				int decimal = 0;
				while(true)
				{
					if(num_begin(m_str[num_end]))
					{
						num_end++;
					}
					else if(m_str[num_end] == '.' && decimal == 0)
					{
						decimal ++;
						num_end++;
					}
					else
					{
						break;
					}
				}
				m_cur = num_end;
				m_toke_arg1 = num_start;
				m_toke_arg2 = num_end;
				return TOKEN_NUM;
			}
			else
			{
				_asm int 3;
			}
			break;
		}
		return 0;
	}

	inline void GetTokenValue(char* buf, int len)
	{
		strncpy(buf, m_str + m_toke_arg1, m_toke_arg2 - m_toke_arg1);
		buf[m_toke_arg2 - m_toke_arg1] = 0;
	}

	int GetCurPos()const {return m_cur; }
private:

	inline bool ident_begin(char ch)
	{
		return (ch>='a' && ch<='z') ||  (ch>='A' && ch<='Z');
	}

	inline bool num_begin(char ch)
	{
		return ch>='0' && ch<='9';
	}

	const char* m_str;
	int m_len;
	int m_cur;
	int m_toke_arg1, m_toke_arg2;
};