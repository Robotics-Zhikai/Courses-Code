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
	screen() = default; //加上这个即便有其他构造函数也可以进行编译器自己合成的默认构造函数
	screen(int data1 = 5) :data(data1){} //在有screen()=default存在的情况下 直接声明时会不知道到底该调用哪个函数
	//同时有这两个构造函数的写法是非常错误的，因为默认构造函数不知道到底该调哪个 但是编译通过是没有问题的

	screen(int s,int s1) :data(1), DATA2(3), DATA1(DATA2) {} //这样初始化是错误的，由于成员初始化是有顺序的
	//这样初始化的话，会出现变量内存储的是垃圾值
	screen & display()
	{
		return *this;
	}
	screen(std::istream & is) { int x; is >> x; }
	//const screen & display() const { return *this; }
	screen display() const { return *this; }
	void changedata(int s) { data=s ; }
	int data = 6;
private:
	int DATA1 = 7;
	int DATA2 = 8;
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

void NotThrow() throw()
{
	throw exception("sss") ;
}

int & CastConstZK(const int &tmp)
{
	return const_cast<int &>(tmp); //把const属性抛掉了，不推荐，因为入口的参数应该是不可改变的
}



typedef string Type;
Type initVal();
class Excercise 
{
public :
	//typedef double Type; //如果加在这里的话就可以隐藏了外层的typedef
	Type SetVal(Type parm);
	Type initVal() { ::Type s; return s; };
private:
	int val;
	typedef double Type;
	Type nima; //重定义名字 是不允许的 但是编译器不会报错 
};

//Excercise::Type initVal() //加在这里的话由于Type是private 不可访问 但是如果再加一个Excercise限定符 就可以访问了
//{
//
//}

Type Excercise::SetVal(::Type parm) //Type是string类型的
{
	::Type str; //必须得加全局域，否则与类中的函数是不兼容的。
	initVal();
	return str;
}

typedef int Notclass;
void func()
{
	//在普通函数里 这样是合法的 但是在类中 类似这样的用法会由于类先处理声明再处理定义的特殊设定而导致非法。
	Notclass s1 = 2;
	typedef double Notclass;
	Notclass s = 1;
}


class Nodefault
{
public:
	~Nodefault() { cout << "析构" << endl; }
	Nodefault(const std::string &);
	Nodefault(int i):data(i){ cout << "执行实际干活的函数了" << endl; };
	Nodefault() :Nodefault(1) { cout << "执行到委托构造函数了" << endl; } //委托构造函数
	const Nodefault & returnRef(const Nodefault& constref) { return constref; }
	Nodefault & returnrefnotconst(Nodefault & ref) { return ref; }
	int data;
};
class C
{
public:
	C() : data() {}
//public:
private:
	Nodefault data;
};


void main()
{
	try
	{
		////////////////////////////////////////////////////////////////////////////////////////////
		int n1118 = 1;
		int &n11181 = n1118;
		int &n11182 = n11181;
		n1118 = 3; //可以对引用多次命名

		Nodefault moren;
		const Nodefault &moren1 = moren.returnRef(moren);
		const Nodefault &moren2 = moren.returnRef(2);
		//把整数隐式转换为了临时的类，如果没有输入参数为整数的构造函数的话会报错没有适当的参数类型转换

		//Nodefault & moren3 = moren.returnrefnotconst(3);
		////上式是会报错的，报错必须是可修改的左值。
		////由C++primer P55可知道，常引用可以绑定在临时量上，但是非常引用不可以绑定在临时量上。
		////上式是先隐式构造了一临时量，然后期望将该临时量绑定在一非常量引用上，这是非法的

		Nodefault moren5;
		Nodefault & moren6 = moren.returnrefnotconst(moren5); //这样就可以了 将非常量引用绑定在了变量上

		moren5.data++;
		const Nodefault&ref2tmp = Nodefault(6); 
		//在内存中有的，因为可以绑定常引用，没有调用析构函数 结合下一条语句可见临时量的释放与否是灵活的
		Nodefault(6); //临时变量出了这条语句后就不可访问了, 释放了内存了,调用了析构函数

		Nodefault(6).returnRef(moren1); //临时变量只能是只读的
		Nodefault(6).returnrefnotconst(moren5).data++; //就是把moren5的data++了一下
		//Nodefault(6).returnRef(moren1).data++; //由于const的限制，使得不能被修改
		//Nodefault(6).data++; //报错表达式必须是可修改的左值，是因为是临时变量，不能被修改

		////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////
		C ctmp;
		cout << endl;
		vector<C> tmpvectornodefault(10,ctmp);
		Nodefault nodefaulttmp;
		vector<Nodefault> tmpvectornodefault1(10, nodefaulttmp);
		cout << endl;
		vector<Nodefault> tmpvectornodefault2(10); //这样的话是构造十次
		cout << endl;
		vector<Nodefault> tmpvectornodefault3(10, Nodefault(1)); //这样的只构造一次，然后复制十次
		////////////////////////////////////////////////////////////////////////////////////////////



		////这下边这个本来应该是未定义的行为的，但是可能VS2015编译器有一些优化，就可以对声明为const的值进行修改
		////但我认为，未定义的行为可能是本来声明为const，但是有这些行为使得const的值变化了，造成难以预计的程序运行后果
		////意味着程序具有设计缺陷，编译可能在所有平台都是可以通过的，只不过与标准相悖
		//const int j = 3; // j 声明为const常量
		//int* pj = const_cast<int*>(&j);
		//*pj = 4;         // 用去const的指针去修改一个原本就是常量的j的值，undefined behavior!

		const int intnum = 5;
		const int intconst1 = intnum;
		//intconst1 = 34; //报错必须是可修改的左值，可以通过引用的const_cast来修改 但是很不推荐！
		//const_cast<int> (intconst1) //报错必须是指针、引用 因为只能修改底层const属性
		CastConstZK(intconst1) = 34; //这样一下就把本来应该是const的值给修改了


		const int * intconst = &intnum;
		const int & ref = intnum;
		//const int & &ss = ref; //这样就会报错不能使用对引用的引用
		const int & ss = ref; //这样不会报错对引用的引用
		const_cast<int &>(ref) = 100;
		//int * intnotconstnotcast = intconst;
		*(const_cast<int *>(intconst)) =1;
		int * intnotconst = const_cast <int *> (intconst);
		*intnotconst = 6;

		//NotThrow();
		vector<int> vectorint(1,1);
		vectorint.push_back(10);
		const screen constscreen(5);
		screen tmpscreen2(3,4);
		//screen tmpscreen3; //此时会报错包含多个默认构造函数

		/*screen tmpscreen4(); //此时会报错必须定义类类型，因为他把tmpscreen4()看成了函数声明
		cout << tmpscreen4.data << endl;*/

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
	catch (exception d)
	{
		cout << d.what();
	}
	

}