#ifndef CP5_ex12_2_h
#define CP5_ex12_2_h

#include <string>
#include <iostream>
#include <memory>

class StrBlob
{
public:
	typedef std::vector<std::string>::size_type size_type;
	//���캯��
	//�������캯��ΪĬ�Ϻϳɵ� ��Ϊû�б�Ҫд����
	StrBlob() :data(std::make_shared<std::vector<std::string>>()) {}
	StrBlob(std::initializer_list<std::string> i1) :data(std::make_shared<std::vector<std::string>>(i1)) {}

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
private:
	std::shared_ptr<std::vector<std::string>> data;
	//void check(size_type i, const std::string &msg);//��������������һ����const�͵������ һ������ͨ�����
	void check(size_type i, const std::string &msg)const;
};

void StrBlob::check(size_type i, const std::string &msg)const
{
	
	if (i >= size())
	{
		throw std::out_of_range(msg);
	}
}


#endif