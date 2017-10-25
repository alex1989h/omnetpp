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

#define WORKING_TIME_BOY_MEAN  2.0
#define WORKING_TIME_BOY_STDDEV 0.2

#define WORKING_TIME_GIRL_MEAN 1.0
#define WORKING_TIME_GIRL_STDDEV 0.2

#define WORKING_TIME_FALLEN_MEAN 10.0
#define WORKING_TIME_FALLEN_STDDEV 3.0

#define DOF 1

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

    workingTimeFallen = 0;
    workingTimeFallenSum = 0;
    workingTimeThrow = 0;
    workingTimeThrowSum = 0;
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

            scheduleAt(simTime() + getWorkingTimeFallen(verteilungNumber,WORKING_TIME_FALLEN_MEAN, WORKING_TIME_FALLEN_STDDEV), self);

        } else { //Bekommt einen Ball vom Spiderman
            if (currentBallCount < MAX_BALL_COUNT) { //Ist noch Platz frei
                EV << this->getFullName() << ": \"Ball\" gefangen\"" << std::endl;
                if (strcmp("boy", this->getFullName()) == 0 || strcmp("boy2", this->getFullName()) == 0) {
                    scheduleAt(simTime() + getWorkingTimeThrow(verteilungNumber, WORKING_TIME_BOY_MEAN, WORKING_TIME_BOY_STDDEV), self);
                } else {
                    scheduleAt(simTime() +getWorkingTimeThrow(verteilungNumber, WORKING_TIME_GIRL_MEAN, WORKING_TIME_GIRL_STDDEV), self);
                }
            } else { //Kein Paltz frei
                EV << this->getFullName() << ": \"Ball\" fallen gelassen" << std::endl;

                verlorenCounter();   ///Hier zählt er die nicht gefangenen Bälle

                scheduleAt(simTime() + getWorkingTimeFallen(verteilungNumber,WORKING_TIME_FALLEN_MEAN,WORKING_TIME_FALLEN_STDDEV), self);
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
    WATCH(workingTimeThrow);
    WATCH(workingTimeThrowSum);
    WATCH(workingTimeFallen);
    WATCH(workingTimeFallenSum);

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
    case 1:
        EV << "Normalverteilung" << std::endl;
        break;
    case 2:
        EV << "Exponentialverteilung" << std::endl;
        break;
    case 3:
        EV << "Student-Verteilung" << std::endl;
        break;
    default:
        EV << "Keine Verteilung" << std::endl;
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

double Actor::getWorkingTimeFallen(int number, double mean, double stddev){
    switch (number) {
    case 1:workingTimeFallen = truncnormal(mean,stddev);
        break;
    case 2:workingTimeFallen = exponential(mean);
        break;
    case 3:workingTimeFallen = fabs(student_t(DOF)+mean);
        break;
    default:workingTimeFallen = mean;
        break;
    }
    return workingTimeFallenSum += workingTimeFallen;
}

double Actor::getWorkingTimeThrow(int number, double mean, double stddev){
    switch (number) {
    case 1:
        workingTimeThrow = truncnormal(mean, stddev);
        break;
    case 2:
        workingTimeThrow = exponential(mean);
        break;
    case 3:
        workingTimeThrow = fabs(student_t(DOF) + mean);
        break;
    default:
        workingTimeThrow = mean;
        break;
    }
    return workingTimeThrowSum += workingTimeThrow;
}
