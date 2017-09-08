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

#include <src/Actor.h>
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

Define_Module(Actor);
using namespace omnetpp;

#define WORKING_TIME_BOY  2.0
#define WORKING_TIME_GIRL 1.0
#define WORKING_TIME_FALLEN 10.0

Actor::Actor() {
    wuerfe_ges.setName("anzahl-wuerfe-gesamt");
    wuerfe_girl.setName("anzahl-wuerfe-girl");
    wuerfe_boy.setName("anzahl-wuerfe-boy");
    wuerfe_lost.setName("anzahl-wuerfe-lost");
    cnt_wuerfe_ges = 0;
    cnt_wuerfe_girl = 0;
    cnt_wuerfe_boy = 0;
    cnt_wuerfe_lost = 0;
}

Actor::~Actor() {
    // TODO Auto-generated destructor stub
}

void Actor::finish()
{
    // This function is called by OMNeT++ at the end of the simulation.
    EV << "Gesamt:     "            << cnt_wuerfe_ges << endl;
    EV << "Junge: "                 << cnt_wuerfe_boy << endl;
    EV << "Mädchen:    "            << cnt_wuerfe_girl << endl;
    EV << "Fallen gelassen :    "   << cnt_wuerfe_lost << endl;

    recordScalar("#cnt_wuerfe_ges", cnt_wuerfe_ges);
    recordScalar("#cnt_wuerfe_girl", cnt_wuerfe_girl);
    recordScalar("#cnt_wuerfe_boy", cnt_wuerfe_boy);
    recordScalar("#cnt_wuerfe_lost", cnt_wuerfe_lost);
}

void Actor::handleMessage(omnetpp::cMessage *msg){
    EV << "Versuche Ball zu fangen" << std::endl;

    if(msg->isSelfMessage()){
        omnetpp::cMessage *ball = new omnetpp::cMessage("ball");
        if(strcmp("boy",msg->getOwner()->getFullName()) == 0){
            cnt_wuerfe_boy++;
            wuerfe_boy.record(cnt_wuerfe_boy);
        }
        else{
            cnt_wuerfe_girl++;
            wuerfe_girl.record(cnt_wuerfe_girl);
        }
        send(ball,"out");
        delete msg;
    }
    else{
        omnetpp::cMessage *self = new omnetpp::cMessage("self");
        // Fallen lassen
        if (uniform(0, 1) < 0.1) { // 10% der Bälle
              cnt_wuerfe_lost++;
              wuerfe_lost.record(cnt_wuerfe_lost);
              EV << "\"Ball \"fällt\n";
              scheduleAt(simTime()+WORKING_TIME_FALLEN,self);
        }
        else{
            cnt_wuerfe_ges++;
            wuerfe_ges.record(cnt_wuerfe_ges);
            EV << "\"Ball \"gefangen\n";
            if(strcmp("boy",this->getFullName()) == 0){
                scheduleAt(simTime()+WORKING_TIME_BOY,self);
            }
            else{
                scheduleAt(simTime()+WORKING_TIME_GIRL,self);
            }
        }
       delete msg;
    }
}

void Actor::initialize(){
    EV << "Init " << this->getFullName() << std::endl;
    if(strcmp("boy",this->getFullName()) == 0){
        cnt_wuerfe_boy++;
        wuerfe_boy.record(cnt_wuerfe_boy);
        EV << "First Message " << this->getFullName() << std::endl;
        omnetpp::cMessage *ball = new omnetpp::cMessage("ball");
        send(ball,"out");
    }
}
