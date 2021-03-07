#pragma once
#include "main.h"
#include "StrVec.h"
namespace QuerySystem
{


using namespace std;

class QueryResult //�����ѯ�����������ѯ���ַ������ı����ݵ�ַ�������кŵĵ�ַ
{
	friend ostream& print(ostream& os, const QueryResult & que)
	{
		os << que.word << "����" << que.setptr->size() << "��" << endl;
		size_t countline = 0;
		for (auto it = que.setptr->begin(); it != que.setptr->end(); it++)
		{
			os << "(line " << *it << ") " << (*que.text)[*it] << endl;
		}
		return os;
	}
public:
	QueryResult() = default;
	QueryResult(const string& instr,const shared_ptr<StrVec>& intextptr,
		const shared_ptr<set<size_t>>& setptr) :word(instr), text(intextptr), setptr(setptr) {}
	set<size_t>::iterator begin()const { return setptr->begin(); }
	set<size_t>::iterator end()const { return setptr->end(); }
	shared_ptr<StrVec> get_file()const { return text; }
private:
	string word;
	shared_ptr<StrVec> text;
	shared_ptr<set<size_t>> setptr;
};


class TextQuery //һ��TextQuery����һ���ı���ӳ�䣬����˵Ӧ��Ҫ�󿽱����캯���Ϳ�����ֵ������delete��
{
public:
	using textptr = shared_ptr<StrVec>;
	using mappingptr = shared_ptr<map<string, set<size_t>>>; 

	TextQuery(const string& path):text(new StrVec)
	{
		ifstream file(path);
		readText(file);
	}
	QueryResult query(const string & word)const //��ðѲ�ѯ���������const�������κθı����ݿ�ķ���
	{
		static shared_ptr<set<size_t>> nodata(new set<size_t>); //����Ǳ���ģ���Ϊshared ptr������ʱ�����delete������ʵ����new�ĳ��� 
		auto found = mapping.find(word);
		if (found != mapping.end())
			return QueryResult(word, text, found->second /*mapping[word]*/);//���ֻ��������Ļ�������map���Զ�����һ��empty��mappedtype �������set.sizeʱ����
		else
			return QueryResult(word, text, nodata);
	}

private:
	textptr text;
	//mappingptr mapping; //���������map�����ó�����ָ�������ô����ζ��TextQuery��ֻ�𵽶�ȡ���ݵ��ڲ������ã����𵽲�ѯ������
	//��ѯ��ѹ���·ŵ���QueryResult�У�û�кܺõĽ���
	//��������Ҫ��ѯ�ܶ൥�ʲ���ѯ��Ϻ�textquery���ͷţ��ڲ����е��ļ����ݶ�û�б��ͷţ���������set��Ϊ����ָ��ʱ��������ѡ����ͷ��ڴ棬������ڴ������ʡ�
	map<string, shared_ptr<set<size_t>>> mapping;


	void readText(ifstream & filein)
	{
		string word;
		string linestr;
		size_t countline = 0;
		while (getline(filein, linestr))
		{
			text->push_back(linestr);
			istringstream record(linestr);
			while (record >> word)
			{
				auto found = mapping.find(word);
				if (found == mapping.end()) //˵��û�ҵ�
					mapping[word] = shared_ptr<set<size_t>>(new set<size_t>);
				mapping[word]->insert(countline);//����mappedtype��set���ͣ����Բ�����ͬһ���ж���ظ�Ԫ�ص��ظ�����
			}
			countline++;
		}
	}
};

class Query;
class Query_base //��ϣ���û���������ֱ��ʹ��Query_base��������еĳ�Ա����public
{
	friend Query;
protected:
	virtual ~Query_base() = default; //�������Ϊ���ڵ�Ҫ�õ������������������protected��Ա
private:
	virtual QueryResult eval(const TextQuery& t)const = 0;
	virtual string rep()const = 0; //���ȥ��virtual�Ļ��ᱨ������⺯������˵����

};

class WordQuery:public Query_base
{
	friend class Query;
private:
	WordQuery(const string & s) :word(s) { }
	QueryResult eval(const TextQuery& t)const override { return t.query(word); }
	string rep()const override { return "(" + word + ")"; }

	string word;

	static int wr;
	static void sad() //��̬��Ա������thisָ�� ���Ҳ�Ͳ��ܵ��÷Ǿ�̬��Ա��
	{
		wr = 3;
		//word = string("saf"); //����Ǿ�̬��Ա�������ض�������� ��Ҳ����û��this������
		//string asg = word;  //����ͬ��
	}
};

class Query
{
	friend Query operator~(const Query & qe);
	friend Query operator&(const Query & l, const Query& r);
	friend Query operator|(const Query & l, const Query& r);
public:
	Query(const string &str) :q(new WordQuery(str)) {};
	QueryResult eval(const TextQuery& t)const { return q->eval(t); }
	string rep()const { return q->rep(); }
	~Query() { cout << "~Query()" << endl; };
private:
	Query(const shared_ptr<Query_base>& q) :q(q) {} //��ϣ���û���������㶨��Query
	shared_ptr<Query_base> q;
};

class HistoryQuery//����һ����ʷ��
{
public:
	void addQuery(const Query& qe)
	{
		his_database.push_back(qe);
	}
	Query& operator[](size_t n)
	{
		return his_database[n];
	}
	size_t size()const { return his_database.size(); }
private:
	vector<Query> his_database;
};

class NotQuery :public Query_base
{
	friend Query operator~(const Query & qe);
private:
	NotQuery(const Query& qe) :qe(qe) {}

	string rep()const override
	{
		return "~(" + qe.rep() + ")";
	}
	QueryResult eval(const TextQuery& t)const override
	{
		QueryResult tmpresult = qe.eval(t);
		auto text = tmpresult.get_file();
		set<size_t>::iterator beg = tmpresult.begin();
		set<size_t>::iterator end = tmpresult.end();
		set<size_t> set_not;
		for (size_t i = 0; i < text->size(); i++)
		{
			if (beg!=end && i == *beg)
				++beg;
			else
				set_not.insert(i);
		}
		return QueryResult(rep(),text,shared_ptr<set<size_t>>(new set<size_t>(std::move(set_not))));
	}

	Query qe;
};

class BinaryQuery:public Query_base
{
protected: //������Ϊprivate����ô����������಻�ܵ���rep(),������û����дeval������໹�ǳ�����࣬����������û������Ա�������protected��
	BinaryQuery(const Query& l, const Query& r, string o) :lhs(l), rhs(r), opSym(o) {}
	string rep()const override { return "(" +lhs.rep()+" " + opSym + " " + rhs.rep()+")"; }

	Query lhs, rhs;
	string opSym; // ָ����Ԫ����ķ���
};

class AndQuery :public BinaryQuery
{
	friend Query operator&(const Query & l, const Query& r);
private:
	AndQuery(const Query& l, const Query& r):BinaryQuery(l,r,"&") {}
	QueryResult eval(const TextQuery& t)const override
	{
		QueryResult lhsresult = lhs.eval(t);
		QueryResult rhsresult = rhs.eval(t);
		set<size_t> setresult;
		set_intersection(lhsresult.begin(), lhsresult.end(), rhsresult.begin(), rhsresult.end(), inserter(setresult, setresult.begin()));
		return QueryResult(rep(), lhsresult.get_file(), shared_ptr<set<size_t>>(new set<size_t>(std::move(setresult))));
	}
};

class OrQuery :public BinaryQuery
{
	friend Query operator|(const Query & l, const Query& r);
private:
	OrQuery(const Query&l, const Query&r) :BinaryQuery(l, r, "|") {}
	QueryResult eval(const TextQuery& t)const override
	{
		QueryResult lhsresult = lhs.eval(t);
		QueryResult rhsresult = rhs.eval(t);
		set<size_t> setresult(lhsresult.begin(), lhsresult.end());
		setresult.insert(rhsresult.begin(), rhsresult.end());
		return QueryResult(rep(), lhsresult.get_file(), shared_ptr<set<size_t>>(new set<size_t>(std::move(setresult))));
	}
};

Query operator~(const Query & qe)
{
	shared_ptr<Query_base> p(new NotQuery(qe));
	return Query(p);
}

Query operator&(const Query & l,const Query& r)
{
	return Query(shared_ptr<Query_base>(new AndQuery(l, r)));
}

Query operator|(const Query & l, const Query& r)
{
	return Query(shared_ptr<Query_base>(new OrQuery(l, r)));
}

}

