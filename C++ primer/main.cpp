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
	screen() = default; //��������������������캯��Ҳ���Խ��б������Լ��ϳɵ�Ĭ�Ϲ��캯��
	screen(int data1 = 5) :data(data1){} //����screen()=default���ڵ������ ֱ������ʱ�᲻֪�����׸õ����ĸ�����
	//ͬʱ�����������캯����д���Ƿǳ�����ģ���ΪĬ�Ϲ��캯����֪�����׸õ��ĸ� ���Ǳ���ͨ����û�������

	screen(int s,int s1) :data(1), DATA2(3), DATA1(DATA2) {} //������ʼ���Ǵ���ģ����ڳ�Ա��ʼ������˳���
	//������ʼ���Ļ�������ֱ����ڴ洢��������ֵ
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
	return const_cast<int &>(tmp); //��const�����׵��ˣ����Ƽ�����Ϊ��ڵĲ���Ӧ���ǲ��ɸı��
}



typedef string Type;
Type initVal();
class Excercise 
{
public :
	//typedef double Type; //�����������Ļ��Ϳ�������������typedef
	Type SetVal(Type parm);
	Type initVal() { ::Type s; return s; };
private:
	int val;
	typedef double Type;
	Type nima; //�ض������� �ǲ������ ���Ǳ��������ᱨ�� 
};

//Excercise::Type initVal() //��������Ļ�����Type��private ���ɷ��� ��������ټ�һ��Excercise�޶��� �Ϳ��Է�����
//{
//
//}

Type Excercise::SetVal(::Type parm) //Type��string���͵�
{
	::Type str; //����ü�ȫ���򣬷��������еĺ����ǲ����ݵġ�
	initVal();
	return str;
}

typedef int Notclass;
void func()
{
	//����ͨ������ �����ǺϷ��� ���������� �����������÷����������ȴ��������ٴ�����������趨�����·Ƿ���
	Notclass s1 = 2;
	typedef double Notclass;
	Notclass s = 1;
}


class Nodefault
{
public:
	Nodefault(const std::string &);
	Nodefault(int) { cout << "ִ��ʵ�ʸɻ�ĺ�����" << endl; };
	Nodefault() :Nodefault(1) { cout << "ִ�е�ί�й��캯����" << endl; } //ί�й��캯��
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
		C ctmp;
		cout << endl;
		vector<C> tmpvectornodefault(10,ctmp);
		Nodefault nodefaulttmp;
		vector<Nodefault> tmpvectornodefault1(10, nodefaulttmp);
		cout << endl;
		vector<Nodefault> tmpvectornodefault2(10); //�����Ļ��ǹ���ʮ��
		cout << endl;
		vector<Nodefault> tmpvectornodefault3(10, Nodefault(1)); //������ֻ����һ�Σ�Ȼ����ʮ��
		////////////////////////////////////////////////////////////////////////////////////////////



		////���±��������Ӧ����δ�������Ϊ�ģ����ǿ���VS2015��������һЩ�Ż����Ϳ��Զ�����Ϊconst��ֵ�����޸�
		////������Ϊ��δ�������Ϊ�����Ǳ�������Ϊconst����������Щ��Ϊʹ��const��ֵ�仯�ˣ��������Ԥ�Ƶĳ������к��
		////��ζ�ų���������ȱ�ݣ��������������ƽ̨���ǿ���ͨ���ģ�ֻ�������׼���
		//const int j = 3; // j ����Ϊconst����
		//int* pj = const_cast<int*>(&j);
		//*pj = 4;         // ��ȥconst��ָ��ȥ�޸�һ��ԭ�����ǳ�����j��ֵ��undefined behavior!

		const int intnum = 5;
		const int intconst1 = intnum;
		//intconst1 = 34; //��������ǿ��޸ĵ���ֵ������ͨ�����õ�const_cast���޸� ���Ǻܲ��Ƽ���
		//const_cast<int> (intconst1) //���������ָ�롢���� ��Ϊֻ���޸ĵײ�const����
		CastConstZK(intconst1) = 34; //����һ�¾Ͱѱ���Ӧ����const��ֵ���޸���


		const int * intconst = &intnum;
		const int & ref = intnum;
		//const int & &ss = ref; //�����ͻᱨ����ʹ�ö����õ�����
		const int & ss = ref; //�������ᱨ������õ�����
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
		//screen tmpscreen3; //��ʱ�ᱨ��������Ĭ�Ϲ��캯��

		/*screen tmpscreen4(); //��ʱ�ᱨ����붨�������ͣ���Ϊ����tmpscreen4()�����˺�������
		cout << tmpscreen4.data << endl;*/

		screen tmpscreen(2);
		screen tmpscreen1(3);
		tmpscreen1 = tmpscreen; //�ƺ��������Լ�������һ����ֵoperator
		constscreen.display() = tmpscreen; //��������� Ϊʲô��������ֵ ��Ϊ�������Ѹñ��ʽ����Ϊ��operator = 
										   //��effective C++���б���
		constscreen.display().changedata(100); // �����constscreen���κβ���

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