#pragma once
#include "main.h"
class StrVec //vector���һ���򻯰汾��ֻ����string
{
public:
	StrVec() :element(nullptr), first_free(nullptr), cap(nullptr) {}
	StrVec(const StrVec& s1) //����ֵ����Ϊ
	{
		auto p = alloc_n_copy(s1.begin(), s1.end()); //��Ϊ�ǿ������죬����������û�д洢ֵ��,��˲��õ���free
		element = p.first;
		first_free = p.second;
		cap = first_free;
	}
	StrVec(StrVec&& s1) noexcept 
		:element(s1.element),first_free(s1.first_free),cap(s1.cap)
	{
		s1.element = nullptr; //�������nullptr�Ļ�����������ֵ����ʱ����ƶ���Ķ���Ҳ������
		s1.first_free = nullptr;
		s1.cap = nullptr;
	}
	StrVec(const initializer_list<string> & in):StrVec()
	{
		auto p = alloc_n_copy(in.begin(), in.end());
		element = p.first;
		first_free = p.second;
		cap = first_free;
	}

	StrVec& operator= (const StrVec& s1)//��������Ϳ�����ֵ�����½�һ��Ԫ��
	{
		auto p = alloc_n_copy(s1.begin(), s1.end());
		free(); //free�Լ�
		element = p.first;
		first_free = p.second;
		cap = first_free;
		return *this;
	}
	StrVec& operator=(StrVec && s1) noexcept
	{
		if (&s1 != this)//�����Ը�ֵ�����������Ļ����ȾͰ��Լ�free�ˣ�֮��Ĳ���Ҳ�Ͷ�û��������
		{
			free();
			element = s1.element;
			first_free = s1.first_free;
			cap = s1.cap;

			s1.element = nullptr;
			s1.first_free = nullptr;
			s1.cap = nullptr;
		}
		return *this;
	}
	string& operator[](size_t i)
	{
		return *(element + i);
	}
	~StrVec()
	{
		free();
	}
	void push_back(const string& s1)
	{
		check_n_alloc();
		alloc.construct(first_free++, s1);
	}
	void reserve(size_t n) //����allocator�Ĵ�С
	{
		if (n <= capacity())
			return;
		else
		{
			auto p = alloc.allocate(n);
			auto pbegin = p;
			for (auto it = element; it != first_free; it++)
				alloc.construct(p++, move(*it)); // �ƶ������µĶ���
			auto pend = p;
			free();
			element = pbegin;
			first_free = pend;
			cap = element + n;
		}
	}
	void resize(size_t n,const string& str = string(""))
	{
		if (n > capacity())
		{
			reserve(n);
			for (auto it = first_free; it != cap; ++it)
				push_back(str);
		}
		else if (n <= capacity() && n > size())
		{
			for (auto it = first_free; it != cap; ++it)
				push_back(str);
		}
		else
		{
			auto p = alloc_n_copy(element, element + n);
			free();
			element = p.first;
			first_free = p.second;
			cap = first_free;
		}
	}
	size_t size()const {return first_free - element;}
	size_t capacity()const { return cap - element; }
	string* begin()const { return element; }
	string* end()const { return first_free; }

	void print()
	{
		cout << "������СΪ" << capacity() << endl;
		cout << "Ԫ����ĿΪ" << size() << endl;
		for (auto it = begin(); it != end(); it++)
			cout << *it << " ";
		cout << endl;
	}
private:
	/*static */std::allocator<string> alloc;
	void check_n_alloc() { if (cap == first_free) reallocate(); } // ���ռ��Ƿ���
	pair<string*, string*> alloc_n_copy(const string* p1, const string* p2)//�����ڴ沢����һ��������Χ�ڵ�Ԫ��
	{
		auto p = alloc.allocate(p2-p1);
		auto pbegin = p;
		for (auto it = p1; it != p2; it++)
			alloc.construct(p++, *it);
		return{ pbegin,p };
	}
	void free()
	{
		if (element != nullptr)
		{
			//for_each(element, first_free, [this](const string& str) {alloc.destroy(&str); });
			//�ϱ����������Դﵽ���±ߵ�forѭ��һ����Ч��
			for (auto it = element; it != first_free; it++)
				alloc.destroy(it);
			alloc.deallocate(element, capacity());
		}
	}
	void reallocate() //��ø����ڴ沢��������Ԫ��
	{
		auto allocecount = size() ? 2 * size() : 1;
		reserve(allocecount);
	}

	string* element; //Ԫ���׵�ַ
	string* first_free;//ָ��ʵ�ʴ��ڵ������β��ָ��
	string* cap; //δ�����ڴ��β���ַ
};
