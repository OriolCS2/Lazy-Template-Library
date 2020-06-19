#include "..\string.h"
#include <string>

int main()
{
	std::string str("look for non-alphabetic characters...");

	std::size_t found = str.find_first_of("kl", 0, 1);

	lzy::string str1("look for non-alphabetic characters...");

	size_t found1 = str1.find_first_of("kl", 0, 1);

	return 0;
}