#pragma once
#include "main.h"
using namespace std;
class Quote
{
	friend double print_total(ostream &os, const Quote& item, size_t n);
public:
	Quote() = default;
	Quote(const string& bn, double pr) :bookNo(bn),prize(pr){}
	string ISBN()const { return bookNo; } //ISBN不能被改变

	void testnotvirtual(int n)
	{
		cout << "ss" << endl;
	}
	virtual ~Quote() = default; // 根节点定义一个虚析构函数
	virtual double net_prize(size_t n)const //意味着派生类需要override这个函数 因为派生类有可能改变了prize的值
	{
		return prize * n;
	}
	virtual void testvirtual(int n = 9)const 
	{
		cout << n << endl;
	}
	virtual void debug()const 
	{
		cout << "bookNo:" << bookNo << endl;
		cout << "prize:" << prize << endl;
	}
private:
	string bookNo;
protected:
	double prize = 0.0;
};

//计算并打印销售给定数量的某种书籍所得费用
double print_total(ostream &os, const Quote& item, size_t n)
//这个其实不用friend也可以,因为都是调用的public封装
{
	item.testvirtual(); 
	//由于是通过基类的引用调用的带有默认参数的虚函数，因此不管派生类的默认参数值是如何定义，都认为默认参数的值是基类的默认参数值
	//可以这么理解：item是一个基类的引用，该引用绑定在派生类上，单独从派生类中剥离出来了基类，调用带有默认参数有调用静态类型的含义，因此调用基类的默认参数
	//但是上边的理解方法不适用于动态类型
	os << item.ISBN() << endl;
	double ret = item.net_prize(n); //根据动态类型来调用，item可能是Quote基类，也可能是任意其派生类
	os << "该书卖" << n << "本的价格是" << ret << endl;
	return ret;
}

class Bulk_quote:public Quote
{
	friend double print_total(ostream &os, const Quote& item, size_t n);
public:
	Bulk_quote() = default;
	Bulk_quote(const string & str, double pri,size_t minct,double dc ):Quote(str,pri),min_qty(minct),discount(dc){}

	void testnotvirtual()
	{
		cout << "ss1" << endl;
	}
	void testvirtual(int n = 19)const override
	{
		cout << n << endl;
	}

	double net_prize(size_t n,double n1)const //这个单纯的当成了派生类的成员函数
	{
		if (n >= min_qty)
			return n*prize*(1 - discount);
		else
			return n*prize;
	}
	double net_prize(size_t n/*,int sfag*/)const override //只有override能得到覆盖的保证 否则用virtual是不能得到覆盖的保证的
		//override不是必须的，如果参数与基类完全相同且不加override，那么默认是覆盖了基类的同名函数的
		//但是当可能写错时，比如写成了上边的两个入口参数，与需要覆盖的基类虚函数不匹配，那么加上override后就能报错
		//指明现在写错了
	{
		if (n >= min_qty)
			return n*prize*(1 - discount);
		else
			return n*prize;
	}
	void debug()const override
	{
		Quote::debug();
		cout << "min_qty:" << min_qty << endl;
		cout << "discount:" << discount << endl;
	}
private:
	size_t min_qty = 0; //使用折扣政策的最低购买量
	double discount = 0.0; //以小数表示的折扣额
};

class Strategy_ex15_7 :public Quote
{
public:
	Strategy_ex15_7() = default;
	Strategy_ex15_7(const string& bookname, double bookprize, size_t numup, double dc)
		:Quote(bookname, bookprize), numupper(numup), discount(dc) {}
	double net_prize(size_t n) const override
	{
		if (n <= numupper)
			return n*prize*(1 - discount);
		else
			return numupper*prize*(1 - discount) + (n - numupper)*prize;
	}
	void debug()const override
	{
		Quote::debug();
		cout << "numupper:" << numupper << endl;
		cout << "discount:" << discount << endl;
	}
private:
	size_t numupper = 0; // 定义一打折上界，超出该上界时超出部分按原价销售，否则打折销售
	double discount = 0.0; // 定义折扣额

};

class testclass :public Strategy_ex15_7 //间接继承Quote，可以访问所有子对象或子对象的子对象的public或protect对象
{
public:
	double teteag = prize;
	string adhADSF = ISBN();
	testclass(const string& bookname, double bookprize, size_t numup, double dc)
		:Strategy_ex15_7(bookname, bookprize, numup, dc) {}
	//test()
	//	:Quote("g",4) {}//报错不允许使用间接非虚拟虚类。
	//就像多米诺骨牌一样，只需要保证相邻两个类的构造关系，就可以把
	//整个继承链的类构造出来 也就是直接调用直接继承的构造函数

	double net_prize(size_t n,size_t j2) const //外界调用没有继承的单参数的net_prize，但是在print_total里调用的是Strategy_ex15_7::net_prize 不知为何？？
	{
		return 1;
	}
};