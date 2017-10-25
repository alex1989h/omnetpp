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

#include <src/Spiderman.h>
#include <src/Actor.h>
Define_Module(Spiderman);
using namespace omnetpp;

#define WORKING_TIME_SPIDERMAN_MEAN 4.0
#define WORKING_TIME_SPIDERMAN_STDDEV 1.0

#define MAX_BALL_COUNT 2

#define MAX_CNT_THROW 1000
#define STOP_IF_REACH_MAX_THROW false

#define DOF 1


Spiderman::Spiderman() {
    if (!Actor::globalAlreadyInitialized) {
        Actor::v_cnt_all_throw = new cOutVector("#anzahl-geworfen-gesamt");
        Actor::globalAlreadyInitialized = true;
    }
    currentBallCount = 0;
    workingTimeThrow = 0;
    workingTimeThrowSum = 0;

}

Spiderman::~Spiderman() {
    // TODO Auto-generated destructor stub
}

void Spiderman::handleMessage(omnetpp::cMessage *msg) {
    if(cnt_throw >= MAX_CNT_THROW && STOP_IF_REACH_MAX_THROW){//Hier sollte er dann keine Event erstellen
        delete msg;
        return;
    }
    if (msg->isSelfMessage()) {
        omnetpp::cMessage *ball = new omnetpp::cMessage(msg->getOwner()->getFullName());
        geworfenCounter();

        currentBallCount--;
        if (strcmp("girl", msg->getFullName()) == 0 || strcmp("girl2", msg->getFullName()) == 0) {
            if (uniform(0, 1) < 0.5) {
                send(ball, "out2");
            } else {
                send(ball, "out4");
            }
            EV << this->getFullName() << ": Werfe \"Ball\" zum Jungen, hab noch "<< currentBallCount <<" \"Bälle\"" << std::endl;
        } else if (strcmp("boy", msg->getFullName()) == 0 || strcmp("boy2", msg->getFullName()) == 0) {
            if (uniform(0, 1) < 0.5) {
                send(ball, "out");
            } else {
                send(ball, "out3");
            }
            EV << this->getFullName() << ": Werfe \"Ball\" zum Mädchen, hab noch "<< currentBallCount <<" \"Bälle\"" << std::endl;
        } else {
            EV << this->getFullName() << ": unbekanne Nachricht " << msg->getFullName()
                      << std::endl;
        }
    } else {
        omnetpp::cMessage *self = NULL;
        if (currentBallCount < MAX_BALL_COUNT) {//Spiderman hat genug Plätze
            EV << this->getFullName() << " \"Ball\" gefangen vom " << msg->getFullName() << std::endl;
            self = new omnetpp::cMessage(msg->getFullName());
            scheduleAt(simTime() + getWorkingTimeThrow(Actor::verteilungNumber,WORKING_TIME_SPIDERMAN_MEAN,WORKING_TIME_SPIDERMAN_STDDEV), self);
            currentBallCount++;
        } else {//Kann nicht fangen senden ERROR zu Absender
            EV << this->getFullName() << ": \"Ball\" fallen gelassen, " << msg->getFullName() << " bescheid geben " << std::endl;;

            verlorenCounter();

            self = new omnetpp::cMessage("ERROR");
            if (strcmp("boy", msg->getFullName()) == 0) {
                send(self, "outCom");
            } else if (strcmp("boy2", msg->getFullName()) == 0) {
                send(self, "out2Com");
            } else if (strcmp("girl", msg->getFullName()) == 0) {
                send(self, "out3Com");
            } else if (strcmp("girl2", msg->getFullName()) == 0) {
                send(self, "out4Com");
            } else {
                EV << "Spiderman: unbekanne Nachricht " << msg->getFullName() << std::endl;
            }
        }
    }
        delete msg;
}

void Spiderman::initialize() {
    WATCH(workingTimeThrow);
    WATCH(workingTimeThrowSum);

    v_cnt_throw.setName("#v_cnt_baelle_geworfen");
    v_cnt_lost.setName("#v_cnt_baelle_verloren");
    EV << "Init " << this->getFullName() << std::endl;
    EV << "Verteilung für \"Ball\" hollen: ";
    switch (Actor::verteilungNumber) {//Ausgabe welche Verteilung genommen wurde
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
    cnt_throw = 0;
    cnt_lost = 0;
}

void Spiderman::finish() {
    // This function is called by OMNeT++ at the end of the simulation.
        if (!Actor::statsAlreadyRecorded) {
            EV << "Gesamt: " << Actor::cnt_all_throw << endl;
            recordScalar("Total: ", Actor::cnt_all_throw);
            Actor::statsAlreadyRecorded = true;
        }

        EV << this->getFullName() << " geworfen:" << cnt_throw << endl;
        EV << this->getFullName() << " fallen gelassen :" << cnt_lost << endl;

        recordScalar("#cnt_baelle_geworfen", cnt_throw);
        recordScalar("#cnt_baelle_verloren", cnt_lost);
}
void Spiderman::geworfenCounter() {
    cnt_throw++;
    Actor::cnt_all_throw++;
    v_cnt_throw.record(cnt_throw);
    Actor::v_cnt_all_throw->record(Actor::cnt_all_throw);
}

void Spiderman::verlorenCounter() {
    cnt_lost++;
    v_cnt_lost.record(cnt_lost);
}

double Spiderman::getWorkingTimeThrow(int number, double mean, double stddev) {
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
