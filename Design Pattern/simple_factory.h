#pragma once
#include "main.h"
//�򵥹���ģʽ
namespace simple_factory
{

class operation_base
{
public:
	virtual double get_result() const = 0;
	virtual ~operation_base() = default;
};

class operation_unitary :public operation_base
{
protected:
	operation_unitary(double num) :num(num) {}
	double num;
};

class operation_binary :public operation_base
{
protected:
	operation_binary(double num1, double num2) :num1(num1), num2(num2) {}

	double num1;
	double num2;
};

class OperationAdd :public operation_binary
{
public:
	OperationAdd(double num1, double num2):operation_binary(num1,num2) {}
	double get_result()const override
	{
		return num1 + num2;
	}
};

class OperationMinus :public operation_binary
{
public:
	OperationMinus(double num1, double num2) :operation_binary(num1, num2) {}
	double get_result()const override
	{
		return num1 - num2;
	}
};

class OperationMultiple :public operation_binary
{
public:
	OperationMultiple(double num1, double num2) :operation_binary(num1, num2) {}
	double get_result()const override
	{
		return num1 * num2;
	}
};

class OperationDivide :public operation_binary
{
public:
	OperationDivide(double num1, double num2) :operation_binary(num1, num2) {}
	double get_result()const override
	{
		if (num2 == 0)
			throw exception("��ͼ����һ��Ϊ0����");
		else
			return num1 / num2;
	}
};

class OperationSqrt :public operation_unitary
{
public:
	OperationSqrt(double num) :operation_unitary(num) {}
	double get_result()const override
	{
		return sqrt(num);
	}
};

class OperationSin :public operation_unitary
{
public:
	OperationSin(double num) :operation_unitary(num) {}
	double get_result()const override
	{
		return sin(num);
	}
};
class factory
{
public:
	factory(double num1, double num2, string operation) :operation(operation) 
	{
		if (operation == "+")
			base = shared_ptr<operation_base>(new OperationAdd(num1, num2));
		else if (operation == "-")
			base = shared_ptr<operation_base>(new OperationMinus(num1, num2));
		else if (operation == "*")
			base = shared_ptr<operation_base>(new OperationMultiple(num1, num2));
		else if (operation == "/")
			base = shared_ptr<operation_base>(new OperationDivide(num1, num2));
		else
			throw exception("û�ж���ķ���");
	}
	factory(double num, string operation) :operation(operation)
	{
		if (operation == "sqrt")
			base = shared_ptr<operation_base>(new OperationSqrt(num));
		else if (operation == "sin")
			base = shared_ptr<operation_base>(new OperationSin(num));
		else
			throw exception("û�ж���ķ���");
	}
	double get_result()const 
	{
		return base->get_result();
	}
private:
	string operation;
	shared_ptr<operation_base> base = nullptr;
};

void test()
{
	factory f1(1, 2, "+");
	factory f2(1, 2, "-");
	factory f3(1, 2, "*");
	factory f4(1, 2, "/");
	factory f5(2, "sqrt");
	factory f6(1, 0, "/");
	factory f7(2, "sin");
	//����� https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/simple_factory.html
//�򵥵�����һ�����������ɷ���һ����Ӧ�Ķ��󣬲���Ҫ֪��������������
//�򵥹���ģʽר�Ŷ���һ���������𴴽��������ʵ������������ʵ��ͨ�������й�ͬ�ĸ��ࡣ
//������Ĵ����Ͷ�����ҵ���������Խ���ϵͳ����϶ȣ�ʹ�������޸�������������ס�
//�򵥹���ģʽ�����������ڹ������ְ����Թ��أ������µĲ�Ʒ��Ҫ�޸Ĺ�������ж��߼�����һ���뿪��ԭ������Υ���ġ�
//�򵥹���ģʽ��Ҫ�����ڣ�������Ҫʲô��ֻ��Ҫ����һ����ȷ�Ĳ������Ϳ��Ի�ȡ������Ҫ�Ķ��󣬶�����֪���䴴��ϸ�ڡ�
//�����ຬ�б�Ҫ���ж��߼������Ծ�����ʲôʱ�򴴽���һ����Ʒ���ʵ�����ͻ��˿������ֱ�Ӵ�����Ʒ��������Σ������������ѡ���Ʒ��
//�򵥹���ģʽͨ����������ʵ���˶����εķָ���ṩ��ר�ŵĹ��������ڴ�������
//ʹ�ü򵥹���ģʽ��������ϵͳ����ĸ�������һ��������������ϵͳ�ĸ��ӶȺ�����Ѷȡ�
//ϵͳ��չ���ѣ�һ������²�Ʒ�Ͳ��ò��޸Ĺ����߼����ڲ�Ʒ���ͽ϶�ʱ���п�����ɹ����߼����ڸ��ӣ�������ϵͳ����չ��ά����
	
	cout << f1.get_result() << endl;
	cout << f2.get_result() << endl;
	cout << f3.get_result() << endl;
	cout << f4.get_result() << endl;
	cout << f5.get_result() << endl;
	cout << f7.get_result() << endl;
	cout << f6.get_result() << endl;
}

//������ԭ����ͨ��
//�����ģʽ

}