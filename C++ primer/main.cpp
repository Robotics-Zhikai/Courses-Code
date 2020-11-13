#include <iostream>
#include "main.h"
#include "another.h"
#include "CLASS.h"
using namespace std;

class Y;
class X
{
public:
	Y & Yreference(Y tmpY);
	X * dataX;
	int d;
	X & s;
};

class Y
{
	Y(X xdata)
	{
		Ydata = xdata.d;
	}
	int Ydata;
};

class screen
{
public:
	screen() = default;
	screen(int data1) :data(data1){}
	screen & display()
	{
		return *this;
	}
	//const screen & display() const { return *this; }
	screen display() const { return *this; }
	void changedata(int s) { data=s ; }
	int data;
};

namespace Exercise
{
	int ivar = 0;
	double dvar = 0;
	const int limit = 1000;
	float cout = 0;
}
namespace Exercise1
{
	int ivar = 1;
	double dvar = 1;
	const int limit = 10001;
	float cout = 1;
}

int ivar = 10;

int inlinetest()
{
	return 2;
}

void main()
{
	const screen constscreen(5);
	screen tmpscreen(2);
	screen tmpscreen1(3);
	tmpscreen1 = tmpscreen; //似乎编译器自己重载了一个赋值operator
	constscreen.display() = tmpscreen; //这个非引用 为什么还能做左值 因为编译器把该表达式翻译为了operator = 
	//在effective C++中有表述
	constscreen.display().changedata(100); // 不会对constscreen做任何操作

	cout << testinline() << endl;
	cout << inlinetest() << endl;
	
	int a = 5;
	int * Locate_a = &a;
	int ** locloc_a = &Locate_a;
	int *** loclocloc_a = &locloc_a;
	cout << Locate_a << endl;
	cout << locloc_a << endl;
	cout << loclocloc_a << endl;
	
	//using namespace Exercise;
	using Exercise::ivar;
	//using Exercise::dvar;
	using Exercise::limit;
	
	fuc();
	using Exercise::cout;
	std::cout << cout << endl;

	double dvar = 3.1415;

	int iobj = limit + 1;
	++ivar;
	++::ivar;

}