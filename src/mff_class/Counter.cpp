
#include <boost/utility.hpp>
#include <boost/noncopyable.hpp>

#ifndef CLASS_COUNTER
#define CLASS_COUNTER

class Counter : boost::noncopyable
{
    // copy-ctor and assignment should be private by default for a class.
  public:
    Counter() : value_(0) {}
    int64_t value() const;
    int64_t getAndIncrease();

  private:
    int64_t value_;
    mutable MutexLock mutex_;
};
int64_t Counter::value() const
{
    MutexLockGuard lock(mutex_); // lock 的析构会晚于返回对象的构造,
    return value_;
    // 因此有效地保护了这个共享数据。
}
int64_t Counter::getAndIncrease()
{
    MutexLockGuard lock(mutex_);
    int64_t ret = value_++;
    return ret;
}

#endif