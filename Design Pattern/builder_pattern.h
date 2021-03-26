#pragma once

namespace builder_pattern
{

//建造者模式
//建造者模式的好处就是使得建造代码与表示代码分离，
//由于建造者隐藏了该产品是如何组装的，所以若需要改变一个产品的内部表示，只需要再定义一个具体的建造者就可以了
/*
建造者模式是在当创建复杂对象的算法应该独立于该算法的组成部分以及他们的装配方式时适用的模式
*/
class BuildDirect;
class BuildDirect_mothod2;
class build_base //比如建造个小人，小人的眼睛鼻子嘴巴胳膊腿等等
{
	friend class BuildDirect_mothod2;
	friend class BuildDirect;
public:
	void debug()
	{
		cout << A << " " << B << " " << C << " " << D << " " << E << " " << F << endl;
	}
protected:
	virtual void build_partA() = 0;
	virtual void build_partB() = 0;
	virtual void build_partC() = 0;
	virtual void build_partD() = 0;
	virtual void build_partE() = 0;
	virtual void build_partF() = 0;

	double A;
	double B;
	double C;
	double D;
	double E;
	double F;
};


class Product1 :public build_base //比如建造个胖小人，是胖一点的参数
{
	friend class BuildDirect;
private:
	void build_partA() override
	{
		A = 3;
		cout << "Product1::build_partA" << endl;
	}
	void build_partB() override
	{
		B = 3;
		cout << "Product1::build_partB" << endl;
	}
	void build_partC() override
	{
		C = 3;
		cout << "Product1::build_partC" << endl;
	}
	void build_partD() override
	{
		D = 3;
		cout << "Product1::build_partD" << endl;
	}
	void build_partE() override
	{
		E = 3;
		cout << "Product1::build_partE" << endl;
	}
	void build_partF() override
	{
		F = 3;
		cout << "Product1::build_partF" << endl;
	}
};

class Product2 :public build_base //比如建造个瘦小人，是瘦一点的参数
{
private:
	void build_partA() override
	{
		A = 1;
		cout << "Product2::build_partA" << endl;
	}
	void build_partB() override
	{
		B = 1;
		cout << "Product2::build_partB" << endl;
	}
	void build_partC() override
	{
		C = 1;
		cout << "Product2::build_partC" << endl;
	}
	void build_partD() override
	{
		D = 1;
		cout << "Product2::build_partD" << endl;
	}
	void build_partE() override
	{
		E = 1;
		cout << "Product2::build_partE" << endl;
	}
	void build_partF() override
	{
		F = 1;
		cout << "Product2::build_partF" << endl;
	}
};

class Product3 :public build_base //比如建造个又高又胖的人，但是忘记继承某两部分了 说明还是抽象基类
{
private:
	void build_partA() override
	{
		A = 6;
		cout << "Product3::build_partA" << endl;
	}
	void build_partB() override
	{
		B = 8;
		cout << "Product3::build_partB" << endl;
	}
	void build_partC() override
	{
		C = 9;
		cout << "Product3::build_partC" << endl;
	}
	void build_partD() override
	{
		E = 10;
		cout << "Product3::build_partD" << endl;
	}
	//void build_partE() override
	//{
	//	cout << "Product3::build_partE" << endl;
	//}
	//void build_partF() override
	//{
	//	cout << "Product3::build_partF" << endl;
	//}
};

class BuildDirect
{
public:
	static void build(const shared_ptr<build_base> & bb)
	{
		//按照给定的顺序 这里是BCAEDF的组装顺序
		bb->build_partB();
		bb->build_partC();
		bb->build_partA();
		bb->build_partE();
		bb->build_partD();
		bb->build_partF();
	}
};

class BuildDirect_mothod2 //第二种建造方法
{
public:
	static void build(shared_ptr<build_base> & bb)
	{
		//按照给定的顺序 这里是ABDCEF的组装顺序
		bb->build_partA();
		bb->build_partB();
		bb->build_partD();
		bb->build_partC();
		bb->build_partE();
		bb->build_partF();
	}
};


/*
建造者模式的动机
1、建造者模式可以将部件和其组装过程分开，一步一步创建一个复杂对象，用户只需要指定复杂对象的类型就可以得到该对象，
而无需知道其内部的具体构造细节
2、在软件开发中，也存在大量类似汽车一样的复杂对象，他们有一系列成员属性，这些成员属性中可能存在着一些相互的制约关系，
比如某些属性没有赋值则复杂对象不能作为一个完整的产品使用；有些属性的赋值必须遵照某个顺序，一个属性没有赋值之前，另一个属性可能无法赋值
3、复杂对象相当于一辆有待建造的汽车，而对象属性相当于汽车的部件，建造产品的过程就相当于组合部件的过程。
由于组合部件的过程很复杂，因此，这些部件的组合过程往往被外部化到一个称作建造者的对象里，
建造者返还给客户端的是一个已经建造完毕的完整产品对象，而用户无需关心该对象所包含的属性以及他们的组装方式

定义：将一个复杂对象的构建与他的表示分离，使得同样的构建过程可以创建不同的表示（麦当劳 味道一样，制作流程一样，但是食材的产地不一样）

在客户端代码中，无需关心产品对象的具体组装过程，只需确定具体建造者的类型即可，建造者模式将复杂对象的构建与对象的表现分离开来，
这使得同样的构建过程可以创建出不同的表现

https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/builder.html
这个链接里给了一个肯德基的很好的例子，MealBuilder为基类，派生出两个套餐，可以固定的组合食物和饮料，
可扩展性也非常好，可以进一步扩展出新的套餐或者让顾客去点
KFCWaiter作为引导类调用整个流程，将产品本身和产品的创建过程解耦 

优点：
1.每一个具体的建造者都相对独立，而与其他的具体建造者无关，因此可以很方便地替换建造者或增加新的具体建造者，
用户使用不同的具体建造者可以得到不同的产品对象
2.可以更加精细地控制产品的创建过程，将复杂产品的创建步骤分解在不同的方法中，使得创建过程更加清晰，也更方便使用程序来控制创建过程
3.增加新的具体建造者无需修改原有类库的代码，指挥者类针对抽象建造者类编程（抽象基类），系统扩展方便，符合开闭原则。

缺点：
1、建造者模式所创建的产品一般具有较多的共同点，其组成部分相似，如果产品之间的差异性很大，则不适合使用建造者模式
2、如果产品的内部变化复杂，可能会导致需要定义很多具体建造者类来实现这种变化，导致系统变得很庞大

模式应用
在很多游戏软件中，地图包括天空、地面、背景等组成部分，人物角色包括人体、服装、装备等组成部分
可以使用建造者模式对其进行设计，通过不同的具体建造者创建不同类型的地图或人物
比如法师和猎手的不同属性点

如果将抽象工厂模式看成汽车配件生产工厂，生产一个产品族的产品，那么建造者模式就是一个汽车组装工厂，通过对部件的组装可以返回一辆完整的汽车

*/

void test()
{
	shared_ptr<build_base> prodptrs = shared_ptr<build_base>(new Product1);
	BuildDirect::build(prodptrs);
	prodptrs->debug();
	prodptrs = shared_ptr<build_base>(new Product2);
	BuildDirect::build(prodptrs);
	prodptrs->debug();
	prodptrs = shared_ptr<build_base>(new Product2);
	BuildDirect_mothod2::build(prodptrs); //用一种新的方法构造
	prodptrs->debug();
	//prodptrs = shared_ptr<build_base>(new Product3);
	//product3没有继承基类的部分虚函数，因此不能定义对象
	//相当于复杂对象没有定义某些属性的话就不能作为一个完整的产品使用
	//BuildDirect BD2(prodptrs);
}

//https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/builder.html
//大话设计模式


} //end namespace builder pattern