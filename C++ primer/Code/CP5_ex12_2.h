#ifndef CP5_ex12_2_h
#define CP5_ex12_2_h

#include <string>
#include <iostream>
#include <memory>



class StrBlob
{
	friend class StrBlobPtr;
public:
	typedef std::vector<std::string>::size_type size_type;
	//���캯��
	//�������캯��ΪĬ�Ϻϳɵ� ��Ϊû�б�Ҫд����
	StrBlob() :data(std::make_shared<std::vector<std::string>>()) {}
	StrBlob(std::initializer_list<std::string> i1) :data(std::make_shared<std::vector<std::string>>(i1)) {}
	StrBlob(const vector<string>& vecstr) :data(std::make_shared<std::vector<std::string>>(vecstr)) {}

	StrBlob(const StrBlob& in) :data(make_shared<vector<string>>(*in.data)) {} //��ֵ�Ŀ������캯�� ���������Ĭ�ϵĻ�������ָ���
	StrBlob& operator=(const StrBlob& in) //��const���εĻ����÷�Χ����Щ ��ֵ�Ŀ�����ֵ���� �������Ĭ�ϵĻ�������ָ���
	{
		data = make_shared<vector<string>>(*in.data);
		return *this;
	}


	//��ȡdata����
	size_type size() const { /*data = NULL;//������const�Ĵ��� ���Բ��ܸ���Ա���ݸ�ֵ*/ return data->size(); }
	bool empty() const { return data->empty(); }
	//��Ӻ�ɾ��Ԫ��
	void push_back(const std::string &input) { data->push_back(input); }
	void pop_back() { check(0, "dataָ���Ԫ��Ϊ�գ�"); data->pop_back(); }
	//Ԫ�ط���
	std::string& front() { check(0, "dataָ���Ԫ��Ϊ�գ�"); return data->front(); };
	std::string& back() { check(0, "dataָ���Ԫ��Ϊ�գ�"); return data->back(); };
	const std::string& front()const { check(0, "dataָ���Ԫ��Ϊ�գ�"); return data->front(); };
	const std::string& back()const { check(0, "dataָ���Ԫ��Ϊ�գ�"); return data->back(); };
	//�������ƺ�ߵ�constǿ�����ǳ�ָ�룬������ζ��ָ����ָ�Ķ���ҲΪ���������ﺯ���ķ������ͱ����const
	//��Ϊ��Ȼ�������أ���ôֻ�е�const Strblobʱ�Ż���룬��ô�������const���������ͣ��Ǿ���ζ�ſ����޸�ָ���Ԫ�أ����Υ���û��ĳ��ԡ�
	std::string & operator[](size_type i) { check(i, "�������磡"); return (*data)[i]; }
	const std::string & operator[](size_type i) const { check(i, "�������磡"); return (*data)[i]; }//���Ҳ�ǣ����뷵������Ϊconst�Ų����û��������
	//std::shared_ptr<std::vector<std::string>>& returndata() const { return data; }
	//�����ᱨ����Ϊ������һ���ǳ��������ˣ�ȥ����const���ԡ�
	//���������Ļ��ǲ���ȫ�ģ���Ϊ���returndata�����˷�const���ͣ���ô���Լ���޸�const���͵�strblob����һ������

	//������ֵ����ҲΪĬ�Ϻϳɵ� û�б�Ҫд���� ��Ϊֻ��һ��data

	StrBlobPtr begin()const ;
	StrBlobPtr end()const ;
private:
	std::shared_ptr<std::vector<std::string>> data;
	//void check(size_type i, const std::string &msg);//��������������һ����const�͵������ һ������ͨ�����
	void check(size_type i, const std::string &msg)const;
};



class StrBlobPtr //����һ��ָ��strblob�ĵ�����
{
public:
	StrBlobPtr() :curr(0) {}
	StrBlobPtr(const StrBlob&in, size_t index = 0) :wptr(in.data), curr(index) {} //StrBlob�������const�Ļ��Ͳ�������const StrBlob
	string& deref()const //ÿ�ν����ö�Ҫ�ж��Ƿ����� ����ͼ��Ӧ��β��ָ��ʱ���׳�����
	{
		auto data = Check(curr);
		return (*data)[curr];
	}
	StrBlobPtr& incr() //ǰ׺���� ��νǰ׺��ָǰ�պ󿪵�ǰ׺
	{
		Check(curr);
		curr++;
		return *this;
	}
	bool operator==(const StrBlobPtr& s2)const
	{
		auto shareptrthis = wptr.lock();
		auto shareptrs2 = s2.wptr.lock();
		if (!shareptrthis || !shareptrs2)
			throw exception("shared_ptr�Ѿ����ͷ���");
		else
		{
			if (shareptrthis == shareptrs2 && curr == s2.curr)
				return 1;
			else
				return 0;
		}
	}
	bool operator!=(const StrBlobPtr& s2)const
	{
		return !operator==(s2);
	}
private:
	shared_ptr<vector<string>> Check(size_t index) const //����������� һ��������Ƿ��ͷ� ����������Ƿ񳬳�ά��
	{
		auto p = wptr.lock(); //�ƺ�ֻ��ͨ�����ַ��������ж�ָ���shareptr�Ƿ��ͷ�
		if (!p) //˵��shared_ptr�Ѿ����ͷ���
			throw exception("shared_ptr�Ѿ����ͷ���");
		if ( index>= p->size())
			throw exception("��������ά��");
		return p;
	}
	size_t curr;
	weak_ptr<vector<string>> wptr; //weak_ptr�󶨵�share_ptr
};

void StrBlob::check(size_type i, const std::string &msg)const
{

	if (i >= size())
	{
		throw std::out_of_range(msg);
	}
}
StrBlobPtr StrBlob::begin()const { return StrBlobPtr(*this, 0); }
StrBlobPtr StrBlob::end()const { return StrBlobPtr(*this, data->size()); }
#endif