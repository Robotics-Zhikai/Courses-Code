#ifndef CP5_ex12_2_h
#define CP5_ex12_2_h

#include <string>
#include <iostream>
#include <memory>



class StrBlob
{
	friend class StrBlobPtr;
public:
	typedef std::vector<std::string>::size_type size_type;
	//构造函数
	//拷贝构造函数为默认合成的 因为没有必要写出来
	StrBlob() :data(std::make_shared<std::vector<std::string>>()) {}
	StrBlob(std::initializer_list<std::string> i1) :data(std::make_shared<std::vector<std::string>>(i1)) {}
	StrBlob(const vector<string>& vecstr) :data(std::make_shared<std::vector<std::string>>(vecstr)) {}

	StrBlob(const StrBlob& in) :data(make_shared<vector<string>>(*in.data)) {} //类值的拷贝构造函数 否则如果是默认的话就是类指针的
	StrBlob& operator=(const StrBlob& in) //用const修饰的话适用范围更广些 类值的拷贝赋值函数 否则如果默认的话就是类指针的
	{
		data = make_shared<vector<string>>(*in.data);
		return *this;
	}


	//读取data属性
	size_type size() const { /*data = NULL;//由于有const的存在 所以不能给成员数据赋值*/ return data->size(); }
	bool empty() const { return data->empty(); }
	//添加和删除元素
	void push_back(const std::string &input) { data->push_back(input); }
	void pop_back() { check(0, "data指向的元素为空！"); data->pop_back(); }
	//元素访问
	std::string& front() { check(0, "data指向的元素为空！"); return data->front(); };
	std::string& back() { check(0, "data指向的元素为空！"); return data->back(); };
	const std::string& front()const { check(0, "data指向的元素为空！"); return data->front(); };
	const std::string& back()const { check(0, "data指向的元素为空！"); return data->back(); };
	//函数名称后边的const强调的是常指针，并不意味着指针所指的对象也为常量，这里函数的返回类型必须加const
	//因为既然加了重载，那么只有当const Strblob时才会进入，那么如果不加const给返回类型，那就意味着可以修改指向的元素，这会违背用户的初衷。
	std::string & operator[](size_type i) { check(i, "索引超界！"); return (*data)[i]; }
	const std::string & operator[](size_type i) const { check(i, "索引超界！"); return (*data)[i]; }//这个也是，必须返回类型为const才不与用户初衷相悖
	//std::shared_ptr<std::vector<std::string>>& returndata() const { return data; }
	//这样会报错，因为常量被一个非常量引用了，去掉了const属性。
	//如果不报错的话是不安全的，因为如果returndata重载了非const类型，那么可以间接修改const类型的strblob，是一个错误！

	//拷贝赋值函数也为默认合成的 没有必要写出来 因为只有一个data

	StrBlobPtr begin()const ;
	StrBlobPtr end()const ;
private:
	std::shared_ptr<std::vector<std::string>> data;
	//void check(size_type i, const std::string &msg);//这是两个函数，一个供const型的类调用 一个供普通类调用
	void check(size_type i, const std::string &msg)const;
};



class StrBlobPtr //搞了一个指向strblob的迭代器
{
public:
	StrBlobPtr() :curr(0) {}
	StrBlobPtr(const StrBlob&in, size_t index = 0) :wptr(in.data), curr(index) {} //StrBlob如果不是const的话就不能输入const StrBlob
	string& deref()const //每次解引用都要判断是否被销毁 当试图解应用尾后指针时会抛出错误
	{
		auto data = Check(curr);
		return (*data)[curr];
	}
	StrBlobPtr& incr() //前缀递增 所谓前缀是指前闭后开的前缀
	{
		Check(curr);
		curr++;
		return *this;
	}
	bool operator==(const StrBlobPtr& s2)const
	{
		auto shareptrthis = wptr.lock();
		auto shareptrs2 = s2.wptr.lock();
		if (!shareptrthis || !shareptrs2)
			throw exception("shared_ptr已经被释放了");
		else
		{
			if (shareptrthis == shareptrs2 && curr == s2.curr)
				return 1;
			else
				return 0;
		}
	}
	bool operator!=(const StrBlobPtr& s2)const
	{
		return !operator==(s2);
	}
private:
	shared_ptr<vector<string>> Check(size_t index) const //检查两个东西 一检查数据是否被释放 二检查索引是否超出维度
	{
		auto p = wptr.lock(); //似乎只有通过这种方法才能判断指向的shareptr是否被释放
		if (!p) //说明shared_ptr已经被释放了
			throw exception("shared_ptr已经被释放了");
		if ( index>= p->size())
			throw exception("索引超出维度");
		return p;
	}
	size_t curr;
	weak_ptr<vector<string>> wptr; //weak_ptr绑定到share_ptr
};

void StrBlob::check(size_type i, const std::string &msg)const
{

	if (i >= size())
	{
		throw std::out_of_range(msg);
	}
}
StrBlobPtr StrBlob::begin()const { return StrBlobPtr(*this, 0); }
StrBlobPtr StrBlob::end()const { return StrBlobPtr(*this, data->size()); }
#endif