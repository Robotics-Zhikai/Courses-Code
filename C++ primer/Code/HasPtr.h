#pragma once
#include "main.h"
#include <iostream>
using namespace std;

std::string * psstore;
class HasPtr
{
public:
	~HasPtr()
	{
		psstore = ps;
		//delete ps;
	}
	HasPtr(int num) :ps(new std::string()), i(num) 
	{
		//std::cout << ps;
	}
	HasPtr(const std::string &s = std::string()) :ps(new std::string(s)), i(0) {}
	HasPtr(const HasPtr& has,int s=1) :ps(new std::string(*has.ps+"2")),i(has.i) {}
	//随机分配一个新的string地址给ps，该地址指向被拷贝的has的string+"2"
	//而如果是编译器自己合成的拷贝构造函数的话，只会拷贝ps本身，也就是说被拷贝与新的ps的值是相等的
	//如果加上不带默认值的int s，那么就不是拷贝构造函数了，就只能使用编译器自己提供的拷贝构造函数，效果是拷贝ps本身
	void pulicfuc() {};
	HasPtr& operator= (const HasPtr& hasptr) 
	{ 
		*ps = *hasptr.ps; //拷贝指向的对象 而不拷贝ps本身
		//ps = hasptr.ps; //这个的作用就跟默认合成的拷贝赋值运算符一样了
		i = hasptr.i; 
		std::cout << "我的拷贝赋值函数" << std::endl; 
		return *this; 
	}
	//void testmoren(std::string pss = *ps);//这会报错，因为非静态数据成员不能作为默认参数 而静态数据成员可以作为默认参数
	constexpr static double teststatic = 1.2; //
	static constexpr double teststatic2 = 2.1;//修饰词的先后顺序不会影响
	constexpr static double teststatic3 = 2.1;//
	static const int teststatic4 = 1;//只有整形才能用static const来修饰并初始化，否则要想类内初始化必须static constexpr
	static const double teststatic5;
	static const std::vector<double> teststatic6;
	static const double teststatic7;
	static double teststatic8;
private:
	std::string *ps;
	int i;
	
};
const std::vector<double> HasPtr::teststatic6(10);
//const double HasPtr::teststatic5 = 1.2;
const static double teststatic = 4 ;

//constexpr static double HasPtr::teststatic = 1.2;//报错此处不能指定存储类 static只出现在类内部的声明语句
constexpr double HasPtr::teststatic;//即便类内静态数据成员被初始化了，也应该在外部定义一下该成员
//constexpr double HasPtr::teststatic = 1.5;//这样类内部已经初始化过了，会报错重定义，多次初始化。
//const double HasPtr::teststatic;// 由于内部是由constexpr声明的 因此不能用const声明

const double HasPtr::teststatic7 = 3.5;
//constexpr double HasPtr::teststatic7 = 3; //内部是由const声明的，外部这样定义的话报错重定义，不同的存储类

double HasPtr::teststatic8;


class HasPtrValue //行为像值的类
{
	friend void swap(HasPtrValue & h1, HasPtrValue& h2); //会在排序时被用到 提升效率
public:
	HasPtrValue(const string & s = string()) :ps(new string(s)), i(0) {}
	//经过测试，带有下边的默认参数的也能用作拷贝构造函数
	HasPtrValue(const HasPtrValue& hpv,int test = 3) :ps(new string(*hpv.ps)), i(hpv.i+ test) {}//最好在冒号之后初始化，可以免除执行默认初始化
	//HasPtrValue& operator=(const HasPtrValue & hpv)
	//{
	//	*ps = *hpv.ps;
	//	i = hpv.i;
	//	cout << "HasPtrValue& operator=(const HasPtrValue & hpv)" << endl;
	//	return *this;
	//}
	HasPtrValue(HasPtrValue&& hpv)noexcept
		:ps(hpv.ps),i(hpv.i)
	{
		hpv.ps = nullptr;
	}
	HasPtrValue& operator=(HasPtrValue hpv) 
		//值传递一个临时变量，然后swap 这样虽然相比较于上边注释掉的那个operator=多了一次拷贝构造函数，
		//但是都用了swap，不会存在一段代码段重复在多处用的情况
	{
		swap( *this,hpv); //拷贝并交换 技术
		cout << "HasPtrValue& operator=(HasPtrValue hpv)" << endl;
		return *this;
	}
	//HasPtrValue& operator=(HasPtrValue&& hpv) //这两个operator同时出现的话会报错operator=不明确，因为对于右值变量都能调用
	//{
	//	swap(*this, hpv);
	//	return *this;
	//}


	bool operator<(const HasPtrValue& h2)const
	{
		return *ps < *h2.ps;
	}
	~HasPtrValue()
	{
		delete ps;
	}
private:
	string * ps;
	int i;
};
void swap(HasPtrValue & h1, HasPtrValue& h2) //避免了默认情况会声明一个temp然后拷贝构造进行内存分配耗费的时间
{
	//using std::swap;
	swap(h1.ps, h2.ps);
	swap(h1.i, h2.i);
	cout << "swap(HasPtrValue & h1, HasPtrValue& h2)" << endl;
}

class HasPtrptr //行为像指针的类 有点智能指针的意思
{
public:

	explicit HasPtrptr(const string & s = string()) :ps(new string(s)), i(1) 
	{
		refcount[ps]++;
	}
	HasPtrptr(const HasPtrptr& hpp) //引入一个指向相同地址的对象
	{
		refcount[hpp.ps]++;
		ps = hpp.ps;
		i = hpp.i;
	}
	HasPtrptr& operator=(const HasPtrptr & hpp)
	{
		refcount[hpp.ps]++;

		if (--refcount[ps] == 0)
		{
			auto it = refcount.find(ps);
			delete it->first;
			refcount.erase(it);
		}

		ps = hpp.ps;
		i = hpp.i;
		return *this;
	}
	~HasPtrptr()
	{
		if (--refcount[ps] == 0)
		{
			auto it = refcount.find(ps);
			delete it->first;
			refcount.erase(it);
		}		
	}

private:
	string * ps;
	int i;
	static map<string*,int> refcount ;//定义地址到引用数的映射 也就是引用计数 
	//实际上用动态分配的指针可以实现同样的效果，操作指针指向的数的改变可以分布式共享 否则如果是值的话，不会进行分布式共享
};
map<string*, int> HasPtrptr::refcount ;
