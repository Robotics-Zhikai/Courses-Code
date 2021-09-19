#include <exception>
#include <mutex>

template<typename T>
class shared_ptrzk {
public:
	shared_ptrzk() = default;

	explicit shared_ptrzk(T * ptr) :manage(ptr) {
		//只要ptr==nullptr，那么所有管理的private指针变量都为nullptr
		//只要ptr!=nullptr，那么不抛出异常导致退出的情况下所有管理的private指针变量都不为nullptr
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

//实现时主要考虑了线程安全和管理的裸指针为nullptr时的边界情况