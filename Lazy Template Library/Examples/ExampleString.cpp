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
	f.resize(15, 'A');
	f.append(10, 'v');
	f.append("HOLA");
	f.append("KKK", 2);
	f.append(std::string("F233"), 2);
	f.append(std::string(""));


	a.reserve(40);
	a.resize(15, 'A');
	a.append(10, 'v');
	a.append("HOLA");
	a.append("KKK", 2);
	a.append(lzy::string("F233"), 2);
	a.append(lzy::string(""));

	return 0;
}