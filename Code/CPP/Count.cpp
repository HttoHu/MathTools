#include "../Count.h"
#include "../BalanceStack.h"
#include <stdlib.h>
using namespace Htto::Count;
void string_replace(std::string&s1, const std::string&s2, const std::string&s3)
{
	using std::string;
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
}
float StringToFloat(std::string str)
{
	float i;
	i = atof(str.c_str());
	return i;
}
unit Push_unit(int & index, std::string input)
{
	unit ret;
	if (index > input.size())
	{
		throw std::out_of_range("out of index");
	}

	if (Htto::StringTools::isSingleNumber(input[index]))
	{
		ret.numData = StringToFloat(Htto::StringTools::get_number_by_index(input, index));
		ret.priority = 0;
		ret.isNumber = true;
		ret.strData = "";
		index = Htto::StringTools::get_end_index(input, index);
		return ret;
	}
	else if (input[index] == '+' || input[index] == '-')
	{
		ret.strData = input[index] + std::string();
		ret.priority = 1;
		ret.isNumber = false;
	}
	else if (input[index] == '*' || input[index] == '/')
	{
		ret.strData = input[index] + std::string();
		ret.priority = 2;
		ret.isNumber = false;
	}
	else if (input[index] == '@' || input[index] == '^')
	{
		ret.strData = input[index] + std::string();
		ret.priority = 3;
		ret.isNumber = false;
	}
	else if (input[index] == '(')
	{
		ret.strData = input[index] + std::string();
		ret.priority = -5;
		ret.isNumber = false;
	}
	else if (input[index] == ')')
	{
		ret.strData = input[index] + std::string();
		ret.priority = 100;
		ret.isNumber = false;
	}
	index++;
	return ret;
}
bool Htto::Count::SimpleCount::IsCountSign(char c)
{
	if (c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/'||c=='^')
		return true;
	else
		return false;
}
Htto::Count::ExpressionList<Htto::Polynomial> Htto::Count::SimpleCount::PushToListP(const std::string & str)
{
	int state = 0;
	ExpressionList<Htto::Polynomial> ret;
	Polynomial poly_temp;
	std::string str_temp;
	for (const auto & a : str)
	{
		if (IsCountSign(a))
		{
			state = 2;
			//std::cout << "SEARCH DOG";
		}
		else
			state = 0;
		switch (state)
		{
		case 0:
			str_temp += a;
			break;
		case 2:
			if (str_temp == "")
			{
				ret.push_back(Element<Polynomial>(std::string(1,a), false,get_priority(a)));
				continue;
			}
			ret.push_back(Element<Polynomial>(str_temp, true));
			str_temp = "";
			ret.push_back(Element<Polynomial>(std::string(1,a),false,get_priority(a)));
			break;
		default:
			break;
		}

	}
	if(str_temp!="")
		ret.push_back(Element<Polynomial>(str_temp, true));
	return ret;
}
std::list <unit> SimpleCount::PushToListF(std::string input)
{
	std::list<unit> ret;
	string_replace(input, "@", std::string() + '@');
	int index = 0;
	while (index < input.size())
	{
		ret.push_back(Push_unit(index, input));
	}
	return ret;
}
std::list<unit> SimpleCount::InfixToPostfix(std::list<unit>& ExpStream)
{
	std::list<unit> output;
	std::stack<unit> Stack;
	for (auto & index : ExpStream)
	{
		if (index.priority == 0)
		{
			output.push_back(index);
		}
		else if (Stack.empty())
		{
			Stack.push(index);
		}
		else if (index.strData == "(")
		{
			Stack.push(index);
		}
		else if (index.strData == ")")
		{
			while (!Stack.empty() && Stack.top().strData != "(")
			{
				output.push_back(Stack.top());
				Stack.pop();
			}
			if (!Stack.empty())
				Stack.pop();
		}
		else
		{
			while (!Stack.empty() && index.priority <= Stack.top().priority)
			{
				output.push_back(Stack.top());
				Stack.pop();
			}
			Stack.push(index);
		}
	}
	while (!Stack.empty())
	{
		output.push_back(Stack.top());
		Stack.pop();
	}
	return output;
}
Htto::Fraction SimpleCount::GetCountResult(const std::list<unit>& exp)
{
	Htto::Fraction ret;
	Htto::Fraction tp1;
	Htto::Fraction tp2;
	std::stack<Htto::Fraction> Stack;
	for (const auto & a : exp)
	{
		if (a.isNumber)
		{
			Stack.push(Htto::Fraction(std::to_string(a.numData)));
		}
		else
		{
			if (a.strData == "+")
			{
				tp2 = Stack.top();
				Stack.pop();
				tp1 = Stack.top();
				Stack.pop();

				Stack.push(tp1 + tp2);
			}
			else if (a.strData == "-")
			{
				tp2 = Stack.top();
				Stack.pop();
				tp1 = Stack.top();
				Stack.pop();

				Stack.push(tp1 - tp2);
			}
			else if (a.strData == "*")
			{
				tp2 = Stack.top();
				Stack.pop();
				tp1 = Stack.top();
				Stack.pop();

				Stack.push(tp1 * tp2);
			}
			else if (a.strData == "/")
			{
				tp2 = Stack.top();
				Stack.pop();
				tp1 = Stack.top();
				Stack.pop();
				Stack.push(tp1 / tp2);
			}
			else if (a.strData == "@")
			{
				Stack.top().numsqrt();
			}
			else if (a.strData == "^")
			{
				int tp3 = Stack.top();
				Stack.pop();
				tp1 = Stack.top();
				Stack.pop();
				tp1 = Htto::Handle::Pow(tp1, tp3);
				Stack.push(tp1);
			}
		}
	}
	return Stack.top();
}
Htto::Fraction SimpleCount::Count(const std::string str)
{
	Htto::BalanceStack bs(str);
	if (!bs.checkBalance())
	{
		throw std::runtime_error("str is bad try to reput");
	}
	std::list<unit> x1 = PushToListF(str);
	x1 = InfixToPostfix(x1);
	return GetCountResult(x1);
}

int Htto::Count::get_priority(char ch)
{
	if (ch == '+' || ch == '-')
		return 1;
	else if (ch == '*' || ch == '/')
		return 2;
	else if (ch == '^')
		return 3;
	else if (ch == '(')
		return -5;
	else if (ch == ')')
		return 100;
	return 0;
}
