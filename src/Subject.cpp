/* 
 * File:   Subject.cpp
 * Author: munehiro
 * 
 * Created on May 10, 2013, 10:42 PM
 */

#include <algorithm>
#include "Subject.h"
#include "Observer.h"

/**
 * Constructor and Destructor
 */
Subject::Subject() {
}

Subject::~Subject() {
}

/**
 * Adds the observer
 * @param observer
 */
void Subject::addObserver(const shared_ptr<Observer>& obs) {
    if (find(obss.begin(), obss.end(), obs) == obss.end()) {
        obss.push_back(obs);
    }
}

/**
 * Deletes the observer
 * @param observer
 */
void Subject::deleteObserver(const shared_ptr<Observer>& obs) {
    auto it = find(obss.begin(), obss.end(), obs);
    if (it != obss.end()) {
        obss.erase(it);
    }
}

/**
 * Notifies observers to update
 */
void Subject::notifyObservers() {
    for_each(obss.begin(), obss.end(), [this](const shared_ptr<Observer>& obs) {
        obs->update(this);
    });
}

