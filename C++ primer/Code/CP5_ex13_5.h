#ifndef CP5_ex13_5_h
#define CP5_ex13_5_h
#include <string>
#include <iostream>

class HasPtr
{
public:
	HasPtr(int num) :ps(new std::string()), i(num) {}
	HasPtr(const std::string &s = std::string()) :ps(new std::string(s)), i(0) {}
	HasPtr(const HasPtr& has,int s=1) :ps(new std::string(*has.ps+"2")),i(has.i) {}
	//�������һ���µ�string��ַ��ps���õ�ַָ�򱻿�����has��string+"2"
	//������Ǳ������Լ��ϳɵĿ������캯���Ļ���ֻ�´��ps����Ҳ����˵���������µ�ps��ֵ����ȵ�
	//������ϲ���Ĭ��ֵ��int s����ô�Ͳ��ǿ������캯���ˣ���ֻ��ʹ�ñ������Լ��ṩ�Ŀ������캯����Ч���ǿ���ps����
private:
	std::string *ps;
	int i;
};






#endif