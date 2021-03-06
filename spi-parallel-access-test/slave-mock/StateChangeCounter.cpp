/**
 * Copyright (C) 2016  Jonas Sticha (jonas.sticha@tum.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <poll.h>
#include <cstring>
#include <iostream>
#include "StateChangeCounter.h"

StateChangeCounter::StateChangeCounter(vector<GpioReader*>& pinReaders) :
        doCount(true),
        numPins((int) pinReaders.size()),
        stateChanged((long*) malloc(sizeof(long) * numPins)),
        pinReaders(pinReaders) {
    memset(stateChanged, 0, sizeof(long)*numPins);
}

void StateChangeCounter::countStateChanges() {
    doCount = true;
    int lastState[numPins];
    struct pollfd fdset[numPins];
    memset((void*) fdset, 0, sizeof(fdset));
    for(int i = 0; i < numPins; i++) {
        lastState[i] = 1;
        fdset[i].fd = pinReaders.at(i)->getFileDescriptor();
        fdset[i].events = POLLPRI;
    }
    int currState[numPins];
    memset(currState, 0, sizeof(currState));
    int concurrentlyActiveCounter = 0;
    while(doCount) {
        poll(fdset, numPins, 10*1000);
        int activeSlaves = 0;
        for(int i = 0; i < numPins; i++) {
            currState[i] = pinReaders.at(i)->getPinStatus();
            if(lastState[i] == 1 &&  currState[i] == 0) {
                stateChanged[i]++;
            }
            if(currState[i] == 0)
                activeSlaves++;
            lastState[i] = currState[i];
            fdset[i].revents = 0;
        }
        if(activeSlaves > 1) {
            concurrentlyActiveCounter++;
            cerr << "More than one CS active at the same time!" << endl;
        }
    }
}

void StateChangeCounter::stopCounting() {
    doCount = false;
}

void StateChangeCounter::printState() {
    cout << "Number state changes per pin id:" << endl;
    cout << "ID\tNAME\tStateChanges" << endl;
    cout << "----------------------------------------------" << endl;
    for(int i = 0; i < numPins; i++) {
        cout << i << "\t" << pinReaders.at(i)->getPinId() << "\t" << stateChanged[i] << endl;
    }
    cout << "----------------------------------------------" << endl;
}
