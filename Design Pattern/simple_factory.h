#pragma once
#include "main.h"
//简单工厂模式
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
			throw exception("试图除以一个为0的数");
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
			throw exception("没有定义的符号");
	}
	factory(double num, string operation) :operation(operation)
	{
		if (operation == "sqrt")
			base = shared_ptr<operation_base>(new OperationSqrt(num));
		else if (operation == "sin")
			base = shared_ptr<operation_base>(new OperationSin(num));
		else
			throw exception("没有定义的符号");
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
	//借鉴于 https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/simple_factory.html
//简单的输入一个参数，即可返回一个相应的对象，不需要知道具体的类的名字
//简单工厂模式专门定义一个类来负责创建其他类的实例，被创建的实例通常都具有共同的父类。
//将对象的创建和对象本身业务处理分离可以降低系统的耦合度，使得两者修改起来都相对容易。
//简单工厂模式最大的问题在于工厂类的职责相对过重，增加新的产品需要修改工厂类的判断逻辑，这一点与开闭原则是相违背的。
//简单工厂模式的要点在于：当你需要什么，只需要传入一个正确的参数，就可以获取你所需要的对象，而无须知道其创建细节。
//工厂类含有必要的判断逻辑，可以决定在什么时候创建哪一个产品类的实例，客户端可以免除直接创建产品对象的责任，而仅仅“消费”产品；
//简单工厂模式通过这种做法实现了对责任的分割，它提供了专门的工厂类用于创建对象。
//使用简单工厂模式将会增加系统中类的个数，在一定程序上增加了系统的复杂度和理解难度。
//系统扩展困难，一旦添加新产品就不得不修改工厂逻辑，在产品类型较多时，有可能造成工厂逻辑过于复杂，不利于系统的扩展和维护。
	
	cout << f1.get_result() << endl;
	cout << f2.get_result() << endl;
	cout << f3.get_result() << endl;
	cout << f4.get_result() << endl;
	cout << f5.get_result() << endl;
	cout << f7.get_result() << endl;
	cout << f6.get_result() << endl;
}

//理解基本原理是通过
//大话设计模式

}