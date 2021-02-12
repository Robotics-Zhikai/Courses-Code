#pragma once


class Base
{
public:
	void pub_mem();
protected:
	int prot_mem;
private:
	char priv_mem;
};

class pub_derv :public Base
{
public:
	void memfcn(Base& b) { b = *this; } //���ۼ̳п�����ʲô�����������ж����Է��ʻ���Ĺ��г�Ա����˿��������������ת��
};
class priv_derv :private Base
{
public:
	void memfcn(Base &b) { b = *this; }
};
class prot_derv :protected Base
{
public:
	void memfcn(Base &b) { b = *this; }
};
class derived_from_public :public pub_derv //����public�̳У�������϶��ܷ��ʵ�����Ĺ��г�Ա
{
public:
	void memfcn(Base& b) { b = *this; }
};
class derived_from_private :public priv_derv //���ڻ��࣬��һ����private�̳У���ô����Ĺ��г�Ա�ͳ���private���ˣ���public��Ҳ������ʵ��� ��˱������
{
	//void memfcn(Base& b) { b = *this; }//��������Բ��ɷ��ʵĻ�����з���
};
class derived_from_protected :public prot_derv //���ڻ��࣬��һ����protected�̳У���public�̳У�����Ĺ��г�Ա����protected�������������ܹ������ʵ�
{
public:
	void test()
	{
		pub_mem(); //protected���ͣ����Ա���������ʵ�
	}
	void memfcn(Base& b) { b = *this; }
};

