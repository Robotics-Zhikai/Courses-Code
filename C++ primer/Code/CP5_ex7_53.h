#ifndef CP5_ex7_53_h
#define CP5_ex7_53_h

class Debug
{
public:
	/*constexpr*/ explicit Debug(bool b ) :bw(b), io(b), other(b) {  }
	constexpr Debug(bool h, bool i, bool o) : bw(h), io(i), other(o) {/*std::cout << "ss" << std::endl;*/ /*�ᱨ����䲻�ܳ�����constexpr������*/ } //��ζ�ű����ڱ����ڸ�������
	//���ǣ�Ҫ�뷢��constexpr�����ԣ���������������ʱǰ�߼���constexpr������������ӵĻ������ǿ���ͨ��������ֵ��
	constexpr Debug():bw(true),io(true),other(true) {} //ֻҪ������constexpr���ͱ����ڱ����ھ͸���Ա��������ֵ
	//Debug(bool h, bool i, bool o) : bw(h), io(i), other(o) {}//�����ض���

	void set_bw(bool b) { bw = b; } //�����ΪҪ�޸ĳ�Ա���ݣ����Բ��ܱ�constexpr�޶�����Ϊconstexpr������ֻ����return
	constexpr bool get_bw() { return bw; }
private:
	bool bw;
	bool io;
	bool other;
};

#endif