#pragma once

#include "utils/utf8string.h"

class ListDataProvider {
public:
	template<class... Args>
	static ListDataProviderPtr Create(Args... args) {
		return std::make_shared<ListDataProvider>(std::forward<Args>(args)...);
	}
	ListDataProvider(std::vector<Utf8String> data);
	const Utf8String& Get();
	bool Next();
	bool Prev();
	bool Empty();
protected:
	std::vector<Utf8String> data;
	int pos = -1;
};
