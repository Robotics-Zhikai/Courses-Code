#include <iostream>
#include "main.h"
#include "another.h"
#include "CLASS.h"
#include "CP5_ex7_41.h"
#include "CP5_ex7_53.h"
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
		//练习7.53
		Debug Debug0;
		Debug Debug1(true,false,true);
		Debug Debug2(2, 3, 1);

		double debuga=3, debugb=2, debugc=1;
		Debug Debug3(debuga, debugb, debugc);
		//要想发挥constexpr的属性，必须在声明变量时前边加上constexpr。
		//否则如果不加的话，还是可以通过变量赋值的，constexpr形同虚设
		cout << Debug3.get_bw();
		Debug3.set_bw(0);  //这样是可以修改的
		cout << Debug3.get_bw();

		//constexpr Debug Debug4_1(debuga, debugb, debugc); //会报错表达式中必须含有常量
		constexpr Debug Debug4(3, 2, 1); //这样由常量赋值后，就正确了。
		Debug4.get_bw();
		//Debug4.set_bw(0); //这个是报错的，因为Debug4声明保证了其内部的值在编译时就已经确定，程序运行时是不能搞的
		
		Debug Debug5(true);
		bool boolva = 1;
		Debug Debug6(boolva);
		const Debug Debug7(true);//不是在编译期就确定值的常量 跟constexpr没什么关系
		//constexpr Debug Debug8(true);//报错：表达式必须含有常量值
		//在构造函数中，并没有对Debug(bool b ) :bw(b), io(b), other(b)这个构造函数声明constexpr
		//而如果在构造函数中声明了constexpr，则上边这一行不报错。意味着可以构造constexpr类型

		//综上，constexpr是一个比较广泛的选项，可以进一步提升代码安全性也可以退一步不声明constexpr当成普通的来处理
		////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////
		//练习7.49
		Sales_data Sales_data0("333");
		Sales_data & Sales_data1 = Sales_data0.returnRef(Sales_data("333"));
		//期望返回一个临时变量的引用，虽然不会报错，但是Sales_data1的值是默认构造构造出来的，没有"333"
		//上边这个似乎会拷贝构造一个 目前还不清楚为什么返回一个局部引用还work
		Sales_data & Sales_data2 = Sales_data0.returnRef(Sales_data0);
		//期望返回Sales_data0的引用，因为其不是一个临时变量，因此真实值确实是"333"

		Sales_data Sales_data3;
		Sales_data & Sales_data4 = Sales_data0.combine(Sales_data3);

		Sales_data Sales_data5("345");
		Sales_data Sales_data6 = Sales_data0.returnValue(Sales_data5);

		double sad = 4;
		//如果加上explicit，那么将不能按照下边的操作进行隐式转化
		Sales_data0.combine(3.3);//自动隐式转化成了一个const Sales_data
		Sales_data0.combine(sad);//临时变量一定是const型的 因为即便是double型变量进行隐式转换，调用的构造函数都是const类型
		////////////////////////////////////////////////////////////////////////////////////////////


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