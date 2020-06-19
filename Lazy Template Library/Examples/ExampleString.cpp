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
	f.assign(2, 'c');
	f.assign("Hello");
	f.assign("Hello", 3);
	f.assign(std::string("Hello"), 1, 3);
	f.assign(std::string("sada"));
	

	a.reserve(40);
	a.assign(2, 'c');
	a.assign(lzy::string("vvvvvv"));

	return 0;
}