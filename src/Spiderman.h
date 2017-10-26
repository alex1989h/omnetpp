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

#ifndef SPIDERMAN_H_
#define SPIDERMAN_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

class Spiderman  : public omnetpp::cSimpleModule{
protected:
        omnetpp::cOutVector v_cnt_throw;
        omnetpp::cOutVector v_cnt_lost;

        int cnt_throw;
        int cnt_lost;
public:
    Spiderman();
    virtual ~Spiderman();
    virtual void handleMessage(omnetpp::cMessage *msg) override;
    virtual void initialize() override;
    virtual void finish() override;
    void geworfenCounter(void);
    void verlorenCounter(void);
private:
    double getWorkingTimeThrow(int,double,double);
    double workingTimeThrow;
    double workingTimeThrowSum;

    int currentBallCount;
    int MAX_THROW_LIMIT;
    bool THROW_LIMIT_ON;
};

#endif /* SPIDERMAN_H_ */
