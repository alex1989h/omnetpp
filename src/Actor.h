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

#ifndef ACTOR_H_
#define ACTOR_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>


class Actor: public omnetpp::cSimpleModule {
protected:
    omnetpp::cOutVector wuerfe_girl;
    omnetpp::cOutVector wuerfe_boy;
    omnetpp::cOutVector wuerfe_lost;

    int cnt_wuerfe_girl;
    int cnt_wuerfe_boy;
    int cnt_wuerfe_lost;
public:
    Actor();
    virtual ~Actor();
    virtual void handleMessage(omnetpp::cMessage *msg) override;
    virtual void initialize() override;
    virtual void finish() override;
};

#endif /* ACTOR_H_ */
