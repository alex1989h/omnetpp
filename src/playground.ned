@license(LGPL);

import ned.DelayChannel;

simple Actor
{
    gates:
        input in;
        output out;
        
        input inCom;
}

simple Spiderman
{
    gates:
        input in;
        output out;
        output outCom;

        input in2;
        output out2;
        output out2Com;

        input in3;
        output out3;
        output out3Com;

        input in4;
        output out4;
        output out4Com;
}


channel WURFGIRL extends ned.DelayChannel
{
    delay = 0.2s;
}

channel WURFBOY extends ned.DelayChannel
{
    delay = 0.3s;
}

channel SPIDERMAN extends ned.DelayChannel
{
    delay = 0.4s;
}

network Playground
{
    @display("bgb=500,500;bgl=2");
    submodules:
        girl: Actor {
            @display("p=450,50");
        }
        girl2: Actor {
            @display("p=450,450");
        }
        boy: Actor {
            @display("p=50,50");
        }
        boy2: Actor {
            @display("p=50,450");
        }
        spiderman: Spiderman {
            @display("p=250,250");
        }
    connections:
        girl.out --> WURFGIRL --> spiderman.in;
        spiderman.out --> SPIDERMAN --> girl.in;

        boy.out --> WURFBOY --> spiderman.in2;
        spiderman.out2 --> SPIDERMAN --> boy.in;

        girl2.out --> WURFGIRL --> spiderman.in3;
        spiderman.out3 --> SPIDERMAN --> girl2.in;

        boy2.out --> WURFBOY --> spiderman.in4;
        spiderman.out4 --> SPIDERMAN --> boy2.in;
        
        spiderman.outCom --> boy.inCom;
        spiderman.out2Com --> boy2.inCom;
        spiderman.out3Com --> girl.inCom;
        spiderman.out4Com --> girl2.inCom;
}
