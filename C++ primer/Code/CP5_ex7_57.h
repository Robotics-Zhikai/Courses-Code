#ifndef CP5_ex7_57_h
#define CP5_ex7_57_h
#include "main.h"
#include <iostream>
#include <string>

class Account
//ÿ�����Լ����˺� 
//������Ϣ�������п��š����롢�洢���
//���еĺ�۵��ذ�����׼����
{
public:
	Account(std::string HumanIDinput, std::string Combiinput = DefaultCombination) :
		ID_credit("ID_card:"+std::to_string(++NumID)), Combination(Combiinput),
		HumanID(HumanIDinput){ };
	//Ĭ������������0
	Account(Account& acc) :ID_credit("ID_credit:" + std::to_string(++NumID)),Combination(DefaultCombination) {}
	//�������캯������ζ�Ž����п��ı�Ҫ��ϢŲ����һ�ſ��ϣ��൱�����¿���һ�ſ������ſ������벻Ӧ�ü̳���һ�ſ����������ΪĬ������
	const std::string ID_credit;
	const std::string HumanID; //�˵����֤��

	static double read_rate() { return rate; }
	static void change_rate(double changedrate) { rate = changedrate; }
	static std::string read_DefaultCombination() { return DefaultCombination; }
private:
	double Balence = 0;//�洢���
	std::string Combination;
	static long NumID; //��ʾ���ſ������з��ĵ�NumID�ſ�
	//static constexpr std::string DefaultCombination ; //�����ᱨ��constexpr��������ı����ͺ���������
	static const std::string DefaultCombination; //ֻ��static���Ͳ�����Ĭ��ʵ��
	static double rate ;//���е�����
};


long Account::NumID = 0;
const std::string Account::DefaultCombination = "000000";//������ͳһ������ʼ��Ĭ�������Ƕ���

#endif