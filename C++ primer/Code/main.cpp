#include <iostream>
#include "main.h"
#include "another.h"
#include "CLASS.h"
#include "CP5_ex7_41.h"
#include "CP5_ex7_53.h"
#include "CP5_ex13_5.h"
#include "CP5_ex7_57.h"
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
	~Nodefault() { cout << "����" << endl; }
	Nodefault(const std::string &);
	Nodefault(int i):data(i){ cout << "ִ��ʵ�ʸɻ�ĺ�����" << endl; };
	Nodefault() :Nodefault(1) { cout << "ִ�е�ί�й��캯����" << endl; } //ί�й��캯��
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


void main()
{
	try
	{
		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ13.11 13.13
		HasPtr hasptr1311 = HasPtr("2");//����ȥ�˿������캯��

		HasPtr("3");
		cout << *psstore << endl; 
		//��һ���д�������ʱ����������ʱ���������������������������Ӧ�ü�һ��delete��̬����Ķ���
		//��Ϊ��ָ��һ����������û�ָ���뿪������ʱ��������������ִ��
		//����Ϊ��ʽ����һ������ָ�����͵ĳ�Ա����delete����ָ��Ķ���
		//����ʾ�ļ���delete psʱ������cout�����

		n13_13::X *px = new n13_13::X;
		n13_13::f(*px, *px);
		delete px;
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ7.57
		Account("wzk0");
		Account("wzk1");
		Account("wzk2");
		Account Account1("wzk3");
		Account Account2 = "232"; 
		//�����캯������explicit�󣬱����ͷǷ���
		//��������˿������캯��������Ӧ������ͨ���ַ�������Ĺ��캯������һ��ʱ������
		//Ȼ����ͨ���������캯����������ģ������Ļ�NumID++2����ʵ������++1,��˾��������˿������캯��
		//�������������캯���������б�����������ʵ�ֵ������ˣ�Ϊ�˱���numID++2�����ַ�������Ĺ�������Ϊexplicit
		//�Ա�����ʽת�����numID�����˷�
		Account Account3 = Account2;

		Account Account4("Account4");
		Account4 = Account1;
		long n = Account::read_NumID();
		Account Account5("Account5");
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ7.58
		const vector<double> vectordouble1;
		//vectordouble1.push_back(2.1); // �ᱨ�� ��Ϊ������һ��const���͵������мӶ���

		HasPtr hs0;
		const vector<double> &vectordouble3 = hs0.teststatic6;
		const vector<double> &vectordouble2 = HasPtr::teststatic6;
		cout << HasPtr::teststatic << endl;
		cout << teststatic << endl;

		cout << HasPtr::teststatic7 << endl;

		HasPtr::teststatic8 = 2;

		//cout << HasPtr::teststatic5 << endl; //���������static������û�г�ʼ�����ᱨ���޷��������ⲿ����

		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ13.5
		HasPtr hasptr0("hasptr0");
		HasPtr hasptr1 = hasptr0; 
		//�����Զ��忽�����캯����Ե�ʣ�����hasptr1.psֵ������hasptr0.ps��
		//ָ�����һ�������ӹ���string������������ȷ����������ϳɵĽ��бȽ�
		//�����Զ��忽�����캯��ʱ���ñ������Լ��ģ�Ч���ǿ�����ַ����
		HasPtr hasptr1_1 = 5;
		//�����˿������캯������û��ִ���Զ���Ŀ������캯��������ֱ�ӵ�����ڲ���Ϊint�Ĺ��캯��
		//���û�����Ļ���Ӧ��+"2"�ģ�����û�м�
		//��Ҳ�Ƿ����߼��ģ���Ϊ�ұ��������5��Ӧ�Ķ��󸳸�hasptr1_1���Ѿ�ִ����һ�鹹���ˣ��Ͳ���Ҫ��ִ��һ��
		HasPtr hasptr1_2 = hasptr1_1;//����õ����Զ���Ŀ������캯������+"2"
		HasPtr hasptr1_3(6, 3); //������Ӱ�쿽�����캯����Ϊ�������캯���������Ų���ƥ�䷢���书��
		HasPtr hasptr2("ptr2");
		(hasptr2 = hasptr0).pulicfuc();//����ǿ�����ֵ�����
		//����Ĭ�ϺϳɵĿ�����ֵ���������ȫ��hasptr0���,���Ƕ������ַ��ͬ������ͽС�������
		//�����Զ���ģ������Զ������Щ���������и������ǣ���Ҫ�Լ��������г�Ա�����Ŀ�����ֵ��������©�������©�ĳ�Ա�������ֲ���
		cout << &hasptr0 << " " << &hasptr2 << "��С��" << sizeof(&hasptr2)<< endl;
		int intvalue = 0;
		int *intvalueadd = &intvalue;
		cout << intvalueadd << "��С��"<<sizeof(intvalueadd)<<endl; 
		//ͨ���Ա�����ĵ�ַ��ʽ���Լ���ַ�Ĵ�С������ָ��ͬ�����ָ����ռ�ڴ�ռ���һ����
		////////////////////////////////////////////////////////////////////////////////////////////
		
		
		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ7.53
		Debug Debug0;
		Debug Debug1(true,false,true);
		Debug Debug2(2, 3, 1);

		double debuga=3, debugb=2, debugc=1;
		Debug Debug3(debuga, debugb, debugc);
		//Ҫ�뷢��constexpr�����ԣ���������������ʱǰ�߼���constexpr��
		//����������ӵĻ������ǿ���ͨ��������ֵ�ģ�constexpr��ͬ����
		cout << Debug3.get_bw();
		Debug3.set_bw(0);  //�����ǿ����޸ĵ�
		cout << Debug3.get_bw();

		//constexpr Debug Debug4_1(debuga, debugb, debugc); //�ᱨ����ʽ�б��뺬�г���
		constexpr Debug Debug4(3, 2, 1); //�����ɳ�����ֵ�󣬾���ȷ�ˡ�
		Debug4.get_bw();
		//Debug4.set_bw(0); //����Ǳ���ģ���ΪDebug4������֤�����ڲ���ֵ�ڱ���ʱ���Ѿ�ȷ������������ʱ�ǲ��ܸ��
		
		Debug Debug5(true);
		bool boolva = 1;
		Debug Debug6(boolva);
		const Debug Debug7(true);//�����ڱ����ھ�ȷ��ֵ�ĳ��� ��constexprûʲô��ϵ
		//constexpr Debug Debug8(true);//�������ʽ���뺬�г���ֵ
		//�ڹ��캯���У���û�ж�Debug(bool b ) :bw(b), io(b), other(b)������캯������constexpr
		//������ڹ��캯����������constexpr�����ϱ���һ�в�������ζ�ſ��Թ���constexpr����

		//���ϣ�constexpr��һ���ȽϹ㷺��ѡ����Խ�һ���������밲ȫ��Ҳ������һ��������constexpr������ͨ��������
		////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ7.49
		Sales_data Sales_data0("333");
		Sales_data & Sales_data1 = Sales_data0.returnRef(Sales_data("333"));
		//��������һ����ʱ���������ã���Ȼ���ᱨ������Sales_data1��ֵ��Ĭ�Ϲ��칹������ģ�û��"333"
		//�ϱ�����ƺ��´������һ�� Ŀǰ�������Ϊʲô����һ���ֲ����û�work
		Sales_data & Sales_data2 = Sales_data0.returnRef(Sales_data0);
		//��������Sales_data0�����ã���Ϊ�䲻��һ����ʱ�����������ʵֵȷʵ��"333"

		Sales_data Sales_data3;
		Sales_data & Sales_data4 = Sales_data0.combine(Sales_data3);

		Sales_data Sales_data5("345");
		Sales_data Sales_data6 = Sales_data0.returnValue(Sales_data5);

		double sad = 4;
		//�������explicit����ô�����ܰ����±ߵĲ���������ʽת��
		Sales_data0.combine(3.3);//�Զ���ʽת������һ��const Sales_data
		Sales_data0.combine(sad);//��ʱ����һ����const�͵� ��Ϊ������double�ͱ���������ʽת�������õĹ��캯������const����
		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		int n1118 = 1;
		int &n11181 = n1118;
		int &n11182 = n11181;
		n1118 = 3; //���Զ����ö������

		Nodefault moren;
		const Nodefault &moren1 = moren.returnRef(moren);
		const Nodefault &moren2 = moren.returnRef(2);
		//��������ʽת��Ϊ����ʱ���࣬���û���������Ϊ�����Ĺ��캯���Ļ��ᱨ��û���ʵ��Ĳ�������ת��

		//Nodefault & moren3 = moren.returnrefnotconst(3);
		////��ʽ�ǻᱨ��ģ���������ǿ��޸ĵ���ֵ��
		////��C++primer P55��֪���������ÿ��԰�����ʱ���ϣ����Ƿǳ����ò����԰�����ʱ���ϡ�
		////��ʽ������ʽ������һ��ʱ����Ȼ������������ʱ������һ�ǳ��������ϣ����ǷǷ���

		Nodefault moren5;
		Nodefault & moren6 = moren.returnrefnotconst(moren5); //�����Ϳ����� ���ǳ������ð����˱�����

		moren5.data++;
		const Nodefault&ref2tmp = Nodefault(6); 
		//���ڴ����еģ���Ϊ���԰󶨳����ã�û�е����������� �����һ�����ɼ���ʱ�����ͷ����������
		Nodefault(6); //��ʱ����������������Ͳ��ɷ�����, �ͷ����ڴ���,��������������

		Nodefault(6).returnRef(moren1); //��ʱ����ֻ����ֻ����
		Nodefault(6).returnrefnotconst(moren5).data++; //���ǰ�moren5��data++��һ��
		//Nodefault(6).returnRef(moren1).data++; //����const�����ƣ�ʹ�ò��ܱ��޸�
		//Nodefault(6).data++; //������ʽ�����ǿ��޸ĵ���ֵ������Ϊ����ʱ���������ܱ��޸�

		////////////////////////////////////////////////////////////////////////////////////////////

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
	
	system("pause");
}