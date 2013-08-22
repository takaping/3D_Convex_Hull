/* 
 * Subject class
 *  - subject of Observer pattern
 * File:   Subject.h
 * Author: munehiro
 *
 * Created on May 10, 2013, 10:42 PM
 */

#ifndef SUBJECT_H
#define	SUBJECT_H

#include <vector>
#include <memory>

using namespace std;

class Observer;

class Subject {
public:
    Subject();
    virtual ~Subject();
    void addObserver(const shared_ptr<Observer>& obs);
    void deleteObserver(const shared_ptr<Observer>& obs);
    void notifyObservers();
private:
    vector<shared_ptr<Observer>> obss;  // observers

};

#endif	/* SUBJECT_H */

