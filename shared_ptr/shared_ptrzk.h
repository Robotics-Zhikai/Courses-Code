#include <exception>
#include <mutex>

template<typename T>
class shared_ptrzk {
public:
	shared_ptrzk() = default;

	explicit shared_ptrzk(T * ptr) :manage(ptr) {
		//ֻҪptr==nullptr����ô���й����privateָ�������Ϊnullptr
		//ֻҪptr!=nullptr����ô���׳��쳣�����˳�����������й����privateָ���������Ϊnullptr
		if (manage != nullptr) {
			try {
				refCount = new int(1);
				mutexPtr = new std::mutex;
			}
			catch(std::exception e){
				delete ptr;
				exit(0);
			}
		}
	}

	shared_ptrzk(const shared_ptrzk& other) :manage(other.manage), refCount(other.refCount), mutexPtr(other.mutexPtr) {
		if (manage != nullptr) {
			mutexPtr->lock();
			(*refCount)++;
			mutexPtr->unlock();
		}
	}

	shared_ptrzk& operator=(const shared_ptrzk& other) {
		if (this != &other) {
			if (manage != other.manage) {

				if (manage != nullptr) {
					mutexPtr->lock();
					(*refCount)--;
					mutexPtr->unlock();

					if (*refCount == 0) {
						delete manage;
						delete refCount;
						delete mutexPtr;
					}
				}

				manage = other.manage;
				mutexPtr = other.mutexPtr;
				refCount = other.refCount;

				if (manage != nullptr) {
					mutexPtr->lock();
					(*refCount)++;
					mutexPtr->unlock();
				}
			}
		}
		return *this;
	}

	shared_ptrzk(shared_ptrzk&& other):manage(other.manage),refCount(other.refCount),mutexPtr(other.mutexPtr) {
		other.manage = nullptr;
		other.refCount = nullptr;
		other.mutexPtr = nullptr;
	}

	shared_ptrzk& operator= (shared_ptrzk&& other) {
		manage = other.manage;
		refCount = other.refCount;
		mutexPtr = other.mutexPtr;
		
		other.manage = nullptr;
		other.refCount = nullptr;
		other.mutexPtr = nullptr;

		return *this;
	}

	~shared_ptrzk() {
		if (manage != nullptr) {
			mutexPtr->lock();
			(*refCount)--;
			mutexPtr->unlock();

			if (*refCount == 0) {
				delete manage;
				delete refCount;
				delete mutexPtr;
			}
		}
	}

	int use_count()const {
		if (manage != nullptr)
			return *refCount;
		else
			return 0;
	}

	T & operator*() {
		if (manage != nullptr) {
			return *manage;
		}
		else {
			throw std::exception("*nullptr");
		}
	}

private:
	T* manage = nullptr;
	int * refCount = nullptr;
	std::mutex * mutexPtr = nullptr;
};

//ʵ��ʱ��Ҫ�������̰߳�ȫ�͹������ָ��Ϊnullptrʱ�ı߽����