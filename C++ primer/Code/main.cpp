#include <iostream>
#include "main.h"
#include "another.h"
#include "CLASS.h"
#include "CP5_ex7_41.h"
#include "CP5_ex7_53.h"
#include "CP5_ex13_5.h"
#include "CP5_ex7_57.h"
#include "CP5_ex13_18.h"
#include "CP5_ex12_2.h"
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

namespace n13_13
{
	struct X {
		X() { std::cout << "X()" << std::endl; }
		X(const X&) { std::cout << "X(const X&)" << std::endl; }
		X& operator=(const X&) { std::cout << "X& operator=(const X&)" << std::endl; return *this; }
		~X() { std::cout << "~X()" << std::endl; }
	};

	void f(const X &rx, X x)
	{
		std::vector<X> vec;
		vec.reserve(2);
		vec.push_back(rx);
		vec.push_back(x);
	}
}

namespace ex2_33
{
	int i = 0, &r = i;
	auto a = r;
	const int ci = i, &cr = ci;
	auto b = ci;
	auto c = cr;//忽略顶层const
	auto d = &i;
	auto e = &ci;//保留底层const
	const auto f = ci;//忽略顶层const 需要显示的加上const
	auto &g = ci;
	//auto &h = 42;//右边这是一个顶层const auto会忽略掉顶层const 这个会报错

	const int i35 = 42;
	auto j = i35; const auto &k = i35; auto *p = &i35;
	const auto j2 = i35, &k2 = i35;

	decltype (i35) s = 0;//如果使用的表达式是一个变量，则decltype返回该变量的类型（包括顶层const和引用在内）
	//引用从来都是作为其所指对象的同义词出现，只有decltype例外
	const int &iii = 2;
	decltype (iii) sad = i35;
}

namespace ex2_36
{
	int a = 3, b = 4;
	decltype(a) c = a;
	decltype((b)) d = a;

	int a1 = 5;
	int a2 = a1;//初始化变量在编译期就能够确定值了。

	auto a3 = a1;
	decltype(a1) a4 = a1; //这两个类型一样

	const int a5 = 6;
	auto a6 = a5;
	decltype(a5) a7 = a5; //这两个类型就不一样了
}

namespace ex6_54
{
	int fucplus(int a, int b) { return a + b; }
	int fucminus(int a, int b) { return a - b; }
	int fucmultiple(int a, int b) { return a*b; }
	int fucdivid(int a, int b) { return a / b; }

	vector<int(*)(int, int)> vec_pfuc = {fucplus,fucminus,fucmultiple,fucdivid};
	void testvec_pfuc(int a,int b)
	{
		for (int i = 0; i < vec_pfuc.size(); i++)
		{
			std::cout << vec_pfuc[i](a, b) << std::endl; 
			//比如用在足式机器人步态序列的生成上，核心就是一行步态生成代码，但是有其他的循环调用语句
			//如果想要弄不同的步态，如果不是像现在这样调用的话需要写好几个重复度很高的函数
			//但是有了这个方法，就可以极大地简化代码量。
		}
	}
	decltype(testvec_pfuc(1,2)) s();//decltype返回假定调用这个函数后返回的类型
	decltype(testvec_pfuc) s1;//decltype返回函数名对应的函数类型，其实本质上是返回一个变量的类型
	//s1 为void(int,int)类型的函数类型
	using Fp = int(*)(int, int);
	using F = int(int, int);

	Fp returnP2fuc(F fucname)//自动转化为函数指针
	{
		return fucname; //返回一个函数指针 不能返回函数类型
	}
	int(*sss(F fucname))(int, int) //这样是可以的
	{
		return fucname;
	}
	//int(*)(int, int) ssss(F fucname) //这样会报错
	//{

	//}
	int useFp(F fucname,int a,int b)
	{
		return returnP2fuc(fucname)(a, b);
	}
}

namespace ex12_2
{
	vector<int>ss(10);
	//ss[1] = 3; //这样是会报错的 不能在namespace中赋值
}
void main()
{
	try
	{
		////////////////////////////////////////////////////////////////////////////////////////////
		//练习12.2
		cout << ex12_2::ss[0] << endl;
		ex12_2::ss[1] = 2;
		cout << ex12_2::ss[1] << endl;

		StrBlob StrBlob1;
		StrBlob1.push_back("ssss");
		StrBlob1.front() = "ssss1";
		
		const StrBlob StrBlob2({"uyffhgf","sfafag"});
		//StrBlob2.front() = "uyffhgfmodify";//因为是const，所以不能修改元素 但若将重载的front返回类型的const去掉，就能修改了，虽然不会报错但与用户初衷相悖
		cout << StrBlob2[1] << endl;
		//StrBlob2[1] = 1; //这会报错，因为已经在成员函数中重载了只读类型

		StrBlob StrBlob3 = StrBlob2; //这会调用编译器默认合成的拷贝构造函数
		StrBlob3.push_back("FASFASF");//StrBlob2是const类型的，意味着不能通过StrBlob2改变共享区域的数据，而能通过其他Blob改变共享区域的数据


		////////////////////////////////////////////////////////////////////////////////////////////
		



		////////////////////////////////////////////////////////////////////////////////////////////
		//练习6.54
		ex6_54::testvec_pfuc(50, 2);
		ex6_54::useFp(ex6_54::fucminus, 48, 3);
		ex6_54::useFp(ex6_54::fucmultiple, 48, 3);
		cout << ex6_54::returnP2fuc(ex6_54::fucmultiple) << endl;
		int i = 1;
		cout << &i << endl; //函数的指针和变量的指针占用的内存是一样的
		ex6_54::Fp s0 = ex6_54::fucminus;
		//ex6_54::F s1 = ex6_54::fucminus;//会报错，因为不能直接给函数类型赋值 没有定义这样的类型的值是什么，不想函数指针那样
		//当我们把函数名作为一个值使用时，该函数自动的转化成指针
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习2.36
		++ex2_36::c;
		++ex2_36::a;
		++ex2_36::d;
		++ex2_36::a1;
		++ex2_36::a2;
		int e236 = 3;
		int e237 = e236;
		++e236;
		++e237;
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习2.33
		ex2_33::a = 42;
		ex2_33::b = 42;
		ex2_33::c = 42;
		//ex2_33::d = 42; //推断是int*类型的，会报错无法将int类型的值分配到int *类型
		//ex2_33::e = 42;	//推断是const int*类型的，会报错无法将int类型的值分配到const int *类型
		//ex2_33::f = 42;	//报错表达式必须是可修改的左值。
		
		int * s = nullptr;
		bool(*p)(const char a, const char b);
		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习13.18
		Employee Employee1("wzk1");
		//Employee Employee2 = Employee1;//会报错拷贝构造函数是被删除的函数。
		Employee Employee3("wzk3");
		//Employee3 = Employee1;//本身合成的拷贝赋值函数就是删除的



		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习13.11 13.13
		HasPtr hasptr1311 = HasPtr("2");//又略去了拷贝构造函数

		HasPtr("3");
		cout << *psstore << endl; 
		//上一步中创建的临时变量被销毁时，会调用析构函数，析构函数中应该加一个delete动态分配的对象
		//因为当指向一个对象的引用或指针离开作用域时，析构函数不会执行
		//且因为隐式销毁一个内置指针类型的成员不会delete它所指向的对象
		//当显示的加入delete ps时，上述cout会出错。

		n13_13::X *px = new n13_13::X;
		n13_13::f(*px, *px);
		delete px;
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//练习7.57
		Account("wzk0");
		Account("wzk1");
		Account("wzk2");
		Account Account1("wzk3");
		Account Account2 = "232"; 
		//给构造函数加上explicit后，本语句就非法了
		//这个跳过了拷贝构造函数，本来应该是先通过字符串输入的构造函数构造一临时变量，
		//然后在通过拷贝构造函数创建对象的，这样的话NumID++2，但实际上是++1,因此就是跳过了拷贝构造函数
		//但跳过拷贝构造函数不是所有编译器都必须实现的事项，因此，为了避免numID++2，把字符串输入的构造声明为explicit
		//以避免隐式转换造成numID分配浪费
		Account Account3 = Account2;

		Account Account4("Account4");
		Account4 = Account1;
		long n = Account::read_NumID();
		Account Account5("Account5");
		Account &Account6 = Account5;
		Account &Account7 = Account4;
		Account6 = Account7;

		int intnum00 = 2;
		int intnum000 = 3;
		int &intnum01 = intnum00;
		int &intnum02 = intnum000;
		intnum01 = intnum02;
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//练习7.58
		const vector<double> vectordouble1;
		//vectordouble1.push_back(2.1); // 会报错 因为不能往一个const类型的容器中加东西

		HasPtr hs0;
		const vector<double> &vectordouble3 = hs0.teststatic6;
		const vector<double> &vectordouble2 = HasPtr::teststatic6;
		cout << HasPtr::teststatic << endl;
		cout << teststatic << endl;

		cout << HasPtr::teststatic7 << endl;

		HasPtr::teststatic8 = 2;

		//cout << HasPtr::teststatic5 << endl; //如果定义了static，但是没有初始化，会报错无法解析的外部命令

		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//练习13.5
		HasPtr hasptr0("hasptr0");
		HasPtr hasptr1 = hasptr0; 
		//由于自定义拷贝构造函数的缘故，导致hasptr1.ps值不等于hasptr0.ps，
		//指向的是一个经过加工的string，这样可以明确地与编译器合成的进行比较
		//当不自定义拷贝构造函数时，用编译器自己的，效果是拷贝地址本身
		HasPtr hasptr1_1 = 5;
		//跳过了拷贝构造函数，并没有执行自定义的拷贝构造函数，而是直接调用入口参数为int的构造函数
		//如果没跳过的话，应该+"2"的，但是没有加
		//这也是符合逻辑的，因为我本意是想把5对应的对象赋给hasptr1_1，已经执行了一遍构造了，就不必要再执行一遍
		HasPtr hasptr1_2 = hasptr1_1;//这就用的是自定义的拷贝构造函数，会+"2"
		HasPtr hasptr1_3(6, 3); //并不会影响拷贝构造函数作为正常构造函数进行最优参数匹配发挥其功能
		HasPtr hasptr2("ptr2");
		(hasptr2 = hasptr0).pulicfuc();//这个是拷贝赋值运算符
		//若是默认合成的拷贝赋值运算符则完全与hasptr0相等,但是对象本身地址不同，因而就叫“拷贝”
		//若是自定义的，就是自定义的那些操作，但有个风险是，需要自己控制所有成员变量的拷贝赋值，如有遗漏，则该遗漏的成员变量保持不变
		cout << &hasptr0 << " " << &hasptr2 << "大小是" << sizeof(&hasptr2)<< endl;
		int intvalue = 0;
		int *intvalueadd = &intvalue;
		cout << intvalueadd << "大小是"<<sizeof(intvalueadd)<<endl; 
		//通过对比输出的地址格式，以及地址的大小，发现指向不同对象的指针所占内存空间是一样的
		////////////////////////////////////////////////////////////////////////////////////////////
		
		
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
	
	system("pause");
}