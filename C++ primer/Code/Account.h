#pragma once
#include "main.h"
#include <iostream>
#include <string>

class Account
//每个人自己的账号 
//个人信息包含银行卡号、密码、存储余额
//银行的宏观调控包括基准利率
{
public:
	/* explicit */Account(std::string HumanIDinput, std::string Combiinput = DefaultCombination) :/*test(HumanIDinput),*/
		ID_credit("ID_card:"+std::to_string(++NumID)), Combination(Combiinput),
		HumanID(HumanIDinput){ };
	//默认密码是六个0
	Account(Account& acc) :ID_credit("ID_credit:" + std::to_string(++NumID)),Combination(DefaultCombination)
	,HumanID(acc.HumanID),Balence(acc.Balence)/*, test(std::string("s"))*/ {}
	//拷贝构造函数，意味着将银行卡的必要信息挪到另一张卡上，相当于重新开了一张卡，这张卡的密码不应该继承上一张卡，因此设置为默认密码
	Account& operator= (const Account& acc)//如果不加引用，那么赋值的时候还要调用一波拷贝构造函数，NUmID就又无缘无故加了一次
	{
		//是不允许给const赋值的，因为既然是要调用赋值，说明已经经过构造函数了，也就是说已经给声明为const的变量赋值了。
		//再要是还能通过赋值语句赋值的话，就矛盾了
		Balence = acc.Balence + 100;
		return *this;
	}
	//由于有const成员的存在，因此合成的拷贝赋值运算符是删除的，只能显示写一个拷贝赋值运算符；如果把上边的operator=注释
	//掉的话，在进行拷贝赋值时会报错：无法引用函数operator=（已隐式声明）--他是已经删除的函数
	const std::string ID_credit;
	const std::string HumanID; //人的身份证号

	static double read_rate() { return rate; }
	static void change_rate(double changedrate) { rate = changedrate; }
	static std::string read_DefaultCombination() { return DefaultCombination; }
	static long read_NumID() { return NumID; }
	
private:
	/*std::string & test ;*/
	double Balence = 9;//存储余额 给每张新开的卡预存9元
	std::string Combination;
	static long NumID; //表示本张卡是银行发的第NumID张卡 由于是private的，为了安全只提供只读函数
	//static constexpr std::string DefaultCombination ; //这样会报错constexpr必须具有文本类型和引用类型
	static const std::string DefaultCombination; //只有static类型才能做默认实参
	static double rate ;//银行的利率
};

double Account::rate = 0.0001;
long Account::NumID = 0;
const std::string Account::DefaultCombination = "000000";//由银行统一决定初始的默认密码是多少

