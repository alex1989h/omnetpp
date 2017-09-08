@license(LGPL);

import ned.DelayChannel;

simple Actor
{
    gates:
        input in;
        output out;
}

channel WURFGIRL extends ned.DelayChannel
{
	delay = 0.2s; 
} 
 
channel WURFBOY extends ned.DelayChannel
{
	delay = 0.3s; 
}
 
network Playground
{
    @display("bgb=464,198;bgl=2");
    submodules:
        girl: Actor {
            @display("p=59,54;tt=girl");
        }
        boy: Actor {
            @display("p=336,54");
        }
    connections:
        girl.out --> WURFGIRL --> boy.in;
        boy.out --> WURFBOY --> girl.in;
}
