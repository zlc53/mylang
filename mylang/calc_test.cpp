#include <assert.h>
#include <vector>
#include "calc_test.h"

enum {
	VM_CMD_PUSH = 1,
	VM_CMD_POP,
	VM_CMD_TOP,
	VM_VALUE_PUSH,
	VM_VALUE_POP,
	VM_VALUE_TOP,
	VM_ADD,
	VM_SUB,
	VM_MUL,
	VM_DIV,
};

class vm_buff
{
	inline void alloc(int size)
	{
		if(m_buf)
		{
			free(m_buf);
			m_buf = (char*)realloc(m_buf, size);
		}
		else
		{
			m_buf = (char*)malloc(size);
		}
		m_buf_len = size;
	}
public:
	vm_buff()
	{
		m_buf = 0;
		m_cur = 0;
		m_len = 0;
		alloc(4096);
	}

	void push_char(char cmd)
	{
		m_buf[m_cur] = cmd;
		m_cur++;
		m_len++;
	}

	void push_int(int value)
	{
		memcpy(m_buf+m_cur, &value, sizeof(int));
		m_cur+= sizeof(int);
		m_len += sizeof(int);
	}

	void seek(int i)
	{
		m_cur = i;
	}

	char read_char()
	{
		char i = (char)(*(m_buf + m_cur));
		m_cur += sizeof(char);
		return i;
	}

	int read_int()
	{
		int i = (int)(*(m_buf + m_cur));
		m_cur += sizeof(int);
		return i;
	}

	int GetPos() { return m_cur; };

	int GetLen() { return m_len; };
private:
	int m_cur;
	int m_len;
	int m_buf_len;
	char* m_buf;
};

vm_buff g_buf;

inline void trace(const char* str)
{
	printf("%s", str);
	_asm int 3;
}

inline const char* OpAsStr(int op)
{
	switch(op)
	{
	case TOKEN_EQUAL:
		return "=";
	case TOKEN_LINEEND:
		return "lineend";
	case TOKEN_BRACKET_BEGIN:
		return "(";
	case TOKEN_BRACKET_END:
		return ")";
	case TOKEN_ADD:
		return "+";
	case TOKEN_SUB:
		return "-";
	case TOKEN_MUL:
		return "*";
	case TOKEN_DIV:
		return "/";
	case TOKEN_IDENT:
		return "ident";
	}
	return "unknow";
}

struct op_s
{
	int op;
	int i;
};

struct var_s
{
	int value;
	int i;
};

int g_err_op;

inline int process_op(op_s& topop, std::stack<var_s>& stack_value)
{
	int op = topop.op;
	int i = topop.i;
	if(stack_value.size() < 2)
		return 1;
	var_s v1 = stack_value.top();
	stack_value.pop();
	var_s v2 = stack_value.top();
	stack_value.pop();

	if(!((topop.i - v2.i) == 1 && (v1.i - topop.i) == 1))
	{
		return 1;
	}
	var_s v;
	switch(op)
	{
	case TOKEN_ADD:
		v.value = v2.value + v1.value;
		g_buf.push_char(VM_ADD);
		break;
	case TOKEN_SUB:
		v.value = v2.value - v1.value;
		g_buf.push_char(VM_SUB);
		break;
	case TOKEN_MUL:
		v.value = v2.value * v1.value;
		g_buf.push_char(VM_MUL);
		break;
	case TOKEN_DIV:
		v.value = v2.value / v1.value;
		g_buf.push_char(VM_DIV);
		break;
	}

	v.i = v2.i;
	stack_value.push(v);

	//g_buf.push_char(VM_VALUE_PUSH);
	//g_buf.push_int(v.value);

	printf("reduce opt:%s, order:%d, var1:%d, order:%d, var2:%d, order:%d\n", OpAsStr(op), topop.i,  v1.value, v1.i, v2.value, v2.i);
	printf("shift value:%d, order:%d\n", v.value, v.i);
	return 0;
}


int reduce_op(std::stack<op_s>& stack_op, std::stack<var_s>& stack_value, TokenParser& parser, int & i)
{
	int op = stack_op.top().op;
	//int i = stack_op.top().i;
	int ret = 0;
	switch(op)
	{
	case TOKEN_SUB:
	case TOKEN_ADD:
	case TOKEN_MUL:
	case TOKEN_DIV:
		{
			ret = process_op(stack_op.top(), stack_value);
		//	stack_op.pop();
		}break;
	case TOKEN_EQUAL:
		{
			int n = 3;
		}break;
	case TOKEN_IDENT:
		break;
	case TOKEN_BRACKET_END:
		break;
	default:
		assert(0);
		break;
	}
	
	if(ret != 0)
	{
		printf("reduce[%s] error in %d bytes\n", OpAsStr(stack_op.top().op), parser.GetCurPos());
	}
	i = stack_op.top().i;
	stack_op.pop();
	return ret;
	
}


void calc_test()
{
	std::stack<op_s> stack_op;
	std::stack<var_s> stack_value;

	 //Priority
	const char* test_data = "2*(3)-4*8+2;";
	//const char* test_data = "3+4+5;";
	//const char* test_data = "as=2*5+3-4*8+2;";
	printf("input str: %s\n", test_data);
	TokenParser parser;
	int token = parser.Parse(test_data, strlen(test_data));

	int i = 0;
	while(token != TOKEN_EOF)
	{
		if(iscommand(token))
		{
			while(true)
			{
				int new_prio = get_toke_priority(token);
				int top_prio = -1;
				op_s curop;
				curop.i = i;
				curop.op = token;

				if(!stack_op.empty())
					top_prio = get_toke_priority(stack_op.top().op);

				//ÒÆ½ø
				if(token == TOKEN_BRACKET_BEGIN || new_prio > top_prio)
				{
					printf("shift op:%s, order: %d\n", OpAsStr(curop.op), curop.i);
					stack_op.push(curop);
					break;
				}
				else if(token == TOKEN_BRACKET_END)
				{
					bool find = false;
					while(!stack_op.empty())
					{
						if(stack_op.top().op == TOKEN_BRACKET_BEGIN)
						{
							i = stack_op.top().i;
							//error
							find = true;
							stack_value.top().i = i;
							stack_op.pop();
							break;
						}
						if(0!= reduce_op(stack_op, stack_value, parser, i))
							goto end;
					}

					if(!find)
					{
						trace("no find (");
					}
					break;
				}
				else
				{
					if(0!= reduce_op(stack_op, stack_value, parser, i))
						goto end;
				}
			}
		}
		else
		{
			char buf[20];
			parser.GetTokenValue(buf, 20);
			var_s v;
			v.value = atoi(buf);
			v.i = i;
			printf("shift value %d, order:%d\n", v.value, v.i);
			stack_value.push(v);
			g_buf.push_char(VM_VALUE_PUSH);
			g_buf.push_int(v.value);
		}
		token = parser.Parse();
		i++;
	}
end:;

	calc_test_vm();
}



extern void calc_test_vm()
{
	std::stack<int> stack_op;
	std::stack<int> stack_value;
	int ax, bx, cx, dx;
	int cur = 0;

	g_buf.seek(0);
	
	while(g_buf.GetPos() < g_buf.GetLen())
	{
		int cmd = g_buf.read_char();
		switch(cmd)
		{
		case VM_CMD_PUSH:
			{
				stack_op.push(cmd);
			}
			break;
		case VM_CMD_POP:
			{
				stack_op.pop();
			}
			break;
		case VM_VALUE_PUSH:
			{
				stack_value.push(g_buf.read_int());
			}
			break;
		case VM_VALUE_POP:
			{
				stack_value.pop();
			}break;
		case VM_ADD:
			{
				int v1 = stack_value.top();
				stack_value.pop();
				int v2 = stack_value.top();
				stack_value.pop();
				stack_value.push(v2+v1);
			}break;
		case VM_SUB:
			{
				int v1 = stack_value.top();
				stack_value.pop();
				int v2 = stack_value.top();
				stack_value.pop();
				stack_value.push(v2-v1);
			}
			break;
		case VM_MUL:
			{
				int v1 = stack_value.top();
				stack_value.pop();
				int v2 = stack_value.top();
				stack_value.pop();
				stack_value.push(v2*v1);
			}
			break;
		case VM_DIV:
			{
				int v1 = stack_value.top();
				stack_value.pop();
				int v2 = stack_value.top();
				stack_value.pop();
				stack_value.push(v2/v1);
			}
			break;
		}
		cur++;
	}

	int c = g_buf.read_char();
	int n = 3;
}

