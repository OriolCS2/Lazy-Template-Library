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
	
	f.reserve(40);
	f.at(0) = 'F';
	

	a.reserve(40);
	a.at(0) = 'F';

	return 0;
}