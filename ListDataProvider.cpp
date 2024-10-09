#include "ListDataProvider.h"

ListDataProvider::ListDataProvider(std::vector<Utf8String> data)
	: data(std::move(data))
	, pos(0)
{}

const Utf8String& ListDataProvider::Get() {
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

bool ListDataProvider::Empty() {
	return data.empty();
}