/* 
 * Memory class
 *  - releases the heap memory
 * File:   Memory.h
 * Author: munehiro
 *
 * Created on May 12, 2013, 5:19 PM
 */

#ifndef MEMORY_H
#define	MEMORY_H

class Memory {
public:
    template<typename T>
    // releases the memory
    // @param pointer
    static void release(T** p) {
        if (*p) {
            delete *p;
            *p = nullptr;
        }
    }
    // releases the memory array
    // @param pointer to the matrix
    template<typename T>
    static void releaseArray(T** p) {
        if (*p) {
            delete [] *p;
            *p = nullptr;
        }
    }
    virtual ~Memory();
private:
    Memory();

};

#endif	/* MEMORY_H */

