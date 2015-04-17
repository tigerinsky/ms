#ifndef __THREAD_SAFE_H_
#define __THREAD_SAFE_H_

#include <pthread.h>

//读写锁
class ReadWriteLock {
public:
    //constrctor
    ReadWriteLock() {
        pthread_rwlock_init(&_rwlock, NULL);
    }

    //destructor
    ~ReadWriteLock() {
        pthread_rwlock_destroy(&_rwlock);
    }

    //lock for read
    bool read_lock() {
        int ret = pthread_rwlock_rdlock(&_rwlock);
        return 0 == ret;
    }

    //unlock for read
    void read_unlock() {
        pthread_rwlock_unlock(&_rwlock);
    }

    //lock for write
    bool write_lock() {
        int ret = pthread_rwlock_wrlock(&_rwlock);
        return 0 == ret;
    }

    //unlock for write
    void write_unlock() {
        pthread_rwlock_unlock(&_rwlock);
    }

private:
    pthread_rwlock_t _rwlock;
};

//读写锁的读锁
class ScopedReadLock {
public:
    ScopedReadLock(ReadWriteLock* rwlock)
        : _rwlock(rwlock) {
        _rwlock->read_lock();
    }

    ~ScopedReadLock() {
        _rwlock->read_unlock();
    }
private:
    ReadWriteLock* _rwlock;
};

//读写锁的写锁
class ScopedWriteLock {
public:
    ScopedWriteLock(ReadWriteLock* rwlock)
        : _rwlock(rwlock) {
        _rwlock->write_lock();
    }

    ~ScopedWriteLock() {
        _rwlock->write_unlock();
    }
private:
    ReadWriteLock* _rwlock;
};

#endif
