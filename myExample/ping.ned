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

package inet.examples.myExample;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;


channel ROUTER extends DatarateChannel
{
    parameters:
        per = 0.0005;
        datarate = 1Mbps;
        delay = 0.4s;
}

//
// TODO auto-generated type
//
network Ping
{
    @display("bgb=645,470");
    submodules:
        Alex: StandardHost {
            @display("p=117,73");
            gates:
                pppg[1];
        }
        Artur: StandardHost {
            @display("p=545,103");
            gates:
                pppg[1];
        }
        Marvin: Router {
            @display("p=314,211");
            gates:
                pppg[2];
        }
        configurator: IPv4NetworkConfigurator {
            @display("p=117,395");
        }
    connections:
        Alex.pppg[0] <--> ROUTER <--> Marvin.pppg[0];
        Marvin.pppg[1] <--> ROUTER <--> Artur.pppg[0];

}
