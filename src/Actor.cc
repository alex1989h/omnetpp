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

Define_Module(Actor);
using namespace omnetpp;

#define WORKING_TIME_BOY  2.0
#define WORKING_TIME_GIRL 1.0
#define WORKING_TIME_FALLEN 10.0

Actor::Actor() {
}

Actor::~Actor() {
    // TODO Auto-generated destructor stub
}

void Actor::finish()
{
    // This function is called by OMNeT++ at the end of the simulation.
//    EV << "Gesamt:     "            << cnt_wuerfe_ges << endl;
    EV << getFullName() << " gefangen:" << cnt << endl;

    EV << getFullName() <<" fallen gelassen :"   << cnt_lost << endl;


    recordScalar("#cnt_wuerfe_maedchen", cnt);
    recordScalar("#cnt_wuerfe_verloren", cnt_lost);
}

void Actor::handleMessage(omnetpp::cMessage *msg){
    EV << "Versuche Ball zu fangen" << std::endl;

    if(msg->isSelfMessage()){
        omnetpp::cMessage *ball = new omnetpp::cMessage("ball");
        cnt++;
        v_cnt.record(cnt);
        send(ball,"out");
        delete msg;
    }
    else{
        omnetpp::cMessage *self = new omnetpp::cMessage("self");
        // Fallen lassen
        if (uniform(0, 1) < 0.1) { // 10% der Bälle
              cnt_lost++;
              v_lost.record(cnt_lost);
              EV << "\"Ball \"fällt\n";
              scheduleAt(simTime()+WORKING_TIME_FALLEN,self);
        }
        else{

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


    v_cnt.setName("#anzahl-wuerfe");
    v_lost.setName("#anzahl-verlorener-wuerfe");
    cnt = 0;
    cnt_lost = 0;

    EV << "Init " << this->getFullName() << std::endl;
    if(strcmp("boy",this->getFullName()) == 0){
        cnt++;
        v_cnt.record(cnt);
        EV << "First Message " << this->getFullName() << std::endl;
        omnetpp::cMessage *ball = new omnetpp::cMessage("ball");
        send(ball,"out");
    }
}
