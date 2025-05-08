//
// Created by qwqb233 on 2025/5/5.
//

#ifndef CPP_TEST_THREAD_LOCKER_H
#define CPP_TEST_THREAD_LOCKER_H


class thread_locker {
public:
    thread_locker() = default;
    ~thread_locker() = default;

private:
    bool lock_ = false;
};


#endif //CPP_TEST_THREAD_LOCKER_H
