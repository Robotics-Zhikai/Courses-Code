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

void NotThrow() throw()
{
	throw exception("sss") ;
}

int & CastConstZK(const int &tmp)
{
	return const_cast<int &>(tmp); //��const�����׵��ˣ����Ƽ�����Ϊ��ڵĲ���Ӧ���ǲ��ɸı��
}

void main()
{
	try
	{
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