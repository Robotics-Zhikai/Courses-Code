#pragma once


class Base
{
public:
	void pub_mem();
protected:
	int prot_mem;
private:
	char priv_mem;
};

class pub_derv :public Base
{
public:
	void memfcn(Base& b) { b = *this; } //不论继承控制是什么，在派生类中都可以访问基类的公有成员，因此可以派生类向基类转换
};
class priv_derv :private Base
{
public:
	void memfcn(Base &b) { b = *this; }
};
class prot_derv :protected Base
{
public:
	void memfcn(Base &b) { b = *this; }
};
class derived_from_public :public pub_derv //两个public继承，派生类肯定能访问到基类的公有成员
{
public:
	void memfcn(Base& b) { b = *this; }
};
class derived_from_private :public priv_derv //基于基类，第一个是private继承，那么基类的公有成员就成了private的了，再public后也不会访问到了 因此编译出错
{
	//void memfcn(Base& b) { b = *this; }//报错不允许对不可访问的基类进行访问
};
class derived_from_protected :public prot_derv //基于基类，第一个是protected继承，再public继承，基类的公有成员就是protected，在派生类中能够被访问到
{
public:
	void test()
	{
		pub_mem(); //protected类型，可以被派生类访问到
	}
	void memfcn(Base& b) { b = *this; }
};

