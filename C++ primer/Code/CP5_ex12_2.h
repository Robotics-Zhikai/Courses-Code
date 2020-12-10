#ifndef CP5_ex12_2_h
#define CP5_ex12_2_h

#include <string>
#include <iostream>
#include <memory>

class StrBlob
{
public:
	typedef std::vector<std::string>::size_type size_type;
	//构造函数
	//拷贝构造函数为默认合成的 因为没有必要写出来
	StrBlob() :data(std::make_shared<vector<std::string>>()) {}
	StrBlob(std::initializer_list<std::string> i1) :data(std::make_shared<vector<std::string>>(i1)) {}
	//读取data属性
	size_type size() const { /*data = NULL;//由于有const的存在 所以不能给成员数据赋值*/ return data->size(); }
	bool empty() const { return data->empty(); }
	//添加和删除元素
	void push_back(const std::string &input) { data->push_back(input); }
	void pop_back() { check(0, "data指向的元素为空！"); data->pop_back(); }
	//元素访问
	std::string& front() { check(0, "data指向的元素为空！"); return data->front(); };
	std::string& back() { check(0, "data指向的元素为空！"); return data->back(); };
	//std::string& front()const { check(0, "data指向的元素为空！"); return data->front(); };
	//std::string& back()const { check(0, "data指向的元素为空！"); return data->back(); };
	std::string & operator[](size_type i) { check(i, "索引超界！"); return (*data)[i]; }
	//拷贝赋值函数也为默认合成的 没有必要写出来 因为只有一个data
private:
	std::shared_ptr<std::vector<std::string>> data;
	//void check(size_type i, const std::string &msg);//这是两个函数，一个供const型的类调用 一个供普通类调用
	void check(size_type i, const std::string &msg)const;
};

void StrBlob::check(size_type i, const std::string &msg)const
{
	StrBlob::front();
	if (i >= size())
	{
		throw std::out_of_range(msg);
	}
}


#endif