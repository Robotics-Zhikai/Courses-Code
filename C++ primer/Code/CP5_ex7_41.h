#ifndef CP5_ex7_41_h
#define CP5_ex7_41_h

#include <string>
#include <iostream>

class Sales_data {
	friend std::istream &read(std::istream &is, Sales_data &item);
	friend std::ostream &print(std::ostream &os, const Sales_data &item);
	friend Sales_data add(const Sales_data &lhs, const Sales_data &rhs);

public:
	~Sales_data()
	{
		std::cout << "Sales_data析构";
	}
	Sales_data(const std::string &s, unsigned n, double p) :bookNo(s), units_sold(n), revenue(n*p)
	{
		std::cout << "Sales_data(const std::string&, unsigned, double)" << std::endl;
	}

	Sales_data() : Sales_data("", 0, 0.0f) //委托构造函数
	{
		std::cout << "Sales_data()" << std::endl;
	}

	Sales_data(const std::string &s) : Sales_data(s, 0, 0.0f)
	{
		std::cout << "Sales_data(const std::string&)" << std::endl;
	}

	Sales_data(std::istream &is);

	std::string isbn() const { return bookNo; }
	Sales_data& combine(const Sales_data&);
	
	///////////////////////////////////////////////
	//zk
	/*explicit*/ Sales_data(double v) :Sales_data("", 0, v) //若加上explicit，就不能隐式调用这个构造函数了
	{
		std::cout << "Sales_data(double v)" << std::endl;
	}
	Sales_data & returnRef(Sales_data & s) { return s; }
	Sales_data returnValue(Sales_data s) { return s; }
	//Sales_data & combine(const Sales_data &s) { Sales_data s1; return s1; } //被调用的函数必须定义一个函数体，否则会出现无法解析的错误
	Sales_data & comblne(Sales_data s) { Sales_data s1; return s1; /*返回局部变量的行为在任何时候都是极其错误的*/}


	///////////////////////////////////////////////



private:
	inline double avg_price() const;

private:
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};

inline
double Sales_data::avg_price() const
{
	return units_sold ? revenue / units_sold : 0;
}

// declarations for nonmember parts of the Sales_data interface.
std::istream &read(std::istream &is, Sales_data &item);
std::ostream &print(std::ostream &os, const Sales_data &item);
Sales_data add(const Sales_data &lhs, const Sales_data &rhs);

#endif