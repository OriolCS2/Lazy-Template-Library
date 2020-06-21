#include "..\string.h"
#include <string>
#include <iostream>

int main()
{
	std::string test("Test");

	for (std::string::iterator item = test.begin(); item != test.end(); ++item) {
		auto i = item;

		int j = 0;
	}
	lzy::string testing("Testing");

	for (lzy::string::iterator item = testing.begin(); item != testing.end(); ++item) {
		char& ch = *item;
		ch = 'F';
		
	}

	return 0;
}