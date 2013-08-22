/* 
 * Observer class
 *  - observer of Observer pattern
 * File:   Observer.h
 * Author: munehiro
 *
 * Created on May 10, 2013, 10:43 PM
 */

#ifndef OBSERVER_H
#define	OBSERVER_H

class Subject;

class Observer {
public:
    Observer();
    virtual ~Observer();
    // updates observer
    // @param subject
    virtual void update(const Subject* sbj) = 0;
private:

};

#endif	/* OBSERVER_H */

