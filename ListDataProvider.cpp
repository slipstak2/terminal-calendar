#include "ListDataProvider.h"
#include <algorithm>

ListDataProvider::ListDataProvider(std::vector<Utf8String> data)
	: data(std::move(data))
	, pos(0)
{}

const Utf8String& ListDataProvider::Get() const {
	return data[pos];
}

bool ListDataProvider::Next() {
	if (pos == -1) {
		return false;
	}
	if (pos + 1 >= data.size()) {
		return false;
	}
	++pos;
	return true;
}

bool ListDataProvider::Prev() {
	if (pos == -1) {
		return false;
	}
	if (pos == 0) {
		return false;
	}
	--pos;
	return true;
}

bool ListDataProvider::Empty() const {
	return data.empty();
}

short ListDataProvider::MaxLen() const {
	if (data.empty()) {
		return 0;
	}

	return (short)std::max_element(data.begin(), data.end(),
		[](const Utf8String& lhs, const Utf8String& rhs) {
			return lhs.size() < rhs.size();
		})->size();
}