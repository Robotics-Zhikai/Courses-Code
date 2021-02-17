#pragma once
#include"main.h"
//工厂方法模式
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

// 在工厂方法模式中，工厂父类负责定义创建产品对象的公共接口，
//而工厂子类则负责生成具体的产品对象，这样做的目的是将产品类的实例化操作延迟到工厂子类中完成，
//即通过工厂子类来确定究竟应该实例化哪一个具体产品类 
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
		//不能通过这样访问protected成员，因为这就相当于用户调用类的protected了
		//报错受保护的成员不能通过factory_base_binary的指针或对象访问
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
由于使用了面向对象的多态性，工厂方法模式保持了简单工厂模式的优点，
而且克服了它的缺点。在工厂方法模式中，核心的工厂类不再负责所有产品的创建，也就是说不像简单工厂那样case
而是将具体创建工作交给子类去做。这个核心类仅仅负责给出具体工厂必须实现的接口，
而不负责哪一个产品类被实例化这种细节，这使得工厂方法模式可以允许系统在不修改工厂角色的情况下引进新产品。
*/

/*
优点
1.工厂方法用来创建客户所需要的产品，同时还向客户隐藏了哪种具体产品类将被实例化这一细节，
用户只需要关心所需产品对应的工厂，无须关心创建细节，甚至无须知道具体产品类的类名。
2.它能够使工厂可以自主确定创建何种产品对象，而如何创建这个对象的细节则完全封装在具体工厂内部。
工厂方法模式之所以又被称为多态工厂模式，是因为所有的具体工厂类都具有同一抽象父类。
3.使用工厂方法模式的另一个优点是在系统中加入新产品时，无须修改抽象工厂和抽象产品提供的接口，
无须修改客户端，也无须修改其他的具体工厂和具体产品，而只要添加一个具体工厂和具体产品就可以了。
这样，系统的可扩展性也就变得非常好，完全符合“开闭原则”。
*/

/*
缺点
1.在添加新产品时，需要编写新的具体产品类，而且还要提供与之对应的具体工厂类，
系统中类的个数将成对增加，在一定程度上增加了系统的复杂度，有更多的类需要编译和运行，
会给系统带来一些额外的开销。
2.由于考虑到系统的可扩展性，需要引入抽象层，
在客户端代码中均使用抽象层进行定义，增加了系统的抽象性和理解难度，
且在实现时可能需要用到DOM、反射等技术，增加了系统的实现难度。
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

//这种抽象化的结果使这种结构可以在不修改具体工厂类的情况下引进新的产品，不像简单工厂模式那样还得在工厂类中修改case
//如果出现新的按钮类型，只需要为这种新类型的按钮创建一个具体的工厂类就可以获得该新按钮的实例，
//这一特点无疑使得工厂方法模式具有超越简单工厂模式的优越性，更加符合“开闭原则”。


//https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/factory_method.html
//大话设计模式

}