#pragma once
#include "main.h"
#include <iostream>
#include <string>

class Account
//ÿ�����Լ����˺� 
//������Ϣ�������п��š����롢�洢���
//���еĺ�۵��ذ�����׼����
{
public:
	/* explicit */Account(std::string HumanIDinput, std::string Combiinput = DefaultCombination) :/*test(HumanIDinput),*/
		ID_credit("ID_card:"+std::to_string(++NumID)), Combination(Combiinput),
		HumanID(HumanIDinput){ };
	//Ĭ������������0
	Account(Account& acc) :ID_credit("ID_credit:" + std::to_string(++NumID)),Combination(DefaultCombination)
	,HumanID(acc.HumanID),Balence(acc.Balence)/*, test(std::string("s"))*/ {}
	//�������캯������ζ�Ž����п��ı�Ҫ��ϢŲ����һ�ſ��ϣ��൱�����¿���һ�ſ������ſ������벻Ӧ�ü̳���һ�ſ����������ΪĬ������
	Account& operator= (const Account& acc)//����������ã���ô��ֵ��ʱ��Ҫ����һ���������캯����NUmID������Ե�޹ʼ���һ��
	{
		//�ǲ������const��ֵ�ģ���Ϊ��Ȼ��Ҫ���ø�ֵ��˵���Ѿ��������캯���ˣ�Ҳ����˵�Ѿ�������Ϊconst�ı�����ֵ�ˡ�
		//��Ҫ�ǻ���ͨ����ֵ��丳ֵ�Ļ�����ì����
		Balence = acc.Balence + 100;
		return *this;
	}
	//������const��Ա�Ĵ��ڣ���˺ϳɵĿ�����ֵ�������ɾ���ģ�ֻ����ʾдһ��������ֵ�������������ϱߵ�operator=ע��
	//���Ļ����ڽ��п�����ֵʱ�ᱨ���޷����ú���operator=������ʽ������--�����Ѿ�ɾ���ĺ���
	const std::string ID_credit;
	const std::string HumanID; //�˵����֤��

	static double read_rate() { return rate; }
	static void change_rate(double changedrate) { rate = changedrate; }
	static std::string read_DefaultCombination() { return DefaultCombination; }
	static long read_NumID() { return NumID; }
	
private:
	/*std::string & test ;*/
	double Balence = 9;//�洢��� ��ÿ���¿��Ŀ�Ԥ��9Ԫ
	std::string Combination;
	static long NumID; //��ʾ���ſ������з��ĵ�NumID�ſ� ������private�ģ�Ϊ�˰�ȫֻ�ṩֻ������
	//static constexpr std::string DefaultCombination ; //�����ᱨ��constexpr��������ı����ͺ���������
	static const std::string DefaultCombination; //ֻ��static���Ͳ�����Ĭ��ʵ��
	static double rate ;//���е�����
};

double Account::rate = 0.0001;
long Account::NumID = 0;
const std::string Account::DefaultCombination = "000000";//������ͳһ������ʼ��Ĭ�������Ƕ���

