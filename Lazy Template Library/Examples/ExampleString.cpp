#include "..\string.h"
#include <string>

int main()
{
	std::string str("ABCD");


	lzy::string str1("ABCD");
	
	/*str.replace*/

	str1.reserve(15);
	str.push_back('2');
	while (str1.size() != 15) {
		str1.push_back('2');
	}
	
	return 0;
}