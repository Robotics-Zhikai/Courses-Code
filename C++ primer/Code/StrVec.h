#pragma once
#include "main.h"
class StrVec //vector类的一个简化版本，只用于string
{
public:
	StrVec() :element(nullptr), first_free(nullptr), cap(nullptr) {}
	StrVec(const StrVec& s1) //有类值的行为
	{
		auto p = alloc_n_copy(s1.begin(), s1.end()); //因为是拷贝构造，所以事先是没有存储值的,因此不用调用free
		element = p.first;
		first_free = p.second;
		cap = first_free;
	}
	StrVec(StrVec&& s1) noexcept 
		:element(s1.element),first_free(s1.first_free),cap(s1.cap)
	{
		s1.element = nullptr; //如果不置nullptr的话，在析构右值引用时会把移动后的对象也析构了
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

	StrVec& operator= (const StrVec& s1)//拷贝构造和拷贝赋值都是新建一个元素
	{
		auto p = alloc_n_copy(s1.begin(), s1.end());
		free(); //free自己
		element = p.first;
		first_free = p.second;
		cap = first_free;
		return *this;
	}
	StrVec& operator=(StrVec && s1) noexcept
	{
		if (&s1 != this)//处理自赋值的情况，否则的话首先就把自己free了，之后的操作也就都没有意义了
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
	void reserve(size_t n) //扩充allocator的大小
	{
		if (n <= capacity())
			return;
		else
		{
			auto p = alloc.allocate(n);
			auto pbegin = p;
			for (auto it = element; it != first_free; it++)
				alloc.construct(p++, move(*it)); // 移动构造新的对象
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
		cout << "容器大小为" << capacity() << endl;
		cout << "元素数目为" << size() << endl;
		for (auto it = begin(); it != end(); it++)
			cout << *it << " ";
		cout << endl;
	}
private:
	/*static */std::allocator<string> alloc;
	void check_n_alloc() { if (cap == first_free) reallocate(); } // 检查空间是否够用
	pair<string*, string*> alloc_n_copy(const string* p1, const string* p2)//分配内存并拷贝一个给定范围内的元素
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
			//上边这条语句可以达到和下边的for循环一样的效果
			for (auto it = element; it != first_free; it++)
				alloc.destroy(it);
			alloc.deallocate(element, capacity());
		}
	}
	void reallocate() //获得更多内存并拷贝已有元素
	{
		auto allocecount = size() ? 2 * size() : 1;
		reserve(allocecount);
	}

	string* element; //元素首地址
	string* first_free;//指向实际存在的数组的尾后指针
	string* cap; //未分配内存的尾后地址
};
