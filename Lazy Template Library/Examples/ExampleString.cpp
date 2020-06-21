#include "..\string.h"
#include <string>
#include <iostream>

int main()
{
	std::string hello("hello");
	std::string bye("bye");
	bye.reserve(100);

	hello.swap(bye);

	lzy::string hello1("hello");
	lzy::string bye1("bye");
	bye1.reserve(100);

	hello1.swap(bye1);

	return 0;
}