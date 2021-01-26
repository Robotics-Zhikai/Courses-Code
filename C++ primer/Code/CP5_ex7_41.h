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
		std::cout << "Sales_data����" << std::endl;
	}
	Sales_data(const std::string &s, unsigned n, double p) :bookNo(s), units_sold(n), revenue(n*p)
	{
		std::cout << "Sales_data(const std::string&, unsigned, double)" << std::endl;
	}

	Sales_data() : Sales_data("", 0, 0.0f) //ί�й��캯��
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
	/*explicit*/ Sales_data(double v) :Sales_data("", 0, v) //������explicit���Ͳ�����ʽ����������캯����
	{
		std::cout << "Sales_data(double v)" << std::endl;
	}
	Sales_data & returnRef(Sales_data & s) { return s; }
	Sales_data returnValue(Sales_data s) { return s; }
	//Sales_data & combine(const Sales_data &s) { Sales_data s1; return s1; } //�����õĺ������붨��һ�������壬���������޷������Ĵ���
	Sales_data & comblne(Sales_data s) { Sales_data s1; return s1; /*���ؾֲ���������Ϊ���κ�ʱ���Ǽ�������*/}

	//Sales_data(Sales_data s);//���������캯�����ܴ���Sales_data���͵Ĳ�����
	/*explicit*/ Sales_data(const Sales_data &s,int s1=0) : bookNo(s.bookNo),units_sold(s.units_sold),revenue(s.revenue)
	{ std::cout << "ִ��Sales_data�������캯��" << std::endl; }
	//�������explicit�����н�Ҫ��ʽ���ÿ������캯���ĵط��������� ��returnһ��ֵ���ͣ���=��ʼ��һ����������ֵ���β�
	//�����һ��������βΣ�û�и�Ĭ��ֵ����ô�Ͳ�����Ϊ�������캯�����������Լ��ϳ�һ���������캯��
	//�����Ĭ��ֵ����ô�͵���Ϊ�������캯��
	//���������β���Ĭ��ֵ�Ĵ��ڵ���2����ô�����������캯����ͻ������ͨ������
	
	//Sales_data operator= (Sales_data& s) = default;//�������Ͷ���Ĭ�ϵķ����������Ч
	//void operator= (Sales_data& s) = default;//�������Ͷ���Ĭ�ϵķ����������Ч
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