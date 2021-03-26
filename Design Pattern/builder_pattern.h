#pragma once

namespace builder_pattern
{

//������ģʽ
//������ģʽ�ĺô�����ʹ�ý���������ʾ������룬
//���ڽ����������˸ò�Ʒ�������װ�ģ���������Ҫ�ı�һ����Ʒ���ڲ���ʾ��ֻ��Ҫ�ٶ���һ������Ľ����߾Ϳ�����
/*
������ģʽ���ڵ��������Ӷ�����㷨Ӧ�ö����ڸ��㷨����ɲ����Լ����ǵ�װ�䷽ʽʱ���õ�ģʽ
*/
class BuildDirect;
class BuildDirect_mothod2;
class build_base //���罨���С�ˣ�С�˵��۾�������͸첲�ȵȵ�
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


class Product1 :public build_base //���罨�����С�ˣ�����һ��Ĳ���
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

class Product2 :public build_base //���罨�����С�ˣ�����һ��Ĳ���
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

class Product3 :public build_base //���罨����ָ����ֵ��ˣ��������Ǽ̳�ĳ�������� ˵�����ǳ������
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
		//���ո�����˳�� ������BCAEDF����װ˳��
		bb->build_partB();
		bb->build_partC();
		bb->build_partA();
		bb->build_partE();
		bb->build_partD();
		bb->build_partF();
	}
};

class BuildDirect_mothod2 //�ڶ��ֽ��췽��
{
public:
	static void build(shared_ptr<build_base> & bb)
	{
		//���ո�����˳�� ������ABDCEF����װ˳��
		bb->build_partA();
		bb->build_partB();
		bb->build_partD();
		bb->build_partC();
		bb->build_partE();
		bb->build_partF();
	}
};


/*
������ģʽ�Ķ���
1��������ģʽ���Խ�����������װ���̷ֿ���һ��һ������һ�����Ӷ����û�ֻ��Ҫָ�����Ӷ�������;Ϳ��Եõ��ö���
������֪�����ڲ��ľ��幹��ϸ��
2������������У�Ҳ���ڴ�����������һ���ĸ��Ӷ���������һϵ�г�Ա���ԣ���Щ��Ա�����п��ܴ�����һЩ�໥����Լ��ϵ��
����ĳЩ����û�и�ֵ���Ӷ�������Ϊһ�������Ĳ�Ʒʹ�ã���Щ���Եĸ�ֵ��������ĳ��˳��һ������û�и�ֵ֮ǰ����һ�����Կ����޷���ֵ
3�����Ӷ����൱��һ���д�����������������������൱�������Ĳ����������Ʒ�Ĺ��̾��൱����ϲ����Ĺ��̡�
������ϲ����Ĺ��̺ܸ��ӣ���ˣ���Щ��������Ϲ����������ⲿ����һ�����������ߵĶ����
�����߷������ͻ��˵���һ���Ѿ�������ϵ�������Ʒ���󣬶��û�������ĸö����������������Լ����ǵ���װ��ʽ

���壺��һ�����Ӷ���Ĺ��������ı�ʾ���룬ʹ��ͬ���Ĺ������̿��Դ�����ͬ�ı�ʾ������ ζ��һ������������һ��������ʳ�ĵĲ��ز�һ����

�ڿͻ��˴����У�������Ĳ�Ʒ����ľ�����װ���̣�ֻ��ȷ�����彨���ߵ����ͼ��ɣ�������ģʽ�����Ӷ���Ĺ��������ı��ַ��뿪����
��ʹ��ͬ���Ĺ������̿��Դ�������ͬ�ı���

https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/builder.html
������������һ���ϵ»��ĺܺõ����ӣ�MealBuilderΪ���࣬�����������ײͣ����Թ̶������ʳ������ϣ�
����չ��Ҳ�ǳ��ã����Խ�һ����չ���µ��ײͻ����ù˿�ȥ��
KFCWaiter��Ϊ����������������̣�����Ʒ����Ͳ�Ʒ�Ĵ������̽��� 

�ŵ㣺
1.ÿһ������Ľ����߶���Զ��������������ľ��彨�����޹أ���˿��Ժܷ�����滻�����߻������µľ��彨���ߣ�
�û�ʹ�ò�ͬ�ľ��彨���߿��Եõ���ͬ�Ĳ�Ʒ����
2.���Ը��Ӿ�ϸ�ؿ��Ʋ�Ʒ�Ĵ������̣������Ӳ�Ʒ�Ĵ�������ֽ��ڲ�ͬ�ķ����У�ʹ�ô������̸���������Ҳ������ʹ�ó��������ƴ�������
3.�����µľ��彨���������޸�ԭ�����Ĵ��룬ָ��������Գ����������̣�������ࣩ��ϵͳ��չ���㣬���Ͽ���ԭ��

ȱ�㣺
1��������ģʽ�������Ĳ�Ʒһ����н϶�Ĺ�ͬ�㣬����ɲ������ƣ������Ʒ֮��Ĳ����Ժܴ����ʺ�ʹ�ý�����ģʽ
2�������Ʒ���ڲ��仯���ӣ����ܻᵼ����Ҫ����ܶ���彨��������ʵ�����ֱ仯������ϵͳ��ú��Ӵ�

ģʽӦ��
�ںܶ���Ϸ����У���ͼ������ա����桢��������ɲ��֣������ɫ�������塢��װ��װ������ɲ���
����ʹ�ý�����ģʽ���������ƣ�ͨ����ͬ�ľ��彨���ߴ�����ͬ���͵ĵ�ͼ������
���編ʦ�����ֵĲ�ͬ���Ե�

��������󹤳�ģʽ�������������������������һ����Ʒ��Ĳ�Ʒ����ô������ģʽ����һ��������װ������ͨ���Բ�������װ���Է���һ������������

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
	BuildDirect_mothod2::build(prodptrs); //��һ���µķ�������
	prodptrs->debug();
	//prodptrs = shared_ptr<build_base>(new Product3);
	//product3û�м̳л���Ĳ����麯������˲��ܶ������
	//�൱�ڸ��Ӷ���û�ж���ĳЩ���ԵĻ��Ͳ�����Ϊһ�������Ĳ�Ʒʹ��
	//BuildDirect BD2(prodptrs);
}

//https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/builder.html
//�����ģʽ


} //end namespace builder pattern