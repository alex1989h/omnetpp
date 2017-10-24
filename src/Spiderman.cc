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
Define_Module(Spiderman);
using namespace omnetpp;
#define WORKING_TIME_BOY  2.0
#define WORKING_TIME_GIRL 1.0
#define WORKING_TIME_FALLEN 10.0

#define WORKING_TIME_SPIDERMAN 4.0

#define MAX_BALL_COUNT 2

Spiderman::Spiderman() {
    currentBallCount = 0;

}

Spiderman::~Spiderman() {
    // TODO Auto-generated destructor stub
}

void Spiderman::handleMessage(omnetpp::cMessage *msg) {
    if (msg->isSelfMessage()) {
        omnetpp::cMessage *ball = new omnetpp::cMessage(msg->getOwner()->getFullName());
        currentBallCount--;
        if (strcmp("girl", msg->getFullName()) == 0 || strcmp("girl2", msg->getFullName()) == 0) {
//            cnt_wuerfe_boy++;
//            wuerfe_boy.record(cnt_wuerfe_boy);
            if (uniform(0, 1) < 0.5) {
                send(ball, "out2");
            } else {
                send(ball, "out4");
            }
            EV << this->getFullName() << ": Werfe \"Ball\" zum Jungen, hab noch "<< currentBallCount <<" \"Bälle\"" << std::endl;
        } else if (strcmp("boy", msg->getFullName()) == 0 || strcmp("boy2", msg->getFullName()) == 0) {
//            cnt_wuerfe_boy++;
//            wuerfe_boy.record(cnt_wuerfe_boy);
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

        delete msg;
    } else {
        omnetpp::cMessage *self = NULL;
        cnt_wuerfe_ges++;
        wuerfe_ges.record(cnt_wuerfe_ges);

        if (currentBallCount < MAX_BALL_COUNT) {//Spiderman hat genug Plätze
            EV << this->getFullName() << " \"Ball\" gefangen vom " << msg->getFullName() << std::endl;;
            self = new omnetpp::cMessage(msg->getFullName());
            scheduleAt(simTime() + WORKING_TIME_SPIDERMAN, self);
            currentBallCount++;
        } else {//Kann nicht fangen senden ERROR zu Absender
            EV << this->getFullName() << ": \"Ball\" fallen gelassen, " << msg->getFullName() << " bescheid geben " << std::endl;;
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
        delete msg;
    }
}

void Spiderman::initialize() {

}

void Spiderman::finish() {

}

