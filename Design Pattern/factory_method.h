#pragma once
#include"main.h"
//��������ģʽ
namespace factory_method
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
	OperationAdd(double num1, double num2) :operation_binary(num1, num2) {}
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

// �ڹ�������ģʽ�У��������ฺ���崴����Ʒ����Ĺ����ӿڣ�
//�����������������ɾ���Ĳ�Ʒ������������Ŀ���ǽ���Ʒ���ʵ���������ӳٵ�������������ɣ�
//��ͨ������������ȷ������Ӧ��ʵ������һ�������Ʒ�� 
class factory_base
{
public:
	virtual shared_ptr<operation_base> retoperation()const = 0;
};

class factory_base_unitary:public factory_base
{
public:
	virtual ~factory_base_unitary() = default;
	factory_base_unitary(double num) :num(num) {}
protected:
	double num;
};
class factory_base_binary :public factory_base
{
public:
	virtual ~factory_base_binary() = default;
	factory_base_binary(double num1, double num2) :num1(num1), num2(num2) {}
protected:
	double num1;
	double num2;
};

class factory_derive_add:public factory_base_binary
{
public:
	factory_derive_add(double num1, double num2) :factory_base_binary(num1, num2) {}
	shared_ptr<operation_base> retoperation()const override
	{
		//factory_base_binary sd(33,2);
		//sd.num1 = 3; 
		//����ͨ����������protected��Ա����Ϊ����൱���û��������protected��
		//�����ܱ����ĳ�Ա����ͨ��factory_base_binary��ָ���������
		return shared_ptr<operation_base>(new OperationAdd(num1, num2));
	}
};

class factory_derive_minus :public factory_base_binary
{
public:
	factory_derive_minus(double num1, double num2) :factory_base_binary(num1, num2) {}
	shared_ptr<operation_base> retoperation()const override
	{
		return shared_ptr<operation_base>(new OperationMinus(num1, num2));
	}
};

class factory_derive_multiple :public factory_base_binary
{
public:
	factory_derive_multiple(double num1, double num2) :factory_base_binary(num1, num2) {}
	shared_ptr<operation_base> retoperation()const override
	{
		return shared_ptr<operation_base>(new OperationMultiple(num1, num2));
	}
};

class factory_derive_divide :public factory_base_binary
{
public:
	factory_derive_divide(double num1, double num2) :factory_base_binary(num1, num2) {}
	shared_ptr<operation_base> retoperation()const override
	{
		return shared_ptr<operation_base>(new OperationDivide(num1, num2));
	}
};

class factory_derive_sqrt :public factory_base_unitary
{
public:
	factory_derive_sqrt(double num) :factory_base_unitary(num) {}
	shared_ptr<operation_base> retoperation()const override
	{
		return shared_ptr<operation_base>(new OperationSqrt(num));
	}
};

class factory_derive_sin :public factory_base_unitary
{
public:
	factory_derive_sin(double num) :factory_base_unitary(num) {}
	shared_ptr<operation_base> retoperation()const override
	{
		return shared_ptr<operation_base>(new OperationSin(num));
	}
};
/*
����ʹ�����������Ķ�̬�ԣ���������ģʽ�����˼򵥹���ģʽ���ŵ㣬
���ҿ˷�������ȱ�㡣�ڹ�������ģʽ�У����ĵĹ����಻�ٸ������в�Ʒ�Ĵ�����Ҳ����˵����򵥹�������case
���ǽ����崴��������������ȥ��������������������������幤������ʵ�ֵĽӿڣ�
����������һ����Ʒ�౻ʵ��������ϸ�ڣ���ʹ�ù�������ģʽ��������ϵͳ�ڲ��޸Ĺ�����ɫ������������²�Ʒ��
*/

/*
�ŵ�
1.�����������������ͻ�����Ҫ�Ĳ�Ʒ��ͬʱ����ͻ����������־����Ʒ�ཫ��ʵ������һϸ�ڣ�
�û�ֻ��Ҫ���������Ʒ��Ӧ�Ĺ�����������Ĵ���ϸ�ڣ���������֪�������Ʒ���������
2.���ܹ�ʹ������������ȷ���������ֲ�Ʒ���󣬶���δ�����������ϸ������ȫ��װ�ھ��幤���ڲ���
��������ģʽ֮�����ֱ���Ϊ��̬����ģʽ������Ϊ���еľ��幤���඼����ͬһ�����ࡣ
3.ʹ�ù�������ģʽ����һ���ŵ�����ϵͳ�м����²�Ʒʱ�������޸ĳ��󹤳��ͳ����Ʒ�ṩ�Ľӿڣ�
�����޸Ŀͻ��ˣ�Ҳ�����޸������ľ��幤���;����Ʒ����ֻҪ���һ�����幤���;����Ʒ�Ϳ����ˡ�
������ϵͳ�Ŀ���չ��Ҳ�ͱ�÷ǳ��ã���ȫ���ϡ�����ԭ�򡱡�
*/

/*
ȱ��
1.������²�Ʒʱ����Ҫ��д�µľ����Ʒ�࣬���һ�Ҫ�ṩ��֮��Ӧ�ľ��幤���࣬
ϵͳ����ĸ������ɶ����ӣ���һ���̶���������ϵͳ�ĸ��Ӷȣ��и��������Ҫ��������У�
���ϵͳ����һЩ����Ŀ�����
2.���ڿ��ǵ�ϵͳ�Ŀ���չ�ԣ���Ҫ�������㣬
�ڿͻ��˴����о�ʹ�ó������ж��壬������ϵͳ�ĳ����Ժ�����Ѷȣ�
����ʵ��ʱ������Ҫ�õ�DOM������ȼ�����������ϵͳ��ʵ���Ѷȡ�
*/
void test()
{
	shared_ptr<factory_base> fac;
	fac = shared_ptr<factory_base>(new factory_derive_add(1, 2));
	cout << fac->retoperation()->get_result() << endl;
	fac = shared_ptr<factory_base>(new factory_derive_minus(1, 2));
	cout << fac->retoperation()->get_result() << endl;
	fac = shared_ptr<factory_base>(new factory_derive_multiple(1, 2));
	cout << fac->retoperation()->get_result() << endl;
	fac = shared_ptr<factory_base>(new factory_derive_divide(1, 2));
	cout << fac->retoperation()->get_result() << endl;
	fac = shared_ptr<factory_base>(new factory_derive_sqrt(2));
	cout << fac->retoperation()->get_result() << endl;
	fac = shared_ptr<factory_base>(new factory_derive_sin(2));
	cout << fac->retoperation()->get_result() << endl;
	fac = shared_ptr<factory_base>(new factory_derive_divide(2,0));
	cout << fac->retoperation()->get_result() << endl;

}

//���ֳ��󻯵Ľ��ʹ���ֽṹ�����ڲ��޸ľ��幤���������������µĲ�Ʒ������򵥹���ģʽ���������ڹ��������޸�case
//��������µİ�ť���ͣ�ֻ��ҪΪ���������͵İ�ť����һ������Ĺ�����Ϳ��Ի�ø��°�ť��ʵ����
//��һ�ص�����ʹ�ù�������ģʽ���г�Խ�򵥹���ģʽ����Խ�ԣ����ӷ��ϡ�����ԭ�򡱡�


//https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/factory_method.html
//�����ģʽ

}