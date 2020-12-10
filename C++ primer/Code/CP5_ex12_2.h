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
	StrBlob() :data(std::make_shared<vector<std::string>>()) {}
	StrBlob(std::initializer_list<std::string> i1) :data(std::make_shared<vector<std::string>>(i1)) {}
	//��ȡdata����
	size_type size() const { /*data = NULL;//������const�Ĵ��� ���Բ��ܸ���Ա���ݸ�ֵ*/ return data->size(); }
	bool empty() const { return data->empty(); }
	//��Ӻ�ɾ��Ԫ��
	void push_back(const std::string &input) { data->push_back(input); }
	void pop_back() { check(0, "dataָ���Ԫ��Ϊ�գ�"); data->pop_back(); }
	//Ԫ�ط���
	std::string& front() { check(0, "dataָ���Ԫ��Ϊ�գ�"); return data->front(); };
	std::string& back() { check(0, "dataָ���Ԫ��Ϊ�գ�"); return data->back(); };
	//std::string& front()const { check(0, "dataָ���Ԫ��Ϊ�գ�"); return data->front(); };
	//std::string& back()const { check(0, "dataָ���Ԫ��Ϊ�գ�"); return data->back(); };
	std::string & operator[](size_type i) { check(i, "�������磡"); return (*data)[i]; }
	//������ֵ����ҲΪĬ�Ϻϳɵ� û�б�Ҫд���� ��Ϊֻ��һ��data
private:
	std::shared_ptr<std::vector<std::string>> data;
	//void check(size_type i, const std::string &msg);//��������������һ����const�͵������ һ������ͨ�����
	void check(size_type i, const std::string &msg)const;
};

void StrBlob::check(size_type i, const std::string &msg)const
{
	StrBlob::front();
	if (i >= size())
	{
		throw std::out_of_range(msg);
	}
}


#endif