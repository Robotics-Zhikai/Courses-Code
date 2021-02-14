#pragma once
#include "main.h"
using namespace std;
class Quote
{
	friend double print_total(ostream &os, const Quote& item, size_t n);
public:
	Quote() { cout << "Quote()" << endl; }
	Quote(const string& bn, double pr) :bookNo(bn), prize(pr) { cout << "Quote(const string& bn, double pr) " << endl; }
	Quote(const Quote& qt) :bookNo(qt.bookNo), prize(qt.prize) { cout << "Quote(const Quote& qt)" << endl; }
	Quote(Quote&& qt)noexcept :bookNo(std::move(qt.bookNo)), prize(std::move(qt.prize)) { cout << "Quote(Quote&& qt)" << endl; }
	Quote& operator=(const Quote& qt)
	{
		bookNo = qt.bookNo;
		prize = qt.prize;
		cout << "Quote& operator=(const Quote& qt)" << endl;
		return *this;
	}
	Quote& operator=(Quote&& qt)noexcept
	{
		if (this != &qt)
		{
			bookNo = std::move(qt.bookNo);
			prize = std::move(qt.prize);
		}
		cout << "Quote& operator=(Quote&& qt)" << endl;
		return *this;
	}
	virtual ~Quote()  // ���ڵ㶨��һ������������
	{
		cout << "virtual ~Quote()" << endl;
	}

	string ISBN()const { return bookNo; } //ISBN���ܱ��ı�

	void testnotvirtual(int n)const
	{
		cout << "ss" << endl;
	}
	
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
	//double newpurevirtual() = 0;//��Ҳ��һ���ģ�ƽ���޹��ڻ����ﶨ����һ�����麯�����ᱨ��
private:
	string bookNo = "";
protected:
	double prize = 0.0;
};

class pro_quote :protected Quote
{
public:
	void test()
	{
		//bookNo; //protected�̳У������private����private�����Բ��ܷ��ʡ���Ϊ��������˵��������������ĳ�Ա�ܷ������ֱ�ӻ���ĳ�ԱûʲôӰ��
		prize;
	}
};

class pro_pro_quote :protected pro_quote
{
public:
	void test()
	{
		ISBN(); //class pro_quote :protected Quoteʱ��ӻ����public����protected������ֱ�����������ڵ��ã������û�����
		//bookNo; //protected�̳ж���private�Ĵ����ξ���private�����������������
	}
};

class Disc_quote:public Quote
{
public:
	//double newpurevirtual() = 0; //���ƽ���޹ʶ���һ�����麯��������û���������麯������ô�ᱨ��
	double net_prize(size_t n)const = 0; //�������0Ҳ���𵽼��������������Ӧ�����Ƿ���ȫ��ͬ������ ��override�İ�ȫ��������һ��
	//string ISBN()const override;//����ʹ��override������д�����Ա
	void debug()const override
	{
		Quote::debug();
		cout << "quantity:" << quantity << endl;
		cout << "discount:" << discount << endl;
	}
	Disc_quote() { cout << "Disc_quote()" << endl; };
	Disc_quote(const string & book, double pr, size_t qt, double dc) :Quote(book, pr), quantity(qt), discount(dc) { cout << "Disc_quote(const string & book, double pr, size_t qt, double dc)" << endl; }
	Disc_quote(const Disc_quote& dq) :Quote(dq),quantity(dq.quantity), discount(dq.discount) { cout << "Disc_quote(const Disc_quote& dq)" << endl; }
	Disc_quote(Disc_quote&& dq)noexcept :Quote(std::move(dq)),quantity(std::move(dq.quantity)), discount(std::move(dq.discount)) { cout << "Disc_quote(Disc_quote&& dq)" << endl; }
	Disc_quote& operator=(const Disc_quote& dq)
	{
		Quote::operator=(dq);
		quantity = dq.quantity;
		discount = dq.discount;
		cout << "Disc_quote& operator=(const Disc_quote& dq)" << endl;
		return *this;
	}
	Disc_quote& operator=(Disc_quote&& dq) noexcept
	{
		Quote::operator=(std::move(dq));
		quantity = std::move(dq.quantity);
		discount = std::move(dq.discount);
		cout << "Disc_quote& operator=(Disc_quote&& dq)" << endl;
		return *this;
	}
	~Disc_quote() override //����override���Ա�֤���ڵ��������������
	{
		cout << "~Disc_quote()" << endl;
	}
protected:
	size_t quantity = 0; //�ۿ����õĹ�����
	double discount = 0.0;//�ۿ�������
};

class Bulk_quoteNew :public Disc_quote
{
public:
	Bulk_quoteNew() { cout << "Bulk_quoteNew()" << endl; };
	Bulk_quoteNew(const string & book, double pr, size_t qt, double dc) :Disc_quote(book, pr, qt, dc) { cout << "Bulk_quoteNew(const string & book, double pr, size_t qt, double dc)" << endl; }
	Bulk_quoteNew(const Bulk_quoteNew& bq) :Disc_quote(bq) { cout << "Bulk_quoteNew(const Bulk_quoteNew& bq)" << endl; }
	Bulk_quoteNew(Bulk_quoteNew&& bq)noexcept :Disc_quote(std::move(bq)) { cout << "Bulk_quoteNew(Bulk_quoteNew&& bq)" << endl; }
	Bulk_quoteNew& operator=(const Bulk_quoteNew& bq)
	{
		Disc_quote::operator=(bq);
		cout << "Bulk_quoteNew& operator=(const Bulk_quoteNew& bq)" << endl;
		return *this;
	}
	Bulk_quoteNew& operator=(Bulk_quoteNew&& bq) noexcept
	{
		Disc_quote::operator=(std::move(bq));
		cout << "Bulk_quoteNew& operator=(Bulk_quoteNew&& bq)" << endl;
		return *this;
	}
	~Bulk_quoteNew() override
	{
		cout << "~Bulk_quoteNew()" << endl;
	}
	
	
	double net_prize(size_t n)const override
	{
		if (n >= quantity)
			return n*prize*(1 - discount);
		else
			return n*prize;
		testnotvirtual(3);//��ӻ���Ҳ�ǿ��Ե��õ�
	}
};

class Strategy_ex15_7New :public Disc_quote
{
public:
	Strategy_ex15_7New() = default;
	Strategy_ex15_7New(const string & book, double pr, size_t qt, double dc):Disc_quote(book, pr, qt, dc) {}
	double net_prize(size_t n)const override
	{
		//Disc_quote dsag;//��������ʹ�ó������͵Ķ��� ��Ϊ������һ�����麯��
		if (n <= quantity)
			return n*prize*(1 - discount);
		else
			return quantity*prize*(1 - discount) + (n - quantity)*prize;
	}
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

