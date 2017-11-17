//
// Copyright (C) 2006 Andras Varga
//
// This library is free software, you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation;
// either version 2 of the License, or any later version.
// The library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//


package wpp_eth_exp.simulations;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;


network OneSwitch
{
    @display("bgb=784,589");
    //    @display("bgb=932,543");
    types:
        channel ethline extends DatarateChannel
        {
            delay = 50ns;
            datarate = 100Mbps;
        }
    submodules:

        R1: StandardHost {
            @display("p=124,495");
            gates:
                ethg[1];
        }

        switch_R1: EtherSwitch {
            @display("p=407,266");
            gates:
                ethg[8];  // Da am Ende der Linie, nur zwei Ports
        }

        R2: StandardHost {
            @display("p=255,495");
            gates:
                ethg[1];
        }

        M: StandardHost {
            @display("p=255,100");
            gates:
                ethg[1];
        }

        TV: StandardHost {
            @display("p=407,100");
            gates:
                ethg[1];
        }

        A: StandardHost {
            @display("p=407,495");
            gates:
                ethg[1];
        }

        HU1: StandardHost {
            @display("p=539,495");
            gates:
                ethg[1];
        }

        HU2: StandardHost {
            @display("p=664,495");
            gates:
                ethg[1];
        }

        Ext: StandardHost {
            @display("p=539,100");
            gates:
                ethg[1];
        }

        configurator: IPv4NetworkConfigurator {
            @display("p=72,69");
        }
    connections:

        R1.ethg[0] <--> ethline <--> switch_R1.ethg[0];
        R2.ethg[0] <--> ethline <--> switch_R1.ethg[1];

        M.ethg[0] <--> ethline <--> switch_R1.ethg[2];
        TV.ethg[0] <--> ethline <--> switch_R1.ethg[3];
        A.ethg[0] <--> ethline <--> switch_R1.ethg[4];

        HU1.ethg[0] <--> ethline <--> switch_R1.ethg[5];
        HU2.ethg[0] <--> ethline <--> switch_R1.ethg[6];

        Ext.ethg[0] <--> ethline <--> switch_R1.ethg[7];

}

