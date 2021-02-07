#pragma once
#include "main.h"

class String
{
public:
	String() :beginele(nullptr), endele(nullptr) {}
	String(const char* in)
	{
		size_t sc = sizechar(in);
		beginele = alloc.allocate(sc);
		endele = beginele + sc;
		for (auto it = beginele; it != endele; ++it)
			alloc.construct(it, *(in++));
	}
	String(const String& str)
	{
		auto pairp = alloc_n_copy(str.beginele, str.endele);
		beginele = pairp.first;
		endele = pairp.second;
	}
	String & operator=(const String& str)
	{
		auto pairp = alloc_n_copy(str.beginele, str.endele);
		free();
		beginele = pairp.first;
		endele = pairp.second;
		return *this;
	}
	~String()
	{
		free();
	}
	void print()
	{
		for (auto it = beginele; it != endele; it++)
			cout << *it ;
		cout << endl;
	}

private:
	allocator<char> alloc;
	char * beginele;
	char * endele;

	pair<char*, char*> alloc_n_copy(char* begin, char *end)
	{
		char* p = alloc.allocate(end - begin);
		char* pbegin = p;
		for (auto it = begin; it != end; it++)
			alloc.construct(p++, *it);
		return{ pbegin,p };
	}

	size_t sizechar(const char* in)
	{
		const char * it = in;
		
		size_t count = 0;
		while (*(it++))
			++count;
		return count;
	}
	void free()
	{
		for (auto it = beginele; it != endele; it++)
			alloc.destroy(it);
		alloc.deallocate(beginele, endele - beginele);
	}
};