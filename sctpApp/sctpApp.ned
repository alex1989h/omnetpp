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

package inet.examples.sctpApp;

import inet.applications.sctpapp.SCTPServer;
import inet.examples.inet.routerperf.BurstHost;
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;


channel ROUTER extends DatarateChannel
{
    parameters:
        per = 0;
        datarate = 1Mbps;
        
}

channel C1 extends DatarateChannel
{
    parameters:
        per = 0.01;
        datarate = 1Mbps;
        delay = 5ms;
}
channel C2 extends DatarateChannel
{
    parameters:
        per = 0;
        datarate = 1Mbps;
        delay = 5ms;
}

//
// TODO auto-generated type
//
network sctpApp
{
    @display("bgb=645,470");
    submodules:
        AlexSctp: StandardHost {
            @display("p=51,87");
            gates:
                pppg[1];
        }
        Marvin: Router {
            @display("p=188,211");
            gates:
                pppg[3];
        }
        configurator: IPv4NetworkConfigurator {
            @display("p=117,395");
        }
        ArturSctp: StandardHost {
            @display("p=68,329");
            gates:
                pppg[1];
        }
        Marvin2: Router {
            @display("p=415,211");
            gates:
                pppg[3];
        }
        WilhelmSctp: StandardHost {
            @display("p=531,73");
            gates:
                pppg[1];
        }
        MarvinSctp: StandardHost {
            @display("p=522,333");
            gates:
                pppg[1];
        }
    connections:
        AlexSctp.pppg[0] <--> C1 <--> Marvin.pppg[0];
        ArturSctp.pppg[0] <--> C2 <--> Marvin.pppg[2];
        Marvin.pppg[1] <--> ROUTER <--> Marvin2.pppg[0];
        Marvin2.pppg[1] <--> ROUTER <--> WilhelmSctp.pppg[0];
        Marvin2.pppg[2] <--> ROUTER <--> MarvinSctp.pppg[0];



}
