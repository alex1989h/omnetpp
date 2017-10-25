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

#define MEAN 5
#define STDDEV 2.5
#define DOF 4
//Max SIZES
#define MAX_BALL_COUNT 1

// Static members
int Actor::cnt_all_throw;
cOutVector* Actor::v_cnt_all_throw = 0;
bool Actor::statsAlreadyRecorded = false;
bool Actor::globalAlreadyInitialized = false;
int Actor::verteilungNumber = 0;

Actor::Actor() {
    if (!globalAlreadyInitialized) {
        v_cnt_all_throw = new cOutVector("#anzahl-geworfen-gesamt");
        globalAlreadyInitialized = true;
            }
    currentBallCount = 0;
    workingTimeSum = 0;
}

Actor::~Actor() {
    if (NULL != v_cnt_all_throw) {
        delete v_cnt_all_throw;
        v_cnt_all_throw = NULL;
    }
}

/**
 * Finalize
 */
void Actor::finish() {

    // This function is called by OMNeT++ at the end of the simulation.
    if (!statsAlreadyRecorded) {
        EV << "Gesamt: " << cnt_all_throw << endl;
        recordScalar("Total: ", cnt_all_throw);
        statsAlreadyRecorded = true;
    }

    EV << this->getFullName() << " geworfen: " << cnt_throw << endl;
    EV << this->getFullName() << " fallen gelassen: " << cnt_lost << endl;

    recordScalar("#cnt_baelle_geworfen", cnt_throw);
    recordScalar("#cnt_baelle_verloren", cnt_lost);

}

/**
 * Perform
 */
void Actor::handleMessage(omnetpp::cMessage *msg) {
    if (msg->isSelfMessage()) {
        omnetpp::cMessage *ball = new omnetpp::cMessage(this->getFullName());
        geworfenCounter();      ///Hier zählt er die geworfenen Bälle

        send(ball, "out");
        currentBallCount--;
        EV << this->getFullName() << ": \"Ball\" geworfen, hab noch "<< currentBallCount <<" \"Bälle\"" << std::endl;
    } else {
        omnetpp::cMessage *self = new omnetpp::cMessage(msg->getFullName());

        if (strcmp("ERROR", msg->getFullName()) == 0) { //Bekommt eine Nachricht vom Spiderman "Ball nicht gefange"
            EV << this->getFullName() << ": Spiderman hat den \"Ball\" nicht gefangen, ich muss ihn aufsammeln" << std::endl;

            scheduleAt(simTime() + getWorkingTimeFallen(verteilungNumber), self);

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

                verlorenCounter();   ///Hier zählt er die nicht gefangenen Bälle

                scheduleAt(simTime() + getWorkingTimeFallen(verteilungNumber), self);
            }
        }
        currentBallCount++;
    }
        delete msg;
}

/**
 * Initialize
 */
void Actor::initialize() {
    verteilungNumber = par("verteilungNumber");
    v_cnt_throw.setName("#v_cnt_baelle_geworfen");
    v_cnt_lost.setName("#v_cnt_baelle_verloren");

    cnt_throw = 0;
    cnt_lost = 0;

    statsAlreadyRecorded = false;
    cnt_all_throw = 0;

    EV << "Init " << this->getFullName() << std::endl;
    EV << "Verteilung für \"Ball\" hollen: ";
    switch (verteilungNumber) {//Ausgabe welche Verteilung genommen wurde
    case 0:
        EV << "Normalverteilung" << std::endl;
        break;
    case 1:
        EV << "Exponentialverteilung" << std::endl;
        break;
    case 2:
        EV << "Student-Verteilung" << std::endl;
        break;
    default:
        EV << "unbekannte Verteilung" << std::endl;
        break;
    }
    if (strcmp("boy", this->getFullName()) == 0 || strcmp("boy2", this->getFullName()) == 0) {
        EV << "Erste Nachricht " << this->getFullName() << std::endl;
        omnetpp::cMessage *ball = new omnetpp::cMessage(this->getFullName());
        send(ball, "out");
    }
}

void Actor::geworfenCounter() {
    cnt_throw++;
    cnt_all_throw++;
    v_cnt_throw.record(cnt_throw);
    v_cnt_all_throw->record(cnt_all_throw);
}

void Actor::verlorenCounter() {
    cnt_lost++;
    v_cnt_lost.record(cnt_lost);
}

int Actor::getWorkingTimeFallen(int number){

    switch (number) {
    case 0:workingTimeSum += truncnormal(MEAN,STDDEV);
        break;
    case 1:workingTimeSum += exponential(MEAN);
        break;
    case 2:workingTimeSum += fabs(student_t(DOF));
        break;
    default:workingTimeSum = MEAN;
        break;
    }
    return workingTimeSum;
}
