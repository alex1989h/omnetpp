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
Spiderman::Spiderman() {
    // TODO Auto-generated constructor stub

}

Spiderman::~Spiderman() {
    // TODO Auto-generated destructor stub
}

void Spiderman::handleMessage(omnetpp::cMessage *msg) {
    EV << "Versuche Ball zu fangen" << std::endl;

    if (msg->isSelfMessage()) {
        omnetpp::cMessage *ball = new omnetpp::cMessage(
                msg->getOwner()->getFullName());
        if (strcmp("girl", msg->getFullName()) == 0
                || strcmp("girl2", msg->getFullName()) == 0) {
//            cnt_wuerfe_boy++;
//            wuerfe_boy.record(cnt_wuerfe_boy);
            EV << "IsSelfMessage " << msg->getFullName() << std::endl;
            if (uniform(0, 1) < 0.5) {
                send(ball, "out2");
            } else {
                send(ball, "out4");
            }
        } else if (strcmp("boy", msg->getFullName()) == 0
                || strcmp("boy2", msg->getFullName()) == 0) {
//            cnt_wuerfe_boy++;
//            wuerfe_boy.record(cnt_wuerfe_boy);
            EV << "IsSelfMessage " << msg->getFullName() << std::endl;
            if (uniform(0, 1) < 0.5) {
                send(ball, "out");
            } else {
                send(ball, "out3");
            }
        } else {
            EV << "Spiderman: unbekanne Nachricht " << msg->getFullName()
                      << std::endl;
        }
        delete msg;
    } else {
        EV << "Nicht Eigentümer " << msg->getFullName() << std::endl;
        omnetpp::cMessage *self = new omnetpp::cMessage(msg->getFullName());
        cnt_wuerfe_ges++;
        wuerfe_ges.record(cnt_wuerfe_ges);
        EV << "\"Ball \"gefangen\n";
        if (strcmp("boy", this->getFullName()) == 0) {
            scheduleAt(simTime() + WORKING_TIME_BOY, self);
        } else {
            scheduleAt(simTime() + WORKING_TIME_GIRL, self);
        }

        delete msg;
    }
}

void Spiderman::initialize() {

}

void Spiderman::finish() {

}

