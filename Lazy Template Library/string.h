#pragma once

#pragma warning( disable : 6387 )

#include "LazyTemplateLibrary.h"
#include <string.h>

BEGIN_LAZY_NAMESPACE

class string {
public:

	static constexpr size_t npos = -1;
	using iterator = iterator<char>;

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

	// move constructor
	string(string&& dyingObj) noexcept {
		restore();

		capacity_t = dyingObj.capacity_t;
		buffer = dyingObj.buffer;
		buf_size = dyingObj.buf_size;

		dyingObj.buffer = nullptr;
		dyingObj.buf_size = dyingObj.capacity_t = 0U;
	}

	// TODO: constructor with iterator, size and capacity

	/*------Constructors------*/

	/*------Destructor------*/
	~string() {
		restore();
	}
	/*------Destructor------*/

	/*------Operators------*/
	/*---Equal---*/
	// copy operator
	string& operator=(const string& str) {
		restore();

		allocate(str.capacity_t);
		buf_size = str.buf_size;
		strncpy_s(buffer, capacity_t + 1, str.buffer, capacity_t);

		return *this;
	}
	// copy on die operator
	string& operator=(string&& dyingObj) noexcept {
		if (this != &dyingObj) {
			restore();

			capacity_t = dyingObj.capacity_t;
			buffer = dyingObj.buffer;
			buf_size = dyingObj.buf_size;

			dyingObj.buffer = nullptr;
			dyingObj.buf_size = dyingObj.capacity_t = 0U;
		}
		return *this;
	}

	string& operator=(const char* ptr) {
		restore();

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

	char& operator[](const size_t index) {
		lzy_assert(index < 0 || index >= buf_size, "Index out of bounds");
		return buffer[index];
	}

	const char& operator[](const size_t index) const {
		lzy_assert(index < 0 || index >= buf_size, "Index out of bounds");
		return buffer[index];
	}

	/*------Operators------*/

	void restore() {
		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}
		capacity_t = buf_size = 0U;
	}

	void clear() {
		memset(buffer, 0, capacity_t);
		buf_size = 0U;
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
			memset(buffer + new_size, '\0', buf_size - new_size);
			buf_size = new_size;
		}
		else if (new_size > buf_size) {
			append(new_size - buf_size, ch);
		}
	}

	size_t capacity() const {
		return capacity_t;
	}
	
	size_t size() const {
		return buf_size;
	}

	size_t length() const {
		return buf_size;
	}

	/*---Append---*/
	string& append(size_t count, char ch) {
		char* tmp = new char[count + 1]();
		memset(tmp, ch, count);
		memset(tmp + count, 0, 1);
		append(tmp, count);
		delete[] tmp;
		return *this;
	}

	string& append(const char* ptr) {
		return append(ptr, strlen(ptr));
	}

	string& append(const string& str, size_t begin, size_t end = npos) {
		if (str.buf_size > 0 && begin < str.buf_size && end > begin) {
			return append(str.buffer + begin, ((end != npos) ? ((end - begin + 1 < str.buf_size) ? end - begin + 1 : str.buf_size - begin) : str.buf_size - begin));
		}
		return *this;
	}

	string& append(const string& str) {
		if (str.buf_size > 0) {
			return append(str.buffer, str.buf_size);
		}
		return *this;
	}

	// TODO: Append with iterators

	string& append(const char* buf, size_t bufSize) {
		if (buf != nullptr) {
			size_t newCapacity = bufSize + buf_size;
			if (newCapacity > capacity_t) {
				char* tmp = buffer;
				size_t last_size = buf_size;
				allocate(newCapacity);
				strncpy_s(buffer, last_size + 1, tmp, last_size);
				delete[] tmp;
			}

			memcpy(buffer + buf_size, buf, bufSize);
			buf_size += bufSize;
		}
		return *this;
	}

	/*---Append---*/

	/*---Assign---*/

	string& assign(size_t count, char ch) {
		if (count > capacity_t) {
			restore();
			allocate(count);
		}
		else {
			memset(buffer, 0, capacity_t);
		}
		buf_size = count;
		memset(buffer, ch, buf_size);
		return *this;
	}

	string& assign(const char* ptr) {
		return assign(ptr, strlen(ptr));
	}

	string& assign(const string& str, size_t begin, size_t end = npos) {
		if (str.buf_size > 0 && begin < str.buf_size && end > begin) {
			return assign(str.buffer + begin, ((end != npos) ? ((end - begin + 1 < str.buf_size) ? end - begin + 1 : str.buf_size - begin) : str.buf_size - begin));
		}
		return *this;
	}

	string& assign(const string& str) {
		return assign(str.buffer, str.buf_size);
	}

	string& assign(string&& str) {
		buf_size = str.buf_size;
		capacity_t = str.capacity_t;
		if (buffer != nullptr) {
			delete[] buffer;
		}
		buffer = str.buffer;
		str.buffer = nullptr;
		return *this;
	}

	// TODO: assign with iterators

	string& assign(const char* ptr, size_t size) {
		if (ptr != nullptr) {
			if (size > capacity_t) {
				restore();
				allocate(size);
			}
			else {
				memset(buffer, 0, capacity_t);
			}
			buf_size = size;
			memcpy(buffer, ptr, size);
		}
		return *this;
	}

	/*---Assign---*/

	/*---At---*/

	char& at(const size_t index) {
		lzy_assert(index < 0 || index >= buf_size, "Index out of bounds");
		return buffer[index];
	}

	const char& at(const size_t index) const {
		lzy_assert(index < 0 || index >= buf_size, "Index out of bounds");
		return buffer[index];
	}

	/*---At---*/

	/*---Back---*/

	char& back() {
		lzy_assert(buffer == nullptr || buf_size == 0U, "String is empty");
		return buffer[buf_size - 1];
	}

	const char& back() const {
		lzy_assert(buffer == nullptr || buf_size == 0U, "String is empty");
		return buffer[buf_size - 1];
	}

	/*---Back---*/

	/*---Compare---*/

	int compare(size_t begin, size_t end, const string& str) const {
		return compare(begin, end, str.buffer);
	}

	int compare(size_t begin, size_t num_to_compare, const string& str, size_t subpos, size_t sublen) const {
		if (str.buf_size > 0 && subpos < str.buf_size && sublen > subpos) {
			size_t size = (str.buf_size - subpos < sublen) ? str.buf_size - subpos : sublen;
			char* tmp = new char[size + 1];
			memcpy(tmp, str.buffer + subpos, size);
			memset(tmp + size, 0, 1);
			int ret = compare(begin, num_to_compare, tmp);
			delete[] tmp;
			return ret;
		}
		return -1;
	}

	int compare(size_t begin, size_t num_to_compare, const char* ptr) const {
		lzy_assert(ptr == nullptr || buffer == nullptr, "const char* or buffer to compare is nullptr!");
		if (buf_size > 0 && begin < buf_size && num_to_compare > 0) {
			size_t size = (num_to_compare > buf_size - begin) ? buf_size - begin : num_to_compare;
			char* tmp = new char[size + 1];
			memcpy(tmp, buffer + begin, size);
			memset(tmp + size, 0, 1);
			int ret = strcmp(tmp, ptr);
			delete[] tmp;
			return ret;
		}
		return -1;
	}

	int compare(const string& str) const {
		lzy_assert(str.buffer == nullptr || buffer == nullptr, "string or buffer to compare is nullptr!");
		return strcmp(str.buffer, buffer);
	}

	int compare(const char* ptr) const {
		lzy_assert(ptr == nullptr || buffer == nullptr, "const char* or buffer to compare is nullptr!");
		return strcmp(ptr, buffer);
	}

	/*---Compare---*/

	/*---Copy---*/

	size_t copy(char* ptr, size_t count, size_t pos = 0U) {
		if (ptr != nullptr && pos >= 0 && pos <= buf_size) {
			size_t size = strlen(ptr);
			size_t to_copy = (count < buf_size) ? count : buf_size - 1;
			size_t copied = 0U;

			for (size_t i = 0U; i < size; ++i) {
				if (pos + i < buf_size) {
					ptr[i] = buffer[pos + i];
					++copied;
				}
				else {
					break;
				}
			}

			return copied;
		}
		return 0U;
	}

	/*---Copy---*/

	/*---C_Str---*/

	const char* c_str() const {
		return buffer;
	}

	/*---C_Str---*/

	/*---Data---*/

	const char* data() const {
		return buffer;
	}

	/*---Data---*/

	/*---Empty---*/

	bool empty() const {
		return buf_size == 0U;
	}

	/*---Empty---*/

	/*---Find---*/

	size_t find(const char ch, size_t pos = 0U) const {
		if (pos >= 0 && pos < buf_size) {
			for (size_t i = pos; i < buf_size; ++i) {
				if (buffer[i] == ch) {
					return i;
				}
			}
		}
		return npos;
	}

	size_t find(const char* ptr, size_t pos, size_t count) const {
		if (ptr != nullptr && pos >= 0 && pos < buf_size && count > 0) {
			count = (count > buf_size - pos) ? buf_size - pos : count;
			size_t ret = npos;
			size_t equal = 0U;
			for (size_t i = pos; i < buf_size; ++i) {
				if ((i < buf_size && buffer[i] == ptr[equal])) {
					if (ret == npos) {
						ret = i;
					}
					++equal;
				}
				else {
					--i;
					equal = 0U;
					ret = npos;
				}

				if (equal == count) {
					return ret;
				}
			}
		}
		return npos;
	}

	size_t find(const string& str, size_t pos = 0U) const {
		return find(str.buffer, pos);
	}

	size_t find(const char* ptr, size_t pos = 0U) const {
		if (ptr != nullptr && pos >= 0 && pos < buf_size) {
			size_t ptr_size = strlen(ptr);
			size_t size = buf_size - pos;
			size_t loop_size = (size < ptr_size) ? size : ptr_size;
			size_t count = 0U;
			size_t ret = npos;
			for (size_t i = pos; i < buf_size; ++i) {
				if ((i < buf_size && buffer[i] == ptr[count])) {
					if (ret == npos) {
						ret = i;
					}
					++count;
					if (count == ptr_size) {
						return ret;
					}
				}
				else {
					--i;
					count = 0U;
					ret = npos;
				}
			}

		}
		return npos;
	}

	size_t find_first_not_of(const string& str, size_t pos = 0U) const {
		return find_first_not_of(str.buffer, pos);
	}

	size_t find_first_not_of(const char* ptr, size_t pos, size_t count) const {
		if (ptr != nullptr && pos >= 0 && pos < buf_size && count > 0U) {
			size_t ptr_len = strlen(ptr);
			ptr_len = (ptr_len > count) ? count : ptr_len;
			for (size_t i = pos; i < buf_size; ++i) {
				bool ret = false;
				for (size_t j = 0; j < ptr_len; ++j) {
					if (buffer[i] == ptr[j]) {
						ret = true;
						break;
					}
				}
				if (!ret) {
					return i;
				}
			}
		}
		return npos;
	}

	size_t find_first_not_of(const char* ptr, size_t pos = 0U) const {
		if (pos >= 0 && pos < buf_size && ptr != nullptr) {
			size_t ptr_len = strlen(ptr);
			for (size_t i = pos; i < buf_size; ++i) {
				bool ret = false;
				for (size_t j = 0; j < ptr_len; ++j) {
					if (buffer[i] == ptr[j]) {
						ret = true;
						break;
					}
				}
				if (!ret) {
					return i;
				}
			}
		}
		return npos;
	}

	size_t find_first_not_of(const char ch, size_t pos = 0U) const {
		if (pos >= 0 && pos < buf_size) {
			for (size_t i = pos; i < buf_size; ++i) {
				if (buffer[i] != ch) {
					return i;
				}
			}
		}
		return npos;
	}

	size_t find_first_of(const string& str, size_t pos = 0U) const {
		return find_first_of(str.buffer, pos);
	}

	size_t find_first_of(const char* ptr, size_t pos, size_t count) const {
		if (ptr != nullptr && pos >= 0 && pos < buf_size && count > 0U) {
			size_t ptr_len = strlen(ptr);
			ptr_len = (ptr_len > count) ? count : ptr_len;
			for (size_t i = pos; i < buf_size; ++i) {
				for (size_t j = 0U; j < ptr_len; ++j) {
					if (buffer[i] == ptr[j]) {
						return i;
					}
				}
			}
		}
		return npos;
	}

	size_t find_first_of(const char* ptr, size_t pos = 0U) const {
		if (ptr != nullptr && pos >= 0 && pos < buf_size) {
			size_t ptr_len = strlen(ptr);
			for (size_t i = pos; i < buf_size; ++i) {
				for (size_t j = 0; j < ptr_len; ++j) {
					if (buffer[i] == ptr[j]) {
						return i;
					}
				}
			}
		}
		return npos;
	}

	size_t find_first_of(const char ch, size_t pos) const {
		return find(ch, pos);
	}

	size_t find_last_not_of(const char ch, size_t last_pos_search = npos) const {
		size_t pos = (last_pos_search >= buf_size) ? buf_size - 1 : last_pos_search;
		if (pos >= 0U && pos < buf_size) {
			size_t last_match = npos;
			for (size_t i = 0U; i <= pos; ++i) {
				if (buffer[i] != ch) {
					last_match = i;
				}
			}
			return last_match;
		}
		return npos;
	}

	size_t find_last_not_of(const string& str, size_t last_pos_search = npos) const {
		return find_last_not_of(str.buffer, last_pos_search);
	}

	size_t find_last_not_of(const char* ptr, size_t last_pos_search, size_t count) const {
		size_t pos = (last_pos_search >= buf_size) ? buf_size - 1 : last_pos_search;
		if (ptr != nullptr && pos >= 0 && pos < buf_size) {
			size_t ptr_len = strlen(ptr);
			ptr_len = (ptr_len > count) ? count : ptr_len;
			size_t last_match = npos;
			for (size_t i = 0U; i <= pos; ++i) {
				bool matches = false;
				for (size_t j = 0U; j < ptr_len; ++j) {
					if (buffer[i] == ptr[j]) {
						matches = true;
					}
				}
				if (!matches) {
					last_match = i;
				}
			}
			return last_match;
		}
		return npos;
	}

	size_t find_last_not_of(const char* ptr, size_t last_pos_search = npos) const {
		size_t pos = (last_pos_search >= buf_size) ? buf_size - 1 : last_pos_search;
		if (ptr != nullptr && pos >= 0 && pos < buf_size) {
			size_t ptr_len = strlen(ptr);
			size_t last_match = npos;
			for (size_t i = 0U; i <= pos; ++i) {
				bool matches = false;
				for (size_t j = 0U; j < ptr_len; ++j) {
					if (buffer[i] == ptr[j]) {
						matches = true;
					}
				}
				if (!matches) {
					last_match = i;
				}
			}
			return last_match;
		}
		return npos;
	}

	size_t find_last_of(const char ch, size_t last_pos_search = npos) const {
		size_t last = npos;
		size_t pos = (last_pos_search >= buf_size) ? buf_size - 1 : last_pos_search;
		for (size_t i = 0U; i <= pos; ++i) {
			if (buffer[i] == ch) {
				last = i;
			}
		}
		return last;
	}

	size_t find_last_of(const string& str, size_t last_pos_search = npos) const {
		return find_last_of(str.buffer, last_pos_search);
	}

	size_t find_last_of(const char* ptr, size_t last_pos_search, size_t count) const {
		size_t pos = (last_pos_search >= buf_size) ? buf_size - 1 : last_pos_search;
		if (ptr != nullptr && pos >= 0 && pos < buf_size) {
			size_t ptr_len = strlen(ptr);
			ptr_len = (ptr_len > count) ? count : ptr_len;
			size_t last = npos;
			for (size_t i = 0U; i <= pos; ++i) {
				for (size_t j = 0U; j < ptr_len; ++j) {
					if (buffer[i] == ptr[j]) {
						last = i;
					}
				}
			}
			return last;
		}
		return npos;
	}

	size_t find_last_of(const char* ptr, size_t last_pos_search = npos) const {
		size_t pos = (last_pos_search >= buf_size) ? buf_size - 1 : last_pos_search;
		if (ptr != nullptr && pos >= 0 && pos < buf_size) {
			size_t last = npos;
			size_t ptr_len = strlen(ptr);
			for (size_t i = 0U; i <= pos; ++i) {
				for (size_t j = 0U; j < ptr_len; ++j) {
					if (buffer[i] == ptr[j]) {
						last = i;
					}
				}
			}
			return last;
		}
		return npos;
	}

	size_t rfind(const char ch, size_t end = npos) const {
		if (end >= 0) {
			end = (end > buf_size) ? buf_size : end;
			size_t last = npos;
			for (size_t i = 0U; i < end; ++i) {
				if (buffer[i] == ch) {
					last = i;
				}
			}
			return last;
		}
		return npos;
	}

	size_t rfind(const char* ptr, size_t end, size_t count) const {
		if (ptr != nullptr && end >= 0) {
			end = (end > buf_size) ? buf_size : end + 1;
			size_t ret = npos;
			size_t equal = 0U;
			size_t last = npos;
			for (size_t i = 0U; i <= end; ++i) {
				if ((i < buf_size && buffer[i] == ptr[equal])) {
					if (ret == npos) {
						ret = i;
					}
					++equal;
				}
				else {
					equal = 0U;
					ret = npos;
					--i;
				}

				if (equal == count) {
					last = ret;
					equal = 0U;
					ret = npos;
				}
			}
			return last;
		}
		return npos;
	}

	size_t rfind(const string& str, size_t end = npos) const {
		return rfind(str.buffer, end);
	}

	size_t rfind(const char* ptr, size_t end = npos) const {
		if (ptr != nullptr && end >= 0) {
			end = (end > buf_size) ? buf_size : end + 1;
			size_t ptr_size = strlen(ptr);
			size_t size = buf_size - end;
			size_t loop_size = (size < ptr_size) ? size : ptr_size;
			size_t count = 0U;
			size_t ret = npos;
			size_t last = npos;
			for (size_t i = 0U; i <= end; ++i) {
				if ((i < buf_size && buffer[i] == ptr[count])) {
					if (ret == npos) {
						ret = i;
					}
					++count;
					if (count == ptr_size) {
						last = ret;
						ret = npos;
						count = 0U;
					}
				}
				else {
					--i;
					count = 0U;
					ret = npos;
				}
			}
			return last;
		}
		return npos;
	}

	/*---Find---*/

	/*---Front---*/

	char& front() {
		lzy_assert(buffer == nullptr || buf_size == 0U, "String is empty");
		return buffer[0];
	}

	const char& front() const {
		lzy_assert(buffer == nullptr || buf_size == 0U, "String is empty");
		return buffer[0];
	}

	/*---Front---*/

	/*---PopBack---*/

	void pop_back() {
		if (buf_size > 0) {
			buffer[--buf_size] = '\0';
		}
	}

	/*---PopBack---*/

	/*---PushBack---*/

	void push_back(const char ch) {
		if (++buf_size > capacity_t) {
			char* tmp = buffer;
			allocate(capacity_t + 1);
			memcpy(buffer, tmp, buf_size - 1);
			delete[] tmp;
		}
		buffer[buf_size - 1] = ch;
	}

	/*---PushBack---*/

	/*---ShrinkToFit---*/
	
	void shrink_to_fit() {
		if (capacity_t > 0 && capacity_t != buf_size) {
			char* tmp = buffer;
			allocate(buf_size);
			memcpy(buffer, tmp, buf_size);
			delete[] tmp;
		}
	}

	/*---ShrinkToFit---*/

	/*---SubString---*/

	string substr(size_t begin = 0U, size_t count = npos) const {
		if (buf_size > 0 && begin < buf_size) {
			string s;
			for (size_t i = begin; i < buf_size; ++i) {
				s.push_back(buffer[i]);
				if (s.size() == count) {
					break;
				}
			}
			return s;
		}
		return string();
	}

	/*---SubString---*/

	/*---Swap---*/

	void swap(string& str) {
		string aux = str;
		str = *this;
		*this = aux;
	}

	/*---Swap---*/

	/*---Iterators---*/

	iterator begin() {
		return iterator(&buffer[0], 1U);
	}

	iterator end() {
		return iterator(&buffer[buf_size], 1U);
	}

	/*---Iterators---*/

	// TODO: begin, cbegin, cend, crbegin, crend, end, rbegin, rend, erase, replace, insert, assign and constructor with format

private:

	void allocate(size_t size) {
		this->capacity_t = size;
		buffer = new char[size + 1]();
	}

private:

	char* buffer = nullptr;
	size_t capacity_t = 0U;
	size_t buf_size = 0U;
};


END_LAZY_NAMESPACE