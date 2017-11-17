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


network DaisyChainLike
{
    //    @display("bgb=932,543");
    types:
        channel ethline extends DatarateChannel {
            delay = 50ns;
            datarate = 100Mbps;
        }
    submodules:

        R1: StandardHost { 				@display("p=80,260");
            gates:
                ethg[1];
        }

        switch_R1: EtherSwitch {		@display("p=60,400");
            gates:
                ethg[2];  // Da am Ende der Linie, nur zwei Ports
        }
        
        R2: StandardHost {				@display("p=180,260");
            gates:
                ethg[1];
        }

        switch_R2: EtherSwitch {		@display("p=160,400");
            gates:
                ethg[3];
        }

        M: StandardHost {				@display("p=280,260");
            gates:
                ethg[1];
        }

        switch_M: EtherSwitch {			@display("p=260,400");
            gates:
                ethg[3];
        }

        TV: StandardHost {				@display("p=380,260");
            gates:
                ethg[1];
        }

        switch_TV: EtherSwitch {		@display("p=360,400");
            gates:
                ethg[3];
        }

        A: StandardHost {				@display("p=480,260");
            gates:
                ethg[1];
        }

        switch_A: EtherSwitch {			@display("p=460,400");
            gates:
                ethg[3];
        }

        HU1: StandardHost {				@display("p=580,260");
            gates:
                ethg[1];
        }

        switch_HU1: EtherSwitch {		@display("p=560,400");
            gates:
                ethg[3];
        }

        HU2: StandardHost {				@display("p=680,260");
            gates:
                ethg[1];
        }

        switch_HU2: EtherSwitch {		@display("p=660,400");
            gates:
                ethg[3];
        }

        Ext: StandardHost {				@display("p=780,260");
            gates:
                ethg[1];
        }

        switch_Ext: EtherSwitch {		@display("p=760,400");
            gates:
                ethg[2];  // Da am Ende der Linie nur zwei Ports 
        }

        configurator: IPv4NetworkConfigurator {
            @display("p=100,200");
        }
    connections:

        R1.ethg[0] <--> ethline <--> switch_R1.ethg[0];
        R2.ethg[0] <--> ethline <--> switch_R2.ethg[0];

        M.ethg[0] <--> ethline <--> switch_M.ethg[0];
        TV.ethg[0] <--> ethline <--> switch_TV.ethg[0];
        A.ethg[0] <--> ethline <--> switch_A.ethg[0];

        HU1.ethg[0] <--> ethline <--> switch_HU1.ethg[0];
        HU2.ethg[0] <--> ethline <--> switch_HU2.ethg[0];

        Ext.ethg[0] <--> ethline <--> switch_Ext.ethg[0];

        switch_R1.ethg[1] <--> ethline <--> switch_R2.ethg[1];
        switch_R2.ethg[2] <--> ethline <--> switch_M.ethg[1];
        switch_M.ethg[2] <--> ethline <--> switch_TV.ethg[1];
        switch_TV.ethg[2] <--> ethline <--> switch_A.ethg[1];
        switch_A.ethg[2] <--> ethline <--> switch_HU1.ethg[1];
        switch_HU1.ethg[2] <--> ethline <--> switch_HU2.ethg[1];
        switch_HU2.ethg[2] <--> ethline <--> switch_Ext.ethg[1];

}

