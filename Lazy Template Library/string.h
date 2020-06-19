#pragma once

#pragma warning( disable : 6387 )

#include "LazyTemplateLibrary.h"
#include <string.h>

BEGIN_LAZY_NAMESPACE

class string {
public:
	/*------Constructors------*/

	// Default constructor
	string() : buffer(nullptr), size(0U) {}

	// Const char* constructor
	string(const char* ptr) {
		if (ptr != nullptr) {
			allocate(strlen(ptr));
			strncpy_s(buffer, size + 1, ptr, size);
		}
	}

	// another string constructor
	string(const string& object) {
		allocate(object.size);
		strncpy_s(buffer, size + 1, object.buffer, size);
	}

	string(string&& dyingObj) noexcept {
		clear();

		size = dyingObj.size;
		buffer = dyingObj.buffer;

		dyingObj.buffer = nullptr;
	}

	/*------Constructors------*/

	/*------Destructor------*/
	~string() {
		clear();
	}
	/*------Destructor------*/

	/*------Operators------*/

	// copy operator
	string& operator=(const string& str) {
		clear();

		allocate(str.size);
		strncpy_s(buffer, size + 1, str.buffer, size);

		return *this;
	}
	// copy on die operator
	string& operator=(string&& dyingObj) noexcept {
		clear();

		size = dyingObj.size;
		buffer = dyingObj.buffer;

		dyingObj.buffer = nullptr;

		return *this;
	}

	string& operator=(const char* ptr) {
		clear();

		if (ptr != nullptr) {
			allocate(strlen(ptr));
			strncpy_s(buffer, size + 1, ptr, size);
		}

		return *this;
	}

	string& operator+=(const string& str) {
		return append(str.buffer, str.size);
	}

	string& operator+=(const char* ptr) {
		if (ptr != nullptr) {
			return append(ptr, strlen(ptr));
		}
		return *this;
	}

	string operator+(const string& str) {
		string tmp;

		tmp.allocate(size + str.size);

		strncpy_s(tmp.buffer, size + 1, buffer, size);
		strncpy_s(tmp.buffer + size, str.size + 1, str.buffer, str.size);

		return tmp;
	}

	string operator+(const char* ptr) {
		string tmp;
		if (ptr != nullptr) {
			size_t ptrSize = strlen(ptr);
			tmp.allocate(size + ptrSize);

			strncpy_s(tmp.buffer, size + 1, buffer, size);
			strncpy_s(tmp.buffer + size, ptrSize + 1, ptr, ptrSize);
		}
		else {
			tmp = *this;
		}
		return tmp;
	}

	bool operator==(const string& str) const {
		return strcmp(buffer, str.buffer) == 0;
	}

	bool operator==(const char* ptr) const {
		if (ptr != nullptr) {
			return strcmp(ptr, buffer) == 0;
		}
		return false;
	}

	bool operator!=(const string& str) const {
		return strcmp(buffer, str.buffer) != 0;
	}

	bool operator!=(const char* ptr) const {
		if (ptr != nullptr) {
			return strcmp(ptr, buffer) != 0;
		}
		return false;
	}
	/*------Operators------*/

	void clear() {
		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}
		size = 0U;
	}


private:

	void allocate(size_t size) {
		this->size = size;
		buffer = new char[size + 1];
	}

	string& append(const char* buf, size_t bufSize) {
		unsigned int need_size = bufSize + size + 1;
		if (need_size > size) {
			char* tmp = buffer;
			size_t last_size = size;
			allocate(need_size);
			strncpy_s(buffer, last_size + 1, tmp, last_size);
			delete[] tmp;
		}
		strcat_s(buffer, size + 1, buf);
	}

private:

	char* buffer = nullptr;
	size_t size = 0U;
};


END_LAZY_NAMESPACE