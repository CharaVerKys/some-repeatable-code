#pragma once
#include <QThread>
#include <cstdarg>

// ? void* itself in any cpp file
extern void* mainThreadID;
extern void* logThreadID;
extern void* extThreadID;

// * you can change "pointer to pointer" to "reference on pointer"
// * checkThread(&mainThreadID) -> checkThread(mainThreadID)
// * (i prefer explicitly use '&')
#ifndef NDEBUG

inline void checkAllThreadIDsCollision() {
    std::vector<void*> threadIDs = {
        mainThreadID,
        logThreadID,
        extThreadID
    };

    for (size_t i = 0; i < threadIDs.size(); ++i) {
        if (threadIDs[i]) {
            for (size_t j = i + 1; j < threadIDs.size(); ++j) {
                if (threadIDs[j]) {
                    assert(threadIDs[i] != threadIDs[j] && "Thread ID collision");
                }
            }
        }
    }
}

#define NOTmainThread assert(QThread::currentThreadId() not_eq mainThreadID);

inline void checkThread(void** thread)
{                   
    if(!*thread){
        *thread = (QThread::currentThreadId()); 
    }else{                              
        assert(*thread == QThread::currentThreadId()); 
    }
    checkAllThreadIDsCollision(); 
}

inline void checkThreadIsNOT(void** thread)
{
    assert(*thread);
    assert(*thread not_eq QThread::currentThreadId()); 
}

inline void checkThread(int count...)
{
    va_list args;
    assert(count>=1);
    uint8_t allCount = count;
    va_start(args, count); // warning by clang: UB if count have type uint8_t or uint16_t (1/2 bytes), Ok if uint // idk why
    bool was_eq = false;
    void** aThread = nullptr;
    while(count){
        aThread = va_arg(args, void**);
        if(allCount == 1 and QThread::currentThreadId() not_eq *aThread){assert(false);}
        if(not *aThread){
            va_end(args);
            return;
        }
        if(*aThread == QThread::currentThreadId()){
            was_eq = true;
        }
        --count;
    }//while
    va_end(args);
    assert(count == 0);
    assert(was_eq);
}

#else
#define NOTmainThread (void)0; 
inline void checkThread(void** thread){(void)thread;}
inline void checkThreadIsNOT(void** thread){(void)thread;}
inline void checkThread(int count...){(void)count;}
#endif