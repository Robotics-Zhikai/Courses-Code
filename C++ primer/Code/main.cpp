#include <iostream>
#include "main.h"
#include "another.h"
#include "CLASS.h"
#include "CP5_ex7_41.h"
#include "CP5_ex7_53.h"
#include "CP5_ex13_5.h"
#include "CP5_ex7_57.h"
#include "CP5_ex13_18.h"
#include "CP5_ex12_2.h"
#include "QuerySystem.h"
#include "Folder.h"
#include "Message.h"
using namespace std;

class Y;
class X
{
public:
	Y & Yreference(Y tmpY);
	X * dataX;
	int d;
	X & s;
};

class Y
{
	Y(X xdata)
	{
		Ydata = xdata.d;
	}
	int Ydata;
};



class screen
{
public:
	screen() = default; //��������������������캯��Ҳ���Խ��б������Լ��ϳɵ�Ĭ�Ϲ��캯��
	screen(int data1 = 5) :data(data1){} //����screen()=default���ڵ������ ֱ������ʱ�᲻֪�����׸õ����ĸ�����
	//ͬʱ�����������캯����д���Ƿǳ�����ģ���ΪĬ�Ϲ��캯����֪�����׸õ��ĸ� ���Ǳ���ͨ����û�������

	screen(int s,int s1) :data(1), DATA2(3), DATA1(DATA2) {} //������ʼ���Ǵ���ģ����ڳ�Ա��ʼ������˳���
	//������ʼ���Ļ�������ֱ����ڴ洢��������ֵ
	screen & display()
	{
		return *this;
	}
	screen(std::istream & is) { int x; is >> x; }
	//const screen & display() const { return *this; }
	screen display() const { return *this; }
	void changedata(int s) { data=s ; }
	int data = 6;
private:
	int DATA1 = 7;
	int DATA2 = 8;
};



namespace Exercise
{
	int ivar = 0;
	double dvar = 0;
	const int limit = 1000;
	float cout = 0;
}
namespace Exercise1
{
	int ivar = 1;
	double dvar = 1;
	const int limit = 10001;
	float cout = 1;
}

int ivar = 10;

int inlinetest()
{
	return 2;
}

void NotThrow() throw()
{
	throw exception("sss") ;
}

int & CastConstZK(const int &tmp)
{
	return const_cast<int &>(tmp); //��const�����׵��ˣ����Ƽ�����Ϊ��ڵĲ���Ӧ���ǲ��ɸı��
}



typedef string Type;
Type initVal();
class Excercise 
{
public :
	//typedef double Type; //�����������Ļ��Ϳ�������������typedef
	Type SetVal(Type parm);
	Type initVal() { ::Type s; return s; };
private:
	int val;
	typedef double Type;
	Type nima; //�ض������� �ǲ������ ���Ǳ��������ᱨ�� 
};

//Excercise::Type initVal() //��������Ļ�����Type��private ���ɷ��� ��������ټ�һ��Excercise�޶��� �Ϳ��Է�����
//{
//
//}

Type Excercise::SetVal(::Type parm) //Type��string���͵�
{
	::Type str; //����ü�ȫ���򣬷��������еĺ����ǲ����ݵġ�
	initVal();
	return str;
}

typedef int Notclass;
void func()
{
	//����ͨ������ �����ǺϷ��� ���������� �����������÷����������ȴ��������ٴ�����������趨�����·Ƿ���
	Notclass s1 = 2;
	typedef double Notclass;
	Notclass s = 1;
}


class Nodefault
{
public:
	~Nodefault() { cout << "����" << endl; }
	Nodefault(const std::string &);
	Nodefault(int i):data(i){ cout << "ִ��ʵ�ʸɻ�ĺ�����" << endl; };
	Nodefault() :Nodefault(1) { cout << "ִ�е�ί�й��캯����" << endl; } //ί�й��캯��
	const Nodefault & returnRef(const Nodefault& constref) { return constref; }
	Nodefault & returnrefnotconst(Nodefault & ref) { return ref; }
	int data;
};
class C
{
public:
	C() : data() {}
//public:
private:
	Nodefault data;
};

namespace n13_13
{
	struct X {
		X() { std::cout << "X()" << std::endl; }
		X(const X&) { std::cout << "X(const X&)" << std::endl; }
		X& operator=(const X&) { std::cout << "X& operator=(const X&)" << std::endl; return *this; }
		~X() { std::cout << "~X()" << std::endl; }
	};

	void f(const X &rx, X x)
	{
		std::vector<X> vec;
		vec.reserve(2);
		vec.push_back(rx);
		vec.push_back(x);
	}
}

namespace ex2_33
{
	int i = 0, &r = i;
	auto a = r;
	const int ci = i, &cr = ci;
	auto b = ci;
	auto c = cr;//���Զ���const
	auto d = &i;
	auto e = &ci;//�����ײ�const
	const auto f = ci;//���Զ���const ��Ҫ��ʾ�ļ���const
	auto &g = ci;
	//auto &h = 42;//�ұ�����һ������const auto����Ե�����const ����ᱨ��

	const int i35 = 42;
	auto j = i35; const auto &k = i35; auto *p = &i35;
	const auto j2 = i35, &k2 = i35;

	decltype (i35) s = 0;//���ʹ�õı��ʽ��һ����������decltype���ظñ��������ͣ���������const���������ڣ�
	//���ô���������Ϊ����ָ�����ͬ��ʳ��֣�ֻ��decltype����
	const int &iii = 2;
	decltype (iii) sad = i35;
}

namespace ex2_36
{
	int a = 3, b = 4;
	decltype(a) c = a;
	decltype((b)) d = a;

	int a1 = 5;
	int a2 = a1;//��ʼ�������ڱ����ھ��ܹ�ȷ��ֵ�ˡ�

	auto a3 = a1;
	decltype(a1) a4 = a1; //����������һ��

	const int a5 = 6;
	auto a6 = a5;
	decltype(a5) a7 = a5; //���������;Ͳ�һ����
}

namespace ex6_54
{
	int fucplus(int a, int b) { return a + b; }
	int fucminus(int a, int b) { return a - b; }
	int fucmultiple(int a, int b) { return a*b; }
	int fucdivid(int a, int b) { return a / b; }

	vector<int(*)(int, int)> vec_pfuc = {fucplus,fucminus,fucmultiple,fucdivid};
	void testvec_pfuc(int a,int b)
	{
		for (int i = 0; i < vec_pfuc.size(); i++)
		{
			std::cout << vec_pfuc[i](a, b) << std::endl; 
			//����������ʽ�����˲�̬���е������ϣ����ľ���һ�в�̬���ɴ��룬������������ѭ���������
			//�����ҪŪ��ͬ�Ĳ�̬����������������������õĻ���Ҫд�ü����ظ��Ⱥܸߵĺ���
			//������������������Ϳ��Լ���ؼ򻯴�������
		}
	}
	decltype(testvec_pfuc(1,2)) s();//decltype���ؼٶ�������������󷵻ص�����
	decltype(testvec_pfuc) s1;//decltype���غ�������Ӧ�ĺ������ͣ���ʵ�������Ƿ���һ������������
	//s1 Ϊvoid(int,int)���͵ĺ�������
	using Fp = int(*)(int, int);
	using F = int(int, int);

	Fp returnP2fuc(F fucname)//�Զ�ת��Ϊ����ָ��
	{
		return fucname; //����һ������ָ�� ���ܷ��غ�������
	}
	int(*sss(F fucname))(int, int) //�����ǿ��Ե�
	{
		return fucname;
	}
	//int(*)(int, int) ssss(F fucname) //�����ᱨ��
	//{

	//}
	int useFp(F fucname,int a,int b)
	{
		return returnP2fuc(fucname)(a, b);
	}
}

namespace ex12_2
{
	vector<int>ss(10);
	//ss[1] = 3; //�����ǻᱨ��� ������namespace�и�ֵ
}

namespace ex12_6
{
	vector<int> *returnvecint()
	{
		return new vector<int>;
	}
	shared_ptr<vector<int>> returnvecintIntelligence()
	{
		//return new vector<int>;//������new��ʼ������Ϊ�����ڶ�Ӧ�Ĺ��캯��
		//������Ϊ����ָ���������ָ��������explicit�ģ�������Ϊ�������캯������
		return make_shared<vector<int>>();
	}

	vector <int> *readinput(vector<int> * input)
	{
		cout << "����10��int����" << endl;
		for (int i = 0; i < 10; i++)
		{
			int tmp;
			cin >> tmp;
			input->push_back(tmp);
		}
		return input;
	}
	void readinputIntelligence(const shared_ptr<vector<int>> & input)
	{
		cout << "����10��int����" << endl;
		for (int i = 0; i < 10; i++)
		{
			int tmp;
			cin >> tmp;
			input->push_back(tmp);
		}
	}

	void printvecint(vector<int> * &input)//����������Ϊָ�������ֱ���޸�ָ���ֵ
	{
		for (int i = 0; i < input->size(); i++)
		{
			cout << (*input)[i] << " ";
		}
		delete input;
		input = nullptr; 
	}
	void printvecintIntelligence(shared_ptr<vector<int>> & input)
	{
		for (int i = 0; i < input->size(); i++)
		{
			cout << (*input)[i] << " ";
		}
		input = make_shared<vector<int>>(); //ָ����һ����ʱ���� �����Ļ��Ͱѷ�����ڴ��ͷ�������
	}
}

namespace ex10_9
{
	template<typename Ty>
	void ElimDups(vector<Ty>& input) //�����ظ�����
	{
		sort(input.begin(), input.end());//���ȥ�����еĻ�unique���ܺܺõ�ȥ��
		auto unieued = unique(input.begin(), input.end());
		input.erase(unieued, input.cend());
	}
}

namespace section10_3_1
{
	bool Isshorter(const string&s1, const string &s2)
	{
		if (s1.size() < s2.size())
			return 1;
		else
			return 0;
	}

	bool compareIsbn(const Sales_data&s1, const Sales_data&s2)
	{
		return s1.isbn().size() > s2.isbn().size();
	}

	Sales_data d1("aa"), d2("aaaa"), d3("aaa"), d4("z"), d5("aaaaz");
	std::vector<Sales_data> Sales_datav{ d1, d2, d3, d4, d5 };

	auto ex10_13 = std::vector<std::string>{ "aaaaassaa","a", "as", "aaaaaabba","aasss",  "aaa" };
	bool IsMore5(const string&s1)
	{
		if (s1.size() > 5)
			return 1;
		else
			return 0;
	}
}

namespace section10_3_2
{
	
	auto f_ex10_14 = 
		[](int a, int b)
	{
		return a + b; 
	};

	auto ex10_15(int a)
	{
		return [a](int b) {return a + b; };
	}

	void biggies(vector<string>& words,vector<string>::size_type sz)//����ڵ���һ���������ȵĵ����ж���,����ӡ
	{
		sort(words.begin(), words.end(), [](const string& s1, const string& s2) {return s1.size() < s2.size(); });//����������
		auto ele = find_if(words.begin(), words.end(), [sz](const string&s1) {return s1.size() > sz ? 1 : 0; });
		auto count = words.end() - ele;
		cout << "����" << sz << "���ȵĵ�����" << count << "��,�ֱ���" << endl;
		for_each(ele, words.end(), [](const string&s1) {cout << s1 << " "; });
	}

	void ex10_21(int in)
	{
		auto f = [&in]()->bool //�����ָ��bool�����ƶϳ������ķ���������int lambda
		{
			if (in > 0)
				in--;
			else if (in < 0)
				in++;
			else
				return 1;
			return 0;
		};

		while (!f())
			cout << in << endl;;
	}


	
	
}

namespace sec10_3_4
{
	using namespace std::placeholders;
	bool Iflowerthansz(const string& s1, int sz)
	{
		if (s1.size() <= sz)
			return 1;
		else
			return 0;
	}

	size_t CheckSizeLambda(const vector<int> & num, const string& s1) //��������ֵ
	{
		return find_if(num.begin(), num.end(), [&s1](int n) {return s1.size() < n; })-num.begin(); //lambda��һ���ɵ��ö���

	}

	bool CheckSize(string::size_type num, const string& s1)
	{
		if (s1.size() < num)
			return 1;
		else
			return 0;
	}
	size_t CheckSizeBind(const vector<int>&num, const string&s1) //��������ֵ
	{
		return find_if(num.begin(), num.end(), bind(CheckSize, _1, s1)) - num.begin();
	}

	//void foreach_exe(string::size_type num, const string& s1)
	//{
	//	if (!CheckSize(num, s1))
	//		cout << s1 << endl;
	//}
	void biggies(const vector<string>& str, string::size_type num) //����ڵ���һ���������ȵĵ����ж���,����ӡ
	{
		auto foreach_exe = [&](const string& s1)
		{
			if (!CheckSize(num, s1))
				cout << s1 << endl;
		};
		for_each(str.begin(), str.end(), foreach_exe); //��lambda�ɵ��ö���
		//for_each(str.begin(), str.end(), bind(foreach_exe, num, _1)); //bind����һ���ɵ��ö���  ע�͵��������������������
	}
}

namespace sec10_4_1
{
	std::vector<int> vec{ 1, 1, 3, 3, 5, 5, 7, 7, 9 };
	std::list<int> lst;

	vector<int> ex10_28 = { 1,2,3,4,5,6,7,8,9 };
	vector<int> insertercopy, backcopy, frontcopy;
}

namespace ex12_26
{
	void input_string_output_reversestring(size_t n)
	{
		string * newtest = new string[n]();//ֻ���ǿ����ţ�����������
		*(newtest + 3) = string("gtr");

		allocator<string> tmpalloc;
		string * beginptr = tmpalloc.allocate(n);
		//*beginptr = string("asdasf"); //�����������ʱ������Ϊ����û��construct����
		string * q = beginptr;
		string s;
		for (int i = 0; i < n; i++)
		{
			cin >> s;
			tmpalloc.construct(beginptr++,s);
		}
		while (q != beginptr)
		{
			cout << *(--beginptr) << endl;
			tmpalloc.destroy(beginptr);
		}
		*(beginptr+3) = string("asdasf"); //������Ͳ��ᱨ����Ϊ�Ѿ�construct��һ����
	}
}

namespace sec11_1
{
	vector<string> testdata = { "example.","example,","Example","gasdas","example","example","examp,le" };
	void CountString(const vector<string> &input) //���ʼ�������
	{
		map<string, size_t> record;
		for (size_t i = 0; i < input.size(); i++)
			record[input[i]]++;
		for (auto &r : record)
		{
			cout << r.first << "���ʳ���" << r.second << "��" << endl;		
		}
	}

	void CountStringIgnorePuc(vector<string> input)
	{
		map<string, size_t> record;
		char * tmpstring = NULL;
		for (size_t i = 0; i < input.size(); i++)
		{
			tmpstring = new char[input[i].size()+1];
			size_t cout = 0;
			for (auto c : input[i])
			{
				if (ispunct(c))
					continue;
				if (isupper(c))
					c = tolower(c);
				*(tmpstring + (cout++)) = c;
			}
			*(tmpstring + cout) = '\0';
			input[i] = string(tmpstring);
			delete[]tmpstring;
			record[input[i]]++;
		}
			
	}
}

namespace sec11_2_1
{
	void ex_11_7_add_family(map<string, vector<string>>& families,const string& firstname,const vector<string>& lastnames )
	{
		//auto sda = families[firstname]; //ֻҪ������������ţ����Զ����ַ������뵽��map��
		//sda.empty();
		if (!families[firstname].empty())
			for (size_t i = 0; i < lastnames.size(); i++)
				families[firstname].push_back(lastnames[i]);
		else
			families[firstname] = lastnames;
	}
	void printfamilies(const map<string, vector<string>>& families)
	{
		for (auto it = families.begin(); it != families.end(); it++) //map������˫�����������
		{
			cout << it->first << endl;
			for (const auto &c : it->second)
				cout << c << " ";
			cout << endl;
		}
	}
	void test_ex_11_7()
	{
		map<string, vector<string>> families;
		ex_11_7_add_family(families, "wang", vector<string>{"zk", "s", "xk"});
		ex_11_7_add_family(families, "wang", vector<string>{"zk1", "s1", "xk1"});
		ex_11_7_add_family(families, "zhang", vector<string>{"zk1", "s1", "xk1"}); 
		ex_11_7_add_family(families, "zhang", vector<string>{"zk11", "s11", "xk11"});
		printfamilies(families);
	}
	void unique_vector_str(vector<string>& input)
	{
		sort(input.begin(), input.end()); //�ñ�׼���㷨����ȥ��
		auto uniqueptr = unique(input.begin(), input.end());
		input.erase(uniqueptr, input.end());
	}
}

namespace sec11_2_2
{
	map<vector<int>::iterator, int> sasf;
	
	map<list<int>::iterator, int> agsg;
	list<int> test1122 = { 1,2,3,4 };
	vector<int> vectest1122 = { 1,2,3,4 };
	bool compisbn(const Sales_data& s1, const Sales_data& s2)
	{
		return s1.isbn() < s2.isbn();
	}
}

namespace sec_11_2_3
{
	vector <pair<string,int>> combiPair(const vector<string>& s, const vector<int>& integar)
	{
		vector <pair<string, int>> result;
		for (size_t i = 0; i < s.size(); i++)
			result.push_back({ s[i],integar[i] });
		return result;
	}
	class families //�е������ݿ����˼�� ����������
	{
		using Datatype = map<string,vector<pair<string, string>>>;
	public:
		void addchild(const string& firstname, const string& lastname, const string& birthday)
		{
			Data[firstname].push_back(make_pair(lastname, birthday));
		}
		void printData()const
		{
			for (auto const & s : Data)
			{
				cout << s.first << " ";
				for (auto const & s1 : s.second)
					cout << s1.first << " " << s1.second;
				cout << endl;
			}
		}
	private:
		Datatype Data;
	};
}

namespace sec11_3_5
{
	multimap<string, string> bookstore = {
		{ "alan", "DMA" },
		{ "pezy", "LeetCode" },
		{ "alan", "CLRS" },
		{ "wang", "FTP" },
		{ "pezy", "CP5" },
		{ "wang", "CPP-Concurrency" }
	};
	void eraseKey(multimap<string, string>& bookstore, multimap<string, string>::key_type value)
	{
		auto countnum = bookstore.count(value);
		multimap<string, string>::iterator ptrbegin = bookstore.find(value);
		for (int i = 0; i < countnum; i++)
			bookstore.erase(ptrbegin++);
	}
	void printbookstore(const multimap<string, string>& bookstore)
	{
		bool flag = 1;
		for (auto it = bookstore.begin(); it != bookstore.end(); it++)
		{
			static string tmpstore = it->first;
			if (tmpstore != it->first||flag)
			{
				flag = 0;
				tmpstore = it->first;
				cout << tmpstore << endl;
			}
			cout << it->second << endl;
		}
	}
	
	void test()
	{
		map<string, vector<int>> ex11_28 = { { "Alan",{ 1,2,3,4,5, } },{ "John",{ 1,5,6,7,8 } } };
		map<string, vector<int>>::iterator finded = ex11_28.find("Alan");
		eraseKey(bookstore, "wan1g");
		printbookstore(bookstore);
	}
}

namespace sec11_3_6
{
	ifstream ifs_map("./word_transformation.txt"), ifs_content("./given_to_transform.txt");

	void skipSpace(const string & str, string::iterator & it)
	{
		for (; it != str.end(); it++)
			if (!isspace(*it))
				break;
	}
	void skipWord(const string& str, string::iterator&it)
	{
		if (it!=str.end()&&isspace(*it))
			skipSpace(str, it);
		for (; it != str.end(); it++)
			if (isspace(*it))
				break;
	}


	map<string, string> getmap(ifstream& ifsmap)
	{
		map<string, string> transformMap;
		string linestr;
		while (getline(ifs_map, linestr))
		{
			string::iterator it = linestr.begin();
			string::iterator keyend = linestr.end();
			string::iterator mappedbegin = linestr.end();
			skipSpace(linestr, it);
			string::iterator keybegin = it;
			for ( ; it != linestr.end(); it++)
			{
				if (isspace(*it))
				{
					keyend = it;
					break;
				}
			}
			skipSpace(linestr, it);
			mappedbegin = it;
			
			string::reverse_iterator rit = linestr.rbegin();
			if (mappedbegin != linestr.end())
			{
				for (; rit != linestr.rend(); rit++)
					if (!isspace(*rit))
						break;
			}
			string key(keybegin, keyend);
			string mappedvalue(mappedbegin, rit.base());
			transformMap[key] = mappedvalue;
		}
		return transformMap;
	}

	

	void TranslateWords(const map<string, string> &maptrans, ifstream& given,ofstream& outfile)
	{
		string linestr;
		while (getline(given, linestr))
		{
			string::iterator it = linestr.begin();

			while (it != linestr.end())
			{
				skipSpace(linestr, it);
				auto wordbegin = it;
				skipWord(linestr, it);
				auto wordend = it;
				string word(wordbegin, wordend);
				
				map<string, string>::const_iterator foundword = maptrans.find(word);
				if (foundword != maptrans.end())
					outfile << maptrans.find(word)->second << " ";
				else
					outfile << word << " ";
			}
			outfile << endl;
		}
	}

	void test()
	{
		auto maptrans = getmap(ifs_map);
		TranslateWords(maptrans, ifs_content, ofstream("ex11_33out.txt"));
	}
}

namespace sec11_3_3
{
	void test()
	{
		vector<int> sec11_3_3 = { 1,2,3,4,6,7,3,2,2,1,6,3 }; //˳���������������const int ��Ϊallocator���ܼ���constԪ��
		sec11_3_3.erase(sec11_3_3.begin(), sec11_3_3.begin() + 1);
		map<string, string> maptesttype;
		map<string, string>::mapped_type sgagah = maptesttype[string("fag")];
		map<string, string>::value_type ahasfhdg = *maptesttype.begin();
		map<string, string>::key_type ahhsli = maptesttype.begin()->first;
	}
}

namespace QuerySystem
{
	void testQuerySystem(string path)
	{
		QueryResult queryres;
		{
			TextQuery test(path); //������������ˣ������ܹ��������ڲ���Ԫ��
			queryres = test.query("bird,1");
			queryres = test.query("bird,1");
			//set<decltype(vector<int>().size())> //���������Զ��ƶ�
		}
		print(std::cout, queryres);
	}
}

namespace testhasptr_chap13
{
	void test()
	{
		HasPtrValue test11("215");
		HasPtrValue test22 = test11;

		{
			HasPtrptr test1(string("1"));
			HasPtrptr test2(string("2"));
			HasPtrptr test3(string("3"));
			HasPtrptr test4 = test1;
			HasPtrptr test6 = test2;
			{
				HasPtrptr test5 = test2;
			}
		}
		
	}
}

namespace sec12_1_6
{
	ifstream ifs("./storyDataFile.txt");
	
	void ex12_20()
	{
		string line;
		vector<string> vecline;
		while (getline(ifs, line))
			vecline.push_back(line);
		const StrBlob Blob(vecline);
		for (auto it = Blob.begin(); it != Blob.end(); it.incr())
		{
			cout << it.deref() << endl;
		}
	}
}

namespace ex13_28
{
	class TreeNode
	{
	public:
		TreeNode() :left(NULL), right(NULL) {}
	private:
		string value;
		int count;
		TreeNode * left;
		TreeNode * right;
	};
}

namespace sec13_3
{
	void ex13_31()
	{
		//vector<HasPtrValue> wt = { "4","5","ah","5y" }; //���ڱ�����ֻ����һ��ת�� ���������ǲ��Ե�
		vector<HasPtrValue> wt = { string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y"),string("4"),string("5"),string("ah"),string("5y") };
		//��ͬ��ģ������sortʱ���õĺ����ǲ�ͬ�ģ�С��ģֻ����operator= ���ģʱ�������operator=��Ҫ����swap
		sort(wt.begin(), wt.end());
	}
}

#include "Message.h"
#include "Folder.h"
namespace MessageFolder
{

	void test()
	{
		Message safga;
		
	}
}
void main()
{
	try
	{
		////////////////////////////////////////////////////////////////////////////////////////////
		//sec13.3
		sec13_3::ex13_31();

		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//sec12.3
		sec12_1_6::ex12_20();
		////////////////////////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////////////////////////
		//chapter13
		testhasptr_chap13::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//sec12.3
		QuerySystem::testQuerySystem(string("./storyDataFile.txt"));
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//11.3.6��
		sec11_3_6::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//11.3.5��
		sec11_3_5::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		



		////////////////////////////////////////////////////////////////////////////////////////////
		//11.3.3��
		sec11_3_3::test();
		////////////////////////////////////////////////////////////////////////////////////////////
		



		////////////////////////////////////////////////////////////////////////////////////////////
		//11.3.1��
		map<string, vector<string>> sasf = { {string("sdafa0"),vector<string>{ "asfga0s","asgagh0" }} };
		//*sasf.begin() = make_pair(string("sdafa"),string{ "asfgas","asgagh" }); //���ڹؼ�����const���͵ģ���˲���ֱ�Ӹ�ֵ
		(*sasf.begin()).second = { "asf" };
		


		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//11.2.3��
		auto testmakepair = make_pair(Sales_data(), int(3));
		auto test11_12 = sec_11_2_3::combiPair({ "wang","zhao" }, { 1,2 });
		sec_11_2_3::families ex11_14;
		ex11_14.addchild("wang", "zk", "98.1");
		ex11_14.addchild("wang", "dsa", "99.2");
		ex11_14.addchild("zhang", "zk", "98.1");
		ex11_14.addchild("zhang", "dsa", "99.2");
		
		
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//11.2.2��
		
		//sec11_2_2::agsg[sec11_2_2::test1122.begin()] = 5; //�ᱨ��û�ж���С�ں�
		sec11_2_2::sasf[sec11_2_2::vectest1122.begin() + 3] = 8;
		sec11_2_2::sasf[sec11_2_2::vectest1122.begin() + 4] = 9; //β�������
		//sec11_2_2::test1122.begin()<sec11_2_2::test1122.end() //list�ĵ�����û��С�ڲ�����ֻ��++ --��

		multiset<Sales_data, bool(*)(const Sales_data&, const Sales_data&)> multi1122(sec11_2_2::compisbn); 
		//��һ���ô��ǣ�compisbn���ڵ�λ�ÿ��Ե����������ݵ���������ͬ�ĺ���
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//11.2.1��
		sec11_2_1::test_ex_11_7();
		vector<string> vecstr = { "af", "af", "asfa", "af", "h23","asfa" };
		sec11_2_1::unique_vector_str(vecstr);
		set<string> dd = { "af", "af", "asfa", "af", "h23","asfa" };
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//11.1��
		sec11_1::CountString(sec11_1::testdata);
		sec11_1::CountStringIgnorePuc(sec11_1::testdata);

		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//ex12_26
		allocator<string> sec12_2_2;
		auto beginptr = sec12_2_2.allocate(100);
		sec12_2_2.construct(beginptr , "dasd");
		ex12_26::input_string_output_reversestring(10);
		////////////////////////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////////////////////////
		//12.1.5��
		//int * sec12_1_5addr = new int(32);
		//{
		//	unique_ptr<int> sec12_1_5_intell(sec12_1_5addr);
		//	{
		//		unique_ptr<int> sec12_1_5_intell1(sec12_1_5addr); //ͬһ���ڴ��ͷ������Σ�����û����������ʱ�����ϵ����
		//	}
		//}
		
		

		//unique_ptr<int> ex12_16(new int(32));
		//unique_ptr<int> ex_12_16_1(ex12_16); //��������ɾ���ĺ���
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//10.4.3��
		for_each(sec10_4_1::vec.crbegin(), sec10_4_1::vec.crend(), [](int n) {cout << n << " "; });
		cout << endl;
		auto it = sec10_4_1::vec.cend();
		while (it != sec10_4_1::vec.cbegin())
			cout << *(--it) << " ";
		cout << endl;
		std::list<int> ex10_36 = { 1, 2, 3, 4, 0, 5, 6 };
		auto last0 = find(ex10_36.rbegin(), ex10_36.rend(), 0);

		vector<int> ex10_37 = { 1,5,12,4,4,3,7,8,9,6 };
		list<int> reverse;
		copy(ex10_37.cbegin() + 3, ex10_37.cbegin() + 8, front_inserter(reverse));

		////////////////////////////////////////////////////////////////////////////////////////////
		



		////////////////////////////////////////////////////////////////////////////////////////////
		//10.4.1��
		sort(sec10_4_1::vec.begin(), sec10_4_1::vec.end());
		unique_copy(sec10_4_1::vec.begin(), sec10_4_1::vec.end(), front_inserter(sec10_4_1::lst));
		copy(sec10_4_1::ex10_28.begin(), sec10_4_1::ex10_28.end(), inserter(sec10_4_1::insertercopy, sec10_4_1::insertercopy.begin()));
		//copy(sec10_4_1::ex10_28.begin(), sec10_4_1::ex10_28.end(), front_inserter(sec10_4_1::insertercopy)); // vector����push front
		////////////////////////////////////////////////////////////////////////////////////////////




		////////////////////////////////////////////////////////////////////////////////////////////
		//10.3.4��
		cout << count_if(section10_3_1::ex10_13.begin(), section10_3_1::ex10_13.end(), bind(sec10_3_4::Iflowerthansz, sec10_3_4::_1, 6)) << endl;
		cout << sec10_3_4::CheckSizeLambda(vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8}, string("gasdf")) << endl;
		cout << sec10_3_4::CheckSizeBind(vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8}, string("gasdf")) << endl;
		sec10_3_4::biggies(section10_3_1::ex10_13, 4);
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//10.3.3��
		//ex 10.20
		cout << count_if(section10_3_1::ex10_13.begin(), section10_3_1::ex10_13.end(), [](const string&s1)->bool {if (s1.size() > 6) return 1; else return 0; }) << endl;
		section10_3_2::ex10_21(10);
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//10.3.2��
		cout << section10_3_2::ex10_15(3)(4) << endl;
		section10_3_2::biggies(section10_3_1::ex10_13, 4);
		sort(section10_3_1::Sales_datav.begin(), section10_3_1::Sales_datav.end(), [](const Sales_data&s1, const Sales_data&s2) {return s1.isbn().size() < s2.isbn().size(); });
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//10.3.1��
		std::vector<std::string> v{ "1234", "1234", "1234", "Hi", "alan", "wang" };
		ex10_9::ElimDups(v);
		stable_sort(v.begin(), v.end(), section10_3_1::Isshorter);

		sort(section10_3_1::Sales_datav.begin(), section10_3_1::Sales_datav.end(), section10_3_1::compareIsbn);
		//sort(section10_3_1::ex10_13.begin(), section10_3_1::ex10_13.end(), section10_3_1::Isshorter);
		partition(section10_3_1::ex10_13.begin(), section10_3_1::ex10_13.end(), section10_3_1::IsMore5);

		////////////////////////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////////////////////////
		//10.2��
		vector<double> ex10_4 = { 1.1,3,2,5,6,2,1,5,2,3,1 };
		cout << accumulate(ex10_4.cbegin(), ex10_4.cend(), 0.0) << endl;
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//10.1
		vector<int> ex10_1_1 = { 1,3,2,1,2,3,4,1,5,7,8,5,23,7 };
		cout << count(ex10_1_1.cbegin(), ex10_1_1.cbegin()+3, 1) << endl;
		vector<string> ex10_1_2 = { "asda s","asda s","s" };
		cout << count(ex10_1_2.cbegin(), ex10_1_2.cbegin() + 3, "asda s") << endl;

		////////////////////////////////////////////////////////////////////////////////////////////
		
		
		auto sdsad = new auto(Sales_data());
		////////////////////////////////////////////////////////////////////////////////////////////
		//12.1.3��
		//shared_ptr<int> tmpsharedptr1 = new int(43); //��Դ��Ļ����ֵ�ַ������ָ��Ĺ��캯����explicit�ģ���˲����ÿ������캯�������漰����ʽת��
		shared_ptr<int> tmpsharedptr2(new int(43)); //�����Ϳ���
		{
			shared_ptr<int> tmpsharedptr3(shared_ptr<int>(new int(43))); //Ĭ�ϵ�����������delete int* �����ָ��Ĳ��Ƕ�̬�ڴ棬��Ҫ�Զ���һ��ɾ����

		}
		



		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ12.6
		vector <int> * ex12_6p = ex12_6::returnvecint();
		ex12_6p = ex12_6::readinput(ex12_6p);
		ex12_6::printvecint(ex12_6p);

		shared_ptr<vector<int>> ex12_6pIntelligence = ex12_6::returnvecintIntelligence();
		ex12_6::readinputIntelligence(ex12_6pIntelligence);
		ex12_6::printvecintIntelligence(ex12_6pIntelligence);
		////////////////////////////////////////////////////////////////////////////////////////////
		



		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ12.2
		cout << ex12_2::ss[0] << endl;
		ex12_2::ss[1] = 2;
		cout << ex12_2::ss[1] << endl;

		StrBlob StrBlob1;
		StrBlob1.push_back("ssss");
		StrBlob1.front() = "ssss1";
		
		const StrBlob StrBlob2({"uyffhgf","sfafag"});
		//StrBlob2.front() = "uyffhgfmodify";//��Ϊ��const�����Բ����޸�Ԫ�� ���������ص�front�������͵�constȥ���������޸��ˣ���Ȼ���ᱨ�����û��������
		cout << StrBlob2[1] << endl;
		//StrBlob2[1] = 1; //��ᱨ����Ϊ�Ѿ��ڳ�Ա������������ֻ������

		StrBlob StrBlob3 = StrBlob2; //�����ñ�����Ĭ�ϺϳɵĿ������캯��
		StrBlob3.push_back("FASFASF");//StrBlob2��const���͵ģ���ζ�Ų���ͨ��StrBlob2�ı乲����������ݣ�����ͨ������Blob�ı乲�����������


		////////////////////////////////////////////////////////////////////////////////////////////
		



		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ6.54
		ex6_54::testvec_pfuc(50, 2);
		ex6_54::useFp(ex6_54::fucminus, 48, 3);
		ex6_54::useFp(ex6_54::fucmultiple, 48, 3);
		cout << ex6_54::returnP2fuc(ex6_54::fucmultiple) << endl;
		int i = 1;
		cout << &i << endl; //������ָ��ͱ�����ָ��ռ�õ��ڴ���һ����
		ex6_54::Fp s0 = ex6_54::fucminus;
		//ex6_54::F s1 = ex6_54::fucminus;//�ᱨ����Ϊ����ֱ�Ӹ��������͸�ֵ û�ж������������͵�ֵ��ʲô�����뺯��ָ������
		//�����ǰѺ�������Ϊһ��ֵʹ��ʱ���ú����Զ���ת����ָ��
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ2.36
		++ex2_36::c;
		++ex2_36::a;
		++ex2_36::d;
		++ex2_36::a1;
		++ex2_36::a2;
		int e236 = 3;
		int e237 = e236;
		++e236;
		++e237;
		////////////////////////////////////////////////////////////////////////////////////////////
		


		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ2.33
		ex2_33::a = 42;
		ex2_33::b = 42;
		ex2_33::c = 42;
		//ex2_33::d = 42; //�ƶ���int*���͵ģ��ᱨ���޷���int���͵�ֵ���䵽int *����
		//ex2_33::e = 42;	//�ƶ���const int*���͵ģ��ᱨ���޷���int���͵�ֵ���䵽const int *����
		//ex2_33::f = 42;	//������ʽ�����ǿ��޸ĵ���ֵ��
		
		int * s = nullptr;
		bool(*p)(const char a, const char b);
		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ13.18
		Employee Employee1("wzk1");
		//Employee Employee2 = Employee1;//�ᱨ�������캯���Ǳ�ɾ���ĺ�����
		Employee Employee3("wzk3");
		//Employee3 = Employee1;//����ϳɵĿ�����ֵ��������ɾ����



		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ13.11 13.13
		HasPtr hasptr1311 = HasPtr("2");//����ȥ�˿������캯��

		HasPtr("3");
		cout << *psstore << endl; 
		//��һ���д�������ʱ����������ʱ���������������������������Ӧ�ü�һ��delete��̬����Ķ���
		//��Ϊ��ָ��һ����������û�ָ���뿪������ʱ��������������ִ��
		//����Ϊ��ʽ����һ������ָ�����͵ĳ�Ա����delete����ָ��Ķ���
		//����ʾ�ļ���delete psʱ������cout�����

		n13_13::X *px = new n13_13::X;
		n13_13::f(*px, *px);
		delete px;
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ7.57
		Account("wzk0");
		Account("wzk1");
		Account("wzk2");
		Account Account1("wzk3");
		Account Account2 = "232"; 
		//�����캯������explicit�󣬱����ͷǷ���
		//��������˿������캯��������Ӧ������ͨ���ַ�������Ĺ��캯������һ��ʱ������
		//Ȼ����ͨ���������캯����������ģ������Ļ�NumID++2����ʵ������++1,��˾��������˿������캯��
		//�������������캯���������б�����������ʵ�ֵ������ˣ�Ϊ�˱���numID++2�����ַ�������Ĺ�������Ϊexplicit
		//�Ա�����ʽת�����numID�����˷�
		Account Account3 = Account2;

		Account Account4("Account4");
		Account4 = Account1;
		long n = Account::read_NumID();
		Account Account5("Account5");
		Account &Account6 = Account5;
		Account &Account7 = Account4;
		Account6 = Account7;

		int intnum00 = 2;
		int intnum000 = 3;
		int &intnum01 = intnum00;
		int &intnum02 = intnum000;
		intnum01 = intnum02;
		////////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ7.58
		const vector<double> vectordouble1;
		//vectordouble1.push_back(2.1); // �ᱨ�� ��Ϊ������һ��const���͵������мӶ���

		HasPtr hs0;
		const vector<double> &vectordouble3 = hs0.teststatic6;
		const vector<double> &vectordouble2 = HasPtr::teststatic6;
		cout << HasPtr::teststatic << endl;
		cout << teststatic << endl;

		cout << HasPtr::teststatic7 << endl;

		HasPtr::teststatic8 = 2;

		//cout << HasPtr::teststatic5 << endl; //���������static������û�г�ʼ�����ᱨ���޷��������ⲿ����

		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ13.5
		HasPtr hasptr0("hasptr0");
		HasPtr hasptr1 = hasptr0; 
		//�����Զ��忽�����캯����Ե�ʣ�����hasptr1.psֵ������hasptr0.ps��
		//ָ�����һ�������ӹ���string������������ȷ����������ϳɵĽ��бȽ�
		//�����Զ��忽�����캯��ʱ���ñ������Լ��ģ�Ч���ǿ�����ַ����
		HasPtr hasptr1_1 = 5;
		//�����˿������캯������û��ִ���Զ���Ŀ������캯��������ֱ�ӵ�����ڲ���Ϊint�Ĺ��캯��
		//���û�����Ļ���Ӧ��+"2"�ģ�����û�м�
		//��Ҳ�Ƿ����߼��ģ���Ϊ�ұ��������5��Ӧ�Ķ��󸳸�hasptr1_1���Ѿ�ִ����һ�鹹���ˣ��Ͳ���Ҫ��ִ��һ��
		HasPtr hasptr1_2 = hasptr1_1;//����õ����Զ���Ŀ������캯������+"2"
		HasPtr hasptr1_3(6, 3); //������Ӱ�쿽�����캯����Ϊ�������캯���������Ų���ƥ�䷢���书��
		HasPtr hasptr2("ptr2");
		(hasptr2 = hasptr0).pulicfuc();//����ǿ�����ֵ�����
		//����Ĭ�ϺϳɵĿ�����ֵ���������ȫ��hasptr0���,���Ƕ������ַ��ͬ������ͽС�������
		//�����Զ���ģ������Զ������Щ���������и������ǣ���Ҫ�Լ��������г�Ա�����Ŀ�����ֵ��������©�������©�ĳ�Ա�������ֲ���
		cout << &hasptr0 << " " << &hasptr2 << "��С��" << sizeof(&hasptr2)<< endl;
		int intvalue = 0;
		int *intvalueadd = &intvalue;
		cout << intvalueadd << "��С��"<<sizeof(intvalueadd)<<endl; 
		//ͨ���Ա�����ĵ�ַ��ʽ���Լ���ַ�Ĵ�С������ָ��ͬ�����ָ����ռ�ڴ�ռ���һ����
		////////////////////////////////////////////////////////////////////////////////////////////
		
		
		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ7.53
		Debug Debug0;
		Debug Debug1(true,false,true);
		Debug Debug2(2, 3, 1);

		double debuga=3, debugb=2, debugc=1;
		Debug Debug3(debuga, debugb, debugc);
		//Ҫ�뷢��constexpr�����ԣ���������������ʱǰ�߼���constexpr��
		//����������ӵĻ������ǿ���ͨ��������ֵ�ģ�constexpr��ͬ����
		cout << Debug3.get_bw();
		Debug3.set_bw(0);  //�����ǿ����޸ĵ�
		cout << Debug3.get_bw();

		//constexpr Debug Debug4_1(debuga, debugb, debugc); //�ᱨ����ʽ�б��뺬�г���
		constexpr Debug Debug4(3, 2, 1); //�����ɳ�����ֵ�󣬾���ȷ�ˡ�
		Debug4.get_bw();
		//Debug4.set_bw(0); //����Ǳ���ģ���ΪDebug4������֤�����ڲ���ֵ�ڱ���ʱ���Ѿ�ȷ������������ʱ�ǲ��ܸ��
		
		Debug Debug5(true);
		bool boolva = 1;
		Debug Debug6(boolva);
		const Debug Debug7(true);//�����ڱ����ھ�ȷ��ֵ�ĳ��� ��constexprûʲô��ϵ
		//constexpr Debug Debug8(true);//�������ʽ���뺬�г���ֵ
		//�ڹ��캯���У���û�ж�Debug(bool b ) :bw(b), io(b), other(b)������캯������constexpr
		//������ڹ��캯����������constexpr�����ϱ���һ�в�������ζ�ſ��Թ���constexpr����

		//���ϣ�constexpr��һ���ȽϹ㷺��ѡ����Խ�һ���������밲ȫ��Ҳ������һ��������constexpr������ͨ��������
		////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////
		//��ϰ7.49
		Sales_data Sales_data0("333");
		Sales_data & Sales_data1 = Sales_data0.returnRef(Sales_data("333"));
		//��������һ����ʱ���������ã���Ȼ���ᱨ������Sales_data1��ֵ��Ĭ�Ϲ��칹������ģ�û��"333"
		//�ϱ�����ƺ��´������һ�� Ŀǰ�������Ϊʲô����һ���ֲ����û�work
		Sales_data & Sales_data2 = Sales_data0.returnRef(Sales_data0);
		//��������Sales_data0�����ã���Ϊ�䲻��һ����ʱ�����������ʵֵȷʵ��"333"

		Sales_data Sales_data3;
		Sales_data & Sales_data4 = Sales_data0.combine(Sales_data3);

		Sales_data Sales_data5("345");
		Sales_data Sales_data6 = Sales_data0.returnValue(Sales_data5);

		double sad = 4;
		//�������explicit����ô�����ܰ����±ߵĲ���������ʽת��
		Sales_data0.combine(3.3);//�Զ���ʽת������һ��const Sales_data
		Sales_data0.combine(sad);//��ʱ����һ����const�͵� ��Ϊ������double�ͱ���������ʽת�������õĹ��캯������const����
		////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////
		int n1118 = 1;
		int &n11181 = n1118;
		int &n11182 = n11181;
		n1118 = 3; //���Զ����ö������

		Nodefault moren;
		const Nodefault &moren1 = moren.returnRef(moren);
		const Nodefault &moren2 = moren.returnRef(2);
		//��������ʽת��Ϊ����ʱ���࣬���û���������Ϊ�����Ĺ��캯���Ļ��ᱨ��û���ʵ��Ĳ�������ת��

		//Nodefault & moren3 = moren.returnrefnotconst(3);
		////��ʽ�ǻᱨ��ģ���������ǿ��޸ĵ���ֵ��
		////��C++primer P55��֪���������ÿ��԰�����ʱ���ϣ����Ƿǳ����ò����԰�����ʱ���ϡ�
		////��ʽ������ʽ������һ��ʱ����Ȼ������������ʱ������һ�ǳ��������ϣ����ǷǷ���

		Nodefault moren5;
		Nodefault & moren6 = moren.returnrefnotconst(moren5); //�����Ϳ����� ���ǳ������ð����˱�����

		moren5.data++;
		const Nodefault&ref2tmp = Nodefault(6); 
		//���ڴ����еģ���Ϊ���԰󶨳����ã�û�е����������� �����һ�����ɼ���ʱ�����ͷ����������
		Nodefault(6); //��ʱ����������������Ͳ��ɷ�����, �ͷ����ڴ���,��������������

		Nodefault(6).returnRef(moren1); //��ʱ����ֻ����ֻ����
		Nodefault(6).returnrefnotconst(moren5).data++; //���ǰ�moren5��data++��һ��
		//Nodefault(6).returnRef(moren1).data++; //����const�����ƣ�ʹ�ò��ܱ��޸�
		//Nodefault(6).data++; //������ʽ�����ǿ��޸ĵ���ֵ������Ϊ����ʱ���������ܱ��޸�

		////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////
		C ctmp;
		cout << endl;
		vector<C> tmpvectornodefault(10,ctmp);
		Nodefault nodefaulttmp;
		vector<Nodefault> tmpvectornodefault1(10, nodefaulttmp);
		cout << endl;
		vector<Nodefault> tmpvectornodefault2(10); //�����Ļ��ǹ���ʮ��
		cout << endl;
		vector<Nodefault> tmpvectornodefault3(10, Nodefault(1)); //������ֻ����һ�Σ�Ȼ����ʮ��
		////////////////////////////////////////////////////////////////////////////////////////////



		////���±��������Ӧ����δ�������Ϊ�ģ����ǿ���VS2015��������һЩ�Ż����Ϳ��Զ�����Ϊconst��ֵ�����޸�
		////������Ϊ��δ�������Ϊ�����Ǳ�������Ϊconst����������Щ��Ϊʹ��const��ֵ�仯�ˣ��������Ԥ�Ƶĳ������к��
		////��ζ�ų���������ȱ�ݣ��������������ƽ̨���ǿ���ͨ���ģ�ֻ�������׼���
		//const int j = 3; // j ����Ϊconst����
		//int* pj = const_cast<int*>(&j);
		//*pj = 4;         // ��ȥconst��ָ��ȥ�޸�һ��ԭ�����ǳ�����j��ֵ��undefined behavior!

		const int intnum = 5;
		const int intconst1 = intnum;
		//intconst1 = 34; //��������ǿ��޸ĵ���ֵ������ͨ�����õ�const_cast���޸� ���Ǻܲ��Ƽ���
		//const_cast<int> (intconst1) //���������ָ�롢���� ��Ϊֻ���޸ĵײ�const����
		CastConstZK(intconst1) = 34; //����һ�¾Ͱѱ���Ӧ����const��ֵ���޸���


		const int * intconst = &intnum;
		const int & ref = intnum;
		//const int & &ss = ref; //�����ͻᱨ����ʹ�ö����õ�����
		const int & ss = ref; //�������ᱨ������õ�����
		const_cast<int &>(ref) = 100;
		//int * intnotconstnotcast = intconst;
		*(const_cast<int *>(intconst)) =1;
		int * intnotconst = const_cast <int *> (intconst);
		*intnotconst = 6;

		//NotThrow();
		vector<int> vectorint(1,1);
		vectorint.push_back(10);
		const screen constscreen(5);
		screen tmpscreen2(3,4);
		//screen tmpscreen3; //��ʱ�ᱨ��������Ĭ�Ϲ��캯��

		/*screen tmpscreen4(); //��ʱ�ᱨ����붨�������ͣ���Ϊ����tmpscreen4()�����˺�������
		cout << tmpscreen4.data << endl;*/

		screen tmpscreen(2);
		screen tmpscreen1(3);
		tmpscreen1 = tmpscreen; //�ƺ��������Լ�������һ����ֵoperator
		constscreen.display() = tmpscreen; //��������� Ϊʲô��������ֵ ��Ϊ�������Ѹñ��ʽ����Ϊ��operator = 
										   //��effective C++���б���
		constscreen.display().changedata(100); // �����constscreen���κβ���

		cout << testinline() << endl;
		cout << inlinetest() << endl;

		int a = 5;
		int * Locate_a = &a;
		int ** locloc_a = &Locate_a;
		int *** loclocloc_a = &locloc_a;
		cout << Locate_a << endl;
		cout << locloc_a << endl;
		cout << loclocloc_a << endl;

		//using namespace Exercise;
		using Exercise::ivar;
		//using Exercise::dvar;
		using Exercise::limit;

		fuc();
		using Exercise::cout;
		std::cout << cout << endl;

		double dvar = 3.1415;

		int iobj = limit + 1;
		++ivar;
		++::ivar;
	}
	catch (exception d)
	{
		cout << d.what();
	}
	
	system("pause");
}