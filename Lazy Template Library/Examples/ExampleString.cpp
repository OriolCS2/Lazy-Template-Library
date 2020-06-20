#include "..\string.h"
#include <string>
#include <iostream>

int main()
{
	std::string str("Hello!");
	std::string s = str.substr(2, 2);

	lzy::string str1("Hello!");
	lzy::string s1 = str1.substr(2, 2);

	/*str.swap*/

	return 0;
}