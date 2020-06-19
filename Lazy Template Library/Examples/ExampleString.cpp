#include "..\string.h"
#include <string>

int main()
{
	std::string str("loolllofor. snon-alphabetic characters...");

	std::size_t found = str.find_last_of("s.",100, 2);

	lzy::string str1("loolllofor. snon-alphabetic characters...");

	size_t found1 = str1.find_last_of("s.", 100, 2);

	return 0;
}