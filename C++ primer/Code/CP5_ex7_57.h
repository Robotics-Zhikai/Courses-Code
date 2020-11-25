#ifndef CP5_ex7_57_h
#define CP5_ex7_57_h
#include "main.h"
#include <iostream>
#include <string>

class Account
//每个人自己的账号 
//个人信息包含银行卡号、密码、存储余额
//银行的宏观调控包括基准利率
{
public:
	Account(std::string HumanIDinput, std::string Combiinput = DefaultCombination) :
		ID_credit("ID_card:"+std::to_string(++NumID)), Combination(Combiinput),
		HumanID(HumanIDinput){ };
	//默认密码是六个0
	Account(Account& acc) :ID_credit("ID_credit:" + std::to_string(++NumID)),Combination(DefaultCombination) {}
	//拷贝构造函数，意味着将银行卡的必要信息挪到另一张卡上，相当于重新开了一张卡，这张卡的密码不应该继承上一张卡，因此设置为默认密码
	const std::string ID_credit;
	const std::string HumanID; //人的身份证号

	static double read_rate() { return rate; }
	static void change_rate(double changedrate) { rate = changedrate; }
	static std::string read_DefaultCombination() { return DefaultCombination; }
private:
	double Balence = 0;//存储余额
	std::string Combination;
	static long NumID; //表示本张卡是银行发的第NumID张卡
	//static constexpr std::string DefaultCombination ; //这样会报错constexpr必须具有文本类型和引用类型
	static const std::string DefaultCombination; //只有static类型才能做默认实参
	static double rate ;//银行的利率
};


long Account::NumID = 0;
const std::string Account::DefaultCombination = "000000";//由银行统一决定初始的默认密码是多少

#endif