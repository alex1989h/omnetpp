//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Actor.h"
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

// For the NED files
Define_Module(Actor);
using namespace omnetpp;

// Symbols
#define WORKING_TIME_BOY  2.0
#define WORKING_TIME_GIRL 1.0
#define WORKING_TIME_FALLEN 10.0

#define AUFSAMMELN_TIME_BOY 3.0
#define AUFSAMMELN_TIME_GIRL 2.0

//Max SIZES
#define MAX_BALL_COUNT 1

// Static members
int Actor::cnt_all;
cOutVector* Actor::v_cnt_all = 0;
bool Actor::statsAlreadyRecorded = false;
bool Actor::globalAlreadyInitialized = false;

Actor::Actor() {
    if (!globalAlreadyInitialized) {
        v_cnt_all = new cOutVector("#anzahl-wuerfe-gesamt");
        globalAlreadyInitialized = true;
    }
    currentBallCount = 0;
}

Actor::~Actor() {
    if (NULL != v_cnt_all) {
        delete v_cnt_all;
        v_cnt_all = NULL;
    }
}

/**
 * Finalize
 */
void Actor::finish() {

    // This function is called by OMNeT++ at the end of the simulation.
    if (!statsAlreadyRecorded) {
        EV << "Gesamt: " << cnt_all << endl;
        recordScalar("Total: ", cnt_all);
        statsAlreadyRecorded = true;
    }

    EV << getFullName() << " gefangen:" << cnt << endl;
    EV << getFullName() << " fallen gelassen :" << cnt_lost << endl;
    recordScalar("#cnt_wuerfe_maedchen", cnt);
    recordScalar("#cnt_wuerfe_verloren", cnt_lost);

}

/**
 * Perform
 */
void Actor::handleMessage(omnetpp::cMessage *msg) {
    if (msg->isSelfMessage()) {
        omnetpp::cMessage *ball = new omnetpp::cMessage(this->getFullName());
        cnt++;
        v_cnt.record(cnt);

        cnt_all++;
        v_cnt_all->record(cnt_all);

        send(ball, "out");
        currentBallCount--;
        EV << this->getFullName() << ": \"Ball\" geworfen, hab noch "<< currentBallCount <<" \"Bälle\"" << std::endl;
        delete msg;
    } else {
        omnetpp::cMessage *self = new omnetpp::cMessage(msg->getFullName());

        if (strcmp("ERROR", msg->getFullName()) == 0) { //Bekommt eine Nachricht vom Spiderman "Ball nicht gefange"
            EV << this->getFullName() << ": Spiderman hat den \"Ball\" nicht gefangen, ich muss ihn aufsammeln" << std::endl;
            if (strcmp("boy", this->getFullName()) == 0 || strcmp("boy2", this->getFullName()) == 0) {
                scheduleAt(simTime() + WORKING_TIME_FALLEN, self);
            } else {
                scheduleAt(simTime() + WORKING_TIME_FALLEN, self);
            }
        } else { //Bekommt einen Ball vom Spiderman
            if (currentBallCount < MAX_BALL_COUNT) { //Ist noch Platz frei
                EV << this->getFullName() << ": \"Ball\" gefangen\"" << std::endl;
                if (strcmp("boy", this->getFullName()) == 0 || strcmp("boy2", this->getFullName()) == 0) {
                    scheduleAt(simTime() + WORKING_TIME_BOY, self);
                } else {
                    scheduleAt(simTime() + WORKING_TIME_GIRL, self);
                }
            } else { //Kein Paltz frei
                EV << this->getFullName() << ": \"Ball\" fallen gelassen" << std::endl;
                if (strcmp("boy", this->getFullName()) == 0 || strcmp("boy2", this->getFullName()) == 0) {
                    scheduleAt(simTime() + WORKING_TIME_FALLEN, self);
                } else {
                    scheduleAt(simTime() + WORKING_TIME_FALLEN, self);
                }
            }
        }
        currentBallCount++;
        delete msg;
    }
}

/**
 * Initialize
 */
void Actor::initialize() {

    v_cnt.setName("#anzahl-wuerfe");
    v_lost.setName("#anzahl-verlorener-wuerfe");

    cnt = 0;
    cnt_lost = 0;

    statsAlreadyRecorded = false;
    cnt_all = 0;

    EV << "Init " << this->getFullName() << std::endl;
    if (strcmp("boy", this->getFullName()) == 0 || strcmp("boy2", this->getFullName()) == 0 || strcmp("girl", this->getFullName()) == 0) {
        cnt++;
        v_cnt.record(cnt);

        cnt_all++;
        v_cnt_all->record(cnt_all);

        EV << "Erste Nachricht " << this->getFullName() << std::endl;
        omnetpp::cMessage *ball = new omnetpp::cMessage(this->getFullName());
        send(ball, "out");
    }
}
