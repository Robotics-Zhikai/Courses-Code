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
	Account(std::string Combiinput = DefaultCombination) :ID_card("ID_card:"+std::to_string(++NumID)), Combination(Combiinput) { };
	//Ĭ������������0
	Account(Account& acc) :ID_card("ID_card:" + std::to_string(++NumID)),Combination(DefaultCombination) {}
	//�������캯������ζ�Ž����п��ı�Ҫ��ϢŲ����һ�ſ��ϣ��൱�����¿���һ�ſ������ſ������벻Ӧ�ü̳���һ�ſ����������ΪĬ������
	const std::string ID_card;
	const std::string name;
private:
	std::string Combination;
	static long NumID; //��ʾ���ſ������з��ĵ�NumID�ſ�
	//static constexpr std::string DefaultCombination ; //�����ᱨ��constexpr��������ı����ͺ���������
	static const std::string DefaultCombination; //ֻ��static���Ͳ�����Ĭ��ʵ��
};

long Account::NumID = 0;
const std::string Account::DefaultCombination = "000000";

#endif