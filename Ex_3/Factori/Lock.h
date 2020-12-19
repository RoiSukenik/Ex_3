#ifndef LOCK_H_FILE
#define LOCK_H_FILE
#define STATUS_CODE_FAILURE 1
#define STATUS_CODE_SUCCESS 0

typedef struct _Lock {

}Lock;

/*
* Description - Initialize Lock
* Parameters - None
* Returns - Pointer to a Lock struct
*/
Lock* InitializeLock(void);

/*
* Description - locks for reading
* Parameters - Pointer to Lock struct
* Returns - FAILURE or SUCCESS
*/
int lock_read(Lock *lock);

/*
* Description - release lock
* Parameters - Pointer to Lock struct
* Returns - NONE
*/
int read_release(Lock* lock);

/*
* Description - locks for writing & reading
* Parameters - Pointer to Lock struct
* Returns - FAILURE or SUCCESS
*/
int write_lock(Lock* lock);

/*
* Description - release lock
* Parameters - Pointer to Lock struct
* Returns - NONE
*/
int write_release(Lock* lock);


/*
* Description - destroy lock
* Parameters - Pointer to Lock struct
* Returns - Free all memory resources, set pointer to NULL
*/
void DestroyLock(Lock* lock);
#endif // !LOCK_H_FILE
