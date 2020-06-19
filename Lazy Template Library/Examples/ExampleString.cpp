#include "..\string.h"

#include <string>

lzy::string getAString()
{
	lzy::string str("AAAAAAAaaa");
	return str;
}

int main()
{
	lzy::string a("aaa");
	std::string f("aaa");
	std::string f2;
	f.resize(15, 'A');

	a.resize(15, 'A');

	return 0;
}