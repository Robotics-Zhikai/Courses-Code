#ifndef QUERY_SYSTEM
#define QUERY_SYSTEM
#include "main.h"

namespace QuerySystem
{


using namespace std;

class TextQuery
{
public:
	TextQuery(const string& path)
	{
		ifstream file(path);
		readText(file);
	}

private:
	vector<string> text;
	map<string, set<size_t>> mapping;

	void readText(ifstream & filein)
	{
		string word;
		string linestr;
		size_t countline = 0;
		while (getline(filein, linestr))
		{
			text.push_back(linestr);
			istringstream record(linestr);
			while (record >> word)
				mapping[word].insert(countline);//����mappedtype��set���ͣ����Բ�����ͬһ���ж���ظ�Ԫ�ص��ظ�����
			countline++;
		}
	}
};














}

#endif