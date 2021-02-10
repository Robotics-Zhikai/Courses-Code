#pragma once
#include "main.h"
using namespace std;
class Quote
{
	friend double print_total(ostream &os, const Quote& item, size_t n);
public:
	Quote() = default;
	Quote(const string& bn, double pr) :bookNo(bn),prize(pr){}
	string ISBN()const { return bookNo; } //ISBN���ܱ��ı�

	void testnotvirtual(int n)
	{
		cout << "ss" << endl;
	}
	virtual ~Quote() = default; // ���ڵ㶨��һ������������
	virtual double net_prize(size_t n)const //��ζ����������Ҫoverride������� ��Ϊ�������п��ܸı���prize��ֵ
	{
		return prize * n;
	}
	virtual void testvirtual(int n = 9)const 
	{
		cout << n << endl;
	}
	virtual void debug()const 
	{
		cout << "bookNo:" << bookNo << endl;
		cout << "prize:" << prize << endl;
	}
private:
	string bookNo;
protected:
	double prize = 0.0;
};

//���㲢��ӡ���۸���������ĳ���鼮���÷���
double print_total(ostream &os, const Quote& item, size_t n)
//�����ʵ����friendҲ����,��Ϊ���ǵ��õ�public��װ
{
	item.testvirtual(); 
	//������ͨ����������õ��õĴ���Ĭ�ϲ������麯������˲����������Ĭ�ϲ���ֵ����ζ��壬����ΪĬ�ϲ�����ֵ�ǻ����Ĭ�ϲ���ֵ
	//������ô��⣺item��һ����������ã������ð����������ϣ��������������а�������˻��࣬���ô���Ĭ�ϲ����е��þ�̬���͵ĺ��壬��˵��û����Ĭ�ϲ���
	//�����ϱߵ���ⷽ���������ڶ�̬����
	os << item.ISBN() << endl;
	double ret = item.net_prize(n); //���ݶ�̬���������ã�item������Quote���࣬Ҳ������������������
	os << "������" << n << "���ļ۸���" << ret << endl;
	return ret;
}

class Bulk_quote:public Quote
{
	friend double print_total(ostream &os, const Quote& item, size_t n);
public:
	Bulk_quote() = default;
	Bulk_quote(const string & str, double pri,size_t minct,double dc ):Quote(str,pri),min_qty(minct),discount(dc){}

	void testnotvirtual()
	{
		cout << "ss1" << endl;
	}
	void testvirtual(int n = 19)const override
	{
		cout << n << endl;
	}

	double net_prize(size_t n,double n1)const //��������ĵ�����������ĳ�Ա����
	{
		if (n >= min_qty)
			return n*prize*(1 - discount);
		else
			return n*prize;
	}
	double net_prize(size_t n/*,int sfag*/)const override //ֻ��override�ܵõ����ǵı�֤ ������virtual�ǲ��ܵõ����ǵı�֤��
		//override���Ǳ���ģ���������������ȫ��ͬ�Ҳ���override����ôĬ���Ǹ����˻����ͬ��������
		//���ǵ�����д��ʱ������д�����ϱߵ�������ڲ���������Ҫ���ǵĻ����麯����ƥ�䣬��ô����override����ܱ���
		//ָ������д����
	{
		if (n >= min_qty)
			return n*prize*(1 - discount);
		else
			return n*prize;
	}
	void debug()const override
	{
		Quote::debug();
		cout << "min_qty:" << min_qty << endl;
		cout << "discount:" << discount << endl;
	}
private:
	size_t min_qty = 0; //ʹ���ۿ����ߵ���͹�����
	double discount = 0.0; //��С����ʾ���ۿ۶�
};

class Strategy_ex15_7 :public Quote
{
public:
	Strategy_ex15_7() = default;
	Strategy_ex15_7(const string& bookname, double bookprize, size_t numup, double dc)
		:Quote(bookname, bookprize), numupper(numup), discount(dc) {}
	double net_prize(size_t n) const override
	{
		if (n <= numupper)
			return n*prize*(1 - discount);
		else
			return numupper*prize*(1 - discount) + (n - numupper)*prize;
	}
	void debug()const override
	{
		Quote::debug();
		cout << "numupper:" << numupper << endl;
		cout << "discount:" << discount << endl;
	}
private:
	size_t numupper = 0; // ����һ�����Ͻ磬�������Ͻ�ʱ�������ְ�ԭ�����ۣ������������
	double discount = 0.0; // �����ۿ۶�

};

class testclass :public Strategy_ex15_7 //��Ӽ̳�Quote�����Է��������Ӷ�����Ӷ�����Ӷ����public��protect����
{
public:
	double teteag = prize;
	string adhADSF = ISBN();
	testclass(const string& bookname, double bookprize, size_t numup, double dc)
		:Strategy_ex15_7(bookname, bookprize, numup, dc) {}
	//test()
	//	:Quote("g",4) {}//��������ʹ�ü�ӷ��������ࡣ
	//�������ŵ����һ����ֻ��Ҫ��֤����������Ĺ����ϵ���Ϳ��԰�
	//�����̳������๹����� Ҳ����ֱ�ӵ���ֱ�Ӽ̳еĹ��캯��

	double net_prize(size_t n,size_t j2) const //������û�м̳еĵ�������net_prize��������print_total����õ���Strategy_ex15_7::net_prize ��֪Ϊ�Σ���
	{
		return 1;
	}
};