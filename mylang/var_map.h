#pragma once
#include <map>
#include <string.h>
#include "var.h"

class VarMap
{
public:
	void SetValue(const char* name, const Var& value)
	{
//		m_values[name] = value;
	}

	const Var* GetValue(const char* name)
	{
	//	std::map<std::string, Var>::iterator it = m_values.find(name);
		//if(it == m_values.end())
	//		return NULL;
	//	return &it->second;
	}
private:
//	std::map<std::string, Var> m_values;
};