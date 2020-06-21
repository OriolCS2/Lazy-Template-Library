#pragma once

#include <assert.h>

#define BEGIN_LAZY_NAMESPACE namespace lzy {
#define END_LAZY_NAMESPACE }

#define lzy_assert(condition, message) assert((!(condition) && message))

BEGIN_LAZY_NAMESPACE

template <typename T>
class iterator {
public:
	iterator(T* current, size_t offset) : data(current), offset(offset) {}

	bool operator!=(const iterator<T>& it) {
		return data != it.data;
	}

	iterator<T> operator++() {
		data += offset;
		return *this;
	}

	iterator<T> operator+(unsigned int add) {
		data += offset * add;
		return *this;
	}

	T& operator *() {
		return *data;
	}

private:
	// TODO: get the index
	T* data = nullptr;
	size_t offset = 0U;

};

END_LAZY_NAMESPACE