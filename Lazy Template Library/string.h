#pragma once

#pragma warning( disable : 6387 )

#include "LazyTemplateLibrary.h"
#include <string.h>

BEGIN_LAZY_NAMESPACE

class string {
public:
	/*------Constructors------*/

	// Default constructor
	string() : buffer(nullptr), capacity_t(0U), buf_size(0U) {}

	// Const char* constructor
	string(const char* ptr) {
		if (ptr != nullptr) {
			buf_size = strlen(ptr);
			allocate(buf_size);
			strncpy_s(buffer, capacity_t + 1, ptr, capacity_t);
		}
	}

	// another string constructor
	string(const string& object) {
		allocate(object.capacity_t);
		buf_size = object.buf_size;
		strncpy_s(buffer, capacity_t + 1, object.buffer, capacity_t);
	}

	string(string&& dyingObj) noexcept {
		clear();

		capacity_t = dyingObj.capacity_t;
		buffer = dyingObj.buffer;
		buf_size = dyingObj.buf_size;

		dyingObj.buffer = nullptr;
	}

	/*------Constructors------*/

	/*------Destructor------*/
	~string() {
		clear();
	}
	/*------Destructor------*/

	/*------Operators------*/
	/*---Equal---*/
	// copy operator
	string& operator=(const string& str) {
		clear();

		allocate(str.capacity_t);
		buf_size = str.buf_size;
		strncpy_s(buffer, capacity_t + 1, str.buffer, capacity_t);

		return *this;
	}
	// copy on die operator
	string& operator=(string&& dyingObj) noexcept {
		clear();

		capacity_t = dyingObj.capacity_t;
		buffer = dyingObj.buffer;
		buf_size = dyingObj.buf_size;

		dyingObj.buffer = nullptr;

		return *this;
	}

	string& operator=(const char* ptr) {
		clear();

		if (ptr != nullptr) {
			buf_size = strlen(ptr);
			allocate(buf_size);
			strncpy_s(buffer, capacity_t + 1, ptr, capacity_t);
		}

		return *this;
	}
	/*---Equal---*/

	/*---Add/Equal---*/
	string& operator+=(const string& str) {
		return append(str.buffer, str.buf_size);
	}

	string& operator+=(const char* ptr) {
		if (ptr != nullptr) {
			return append(ptr, strlen(ptr));
		}
		return *this;
	}
	/*---Add/Equal---*/

	/*---Add---*/
	string operator+(const string& str) {
		string tmp;

		tmp.allocate(buf_size + str.buf_size);
		tmp.buf_size = tmp.capacity_t;

		strncpy_s(tmp.buffer, capacity_t + 1, buffer, capacity_t);
		strncpy_s(tmp.buffer + capacity_t, str.capacity_t + 1, str.buffer, str.capacity_t);

		return tmp;
	}

	string operator+(const char* ptr) {
		string tmp;
		if (ptr != nullptr) {
			size_t ptrSize = strlen(ptr);
			tmp.allocate(buf_size + ptrSize);
			tmp.buf_size = tmp.capacity_t;

			strncpy_s(tmp.buffer, capacity_t + 1, buffer, capacity_t);
			strncpy_s(tmp.buffer + capacity_t, ptrSize + 1, ptr, ptrSize);
		}
		else {
			tmp = *this;
		}
		return tmp;
	}
	/*---Add---*/


	/*---Compare Equal---*/
	bool operator==(const string& str) const {
		return strcmp(buffer, str.buffer) == 0;
	}

	bool operator==(const char* ptr) const {
		if (ptr != nullptr) {
			return strcmp(ptr, buffer) == 0;
		}
		return false;
	}
	/*---Compare Equal---*/

	/*---Compare Different---*/
	bool operator!=(const string& str) const {
		return strcmp(buffer, str.buffer) != 0;
	}

	bool operator!=(const char* ptr) const {
		if (ptr != nullptr) {
			return strcmp(ptr, buffer) != 0;
		}
		return false;
	}
	/*---Compare Different---*/
	/*------Operators------*/

	void clear() {
		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}
		capacity_t = buf_size = 0U;
	}

	void reserve(size_t new_capacity) {
		if (capacity_t >= new_capacity) {
			return;
		}

		char* tmp = buffer;
		size_t last_size = buf_size;
		allocate(new_capacity);
		strncpy_s(buffer, last_size + 1, tmp, last_size);
		delete[] tmp;
	}

	void resize(size_t new_size, char ch = '\0') {
		if (new_size < buf_size) {
			char* tmp = buffer;
			buf_size = new_size;
			allocate(new_size);
			memcpy(buffer, tmp, new_size);
			delete[] tmp;
		}
		else if (new_size > buf_size) {
			size_t newBufSize = new_size - buf_size;
			char* tmp = new char[newBufSize + 1]();
			memset(tmp, ch, newBufSize);
			memset(tmp + newBufSize, 0, 1);
			append(tmp, newBufSize);
			delete[] tmp;
		}
	}

	size_t capacity() {
		return capacity_t;
	}
	
	size_t size() {
		return buf_size;
	}

private:

	void allocate(size_t size) {
		this->capacity_t = size;
		buffer = new char[size + 1]();
	}

	string& append(const char* buf, size_t bufSize) {
		size_t newCapacity = bufSize + buf_size;
		if (newCapacity > capacity_t) {
			char* tmp = buffer;
			size_t last_size = buf_size;
			allocate(newCapacity);
			strncpy_s(buffer, last_size + 1, tmp, last_size);
			delete[] tmp;
		}

		buf_size += bufSize;
		strcat_s(buffer, capacity_t + 1, buf);
		//strcat_s(buffer, buf_size, buf);

		return *this;
	}

private:

	char* buffer = nullptr;
	size_t capacity_t = 0U;
	size_t buf_size = 0U;
};


END_LAZY_NAMESPACE