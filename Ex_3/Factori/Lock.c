#include "Lock.h"


Lock* InitializeLock(void)
{
    Lock* lock = NULL;
	lock = (Lock*)malloc(sizeof(Lock));
    if (lock == NULL) {
        printf("Failed to allocate memory for lock object\n");
        return NULL;
    }
    lock->readers = 0;

    // semaphore creation:
    lock->semaphore_roomEmpty = CreateSemaphore(
        NULL,                       // default security attributes
        semaphore_starting_value,   // initial count
        semaphore_max_value,        // maximum count
        NULL);                      // unnamed semaphore
    if (lock->semaphore_roomEmpty == NULL)
    { 
        printf("CreateSemaphore error: %d\n", GetLastError());
        return NULL;
    }

    // mutex creation:
    lock->Mutex = CreateMutex(
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL);             // unnamed mutex

    if (lock->Mutex == NULL)
    {
        printf("CreateMutex error: %d\n", GetLastError());
        return NULL;
    }

    //// mutex creation:
    //lock->turnstile = CreateMutex(
    //    NULL,              // default security attributes
    //    FALSE,             // initially not owned
    //    NULL);             // unnamed mutex

    //if (lock->turnstile == NULL)
    //{
    //    printf("CreateMutex error: %d\n", GetLastError());
    //    return STATUS_CODE_FAILURE;
    //}
    // //turnstile

    return lock;
}



int read_lock(Lock* lock) {
    DWORD wait_code;

    ////turnstile:

    //wait_code = WaitForSingleObject(
    //    lock->turnstile,    // handle to mutex
    //    WAIT_TWO_MINUTES);  // no time-out interval
    //if (wait_code != WAIT_OBJECT_0) {
    //    return STATUS_CODE_FAILURE;
    //}
    //if (!ReleaseMutex(lock->turnstile))
    //{
    //    printf("Release Mutex error: %d\n", GetLastError());
    //    return STATUS_CODE_FAILURE;
    //}
    // //turnstile




    wait_code = WaitForSingleObject(
        lock->Mutex,        // handle to mutex
        WAIT_TWO_MINUTES);  // time-out interval
    if (wait_code != WAIT_OBJECT_0) {
        return STATUS_CODE_FAILURE;
    }
    lock->readers += 1;
    if (lock->readers == 1) {
        // wait on semaphore (wait for writer to finish)
        wait_code = WaitForSingleObject(
            lock->semaphore_roomEmpty,  // handle to semaphore
            WAIT_TWO_MINUTES);          // wait two minutes at most, as instructed
        if (wait_code != WAIT_OBJECT_0) {
            return STATUS_CODE_FAILURE;
        }
    }

    if (!ReleaseMutex(lock->Mutex))
    {
        printf("Release Mutex error: %d\n", GetLastError());
        return STATUS_CODE_FAILURE;
    }
    return STATUS_CODE_SUCCESS;
}


int read_release(Lock* lock) {
    DWORD wait_code;
    wait_code = WaitForSingleObject(
        lock->Mutex,        // handle to mutex
        WAIT_TWO_MINUTES);  // time-out interval
    if (wait_code != WAIT_OBJECT_0) {
        return STATUS_CODE_FAILURE;
    }

    lock->readers -= 1;
    if (lock->readers == 0) {
        // last reader release
        if (!ReleaseSemaphore(
            lock->semaphore_roomEmpty,  // handle to semaphore
            1,                          // increase count by one
            NULL))                      // not interested in previous count
        {
            printf("Release Semaphore error: %d\n", GetLastError());
            return STATUS_CODE_FAILURE;
        }
    }
    if (!ReleaseMutex(lock->Mutex))
    {
        printf("Release Mutex error: %d\n", GetLastError());
        return STATUS_CODE_FAILURE;
    }

    return STATUS_CODE_SUCCESS;
}

int write_lock(Lock* lock) {
    DWORD wait_code;

    ////turnstile:

    //wait_code = WaitForSingleObject(
    //    lock->turnstile,    // handle to mutex
    //    WAIT_TWO_MINUTES);  // no time-out interval
    //if (wait_code != WAIT_OBJECT_0) {
    //    return STATUS_CODE_FAILURE;
    //}
    // //turnstile

    wait_code = WaitForSingleObject(
        lock->semaphore_roomEmpty,  // handle to semaphore
        WAIT_TWO_MINUTES);          // wait two minutes at most, as instructed
    if (wait_code != WAIT_OBJECT_0) {
        return STATUS_CODE_FAILURE;
    }
}

int write_release(Lock* lock) {
    //if (!ReleaseMutex(lock->turnstile))
    //{
    //    printf("Release Mutex error: %d\n", GetLastError());
    //    return STATUS_CODE_FAILURE;
    //}
    // //turnstile
    if (!ReleaseSemaphore(
        lock->semaphore_roomEmpty,  // handle to semaphore
        1,                          // increase count by one
        NULL))                      // not interested in previous count
    {
        printf("Release Semaphore error: %d\n", GetLastError());
        return STATUS_CODE_FAILURE;
    }
}

void DestroyLock(Lock* lock) {
    free(lock);
}