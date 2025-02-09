#pragma once

#include <stdlib.h>

#ifdef __unix__
#include <unistd.h>
#endif

#include "err.hpp"
#include "global.hxx"

namespace ods {
#ifdef __linux__
class ODS_API AutoCloseFd {
public:
	AutoCloseFd(int fd): fd_(fd) {}
	~AutoCloseFd() {
		if (fd_ != -1)
			::close(fd_);
	}
private:
	NO_ASSIGN_COPY_MOVE(AutoCloseFd);
	int fd_ = -1;
};
#endif

template <class A_Type> class ODS_API AutoFree {
public:
	AutoFree(A_Type x) : x_(x) {}
	virtual ~AutoFree() { free(x_); }
	
private:
	//NO_ASSIGN_COPY_MOVE(AutoFree);
	A_Type x_ = nullptr;
};

template <class A_Type> class ODS_API AutoDelete {
public:
	AutoDelete(A_Type x) : x_(x) {}
	virtual ~AutoDelete() { delete x_; }
	
private:
	//NO_ASSIGN_COPY_MOVE(AutoDelete);
	A_Type x_ = nullptr;
};

template <class A_Type> class ODS_API AutoDeleteArr {
public:
	AutoDeleteArr(A_Type x) : x_(x) {}
	virtual ~AutoDeleteArr() { delete [] x_; x_ = nullptr; }
	
private:
	//NO_ASSIGN_COPY_MOVE(AutoDeleteArr);
	A_Type x_ = nullptr;
};

template <class VecType> class ODS_API AutoDeleteVec {
public:
	AutoDeleteVec(VecType &x) : vec_(x) {}
	~AutoDeleteVec() {
		for (auto *item : vec_) {
			delete item;
		}
	}
	
private:
	//NO_ASSIGN_COPY_MOVE(AutoDeleteVec);
	VecType &vec_;
};

template <class VecType> class ODS_API AutoDeleteVecP {
public:
	AutoDeleteVecP(VecType &x) : vec_(x) {}
	~AutoDeleteVecP() {
		for (auto *item : *vec_) {
			delete item;
		}
		delete vec_;
	}
	
private:
	//NO_ASSIGN_COPY_MOVE(AutoDeleteVecP);
	VecType vec_;
};

class ODS_API SwitchToFalseLater {
public:
	SwitchToFalseLater(bool *b): b_(b) {}
	~SwitchToFalseLater() {
		if (b_ != nullptr)
			*b_ = false;
	}
private:
	//NO_ASSIGN_COPY_MOVE(SwitchToFalseLater);
	bool *b_ = nullptr;
};
}
