package go_back_n;

@license(LGPL);
//
// TODO documentation
//
network Network
{
        parameters:
        int WS;
        double TO;
        double PT;
        double TD;
        double ED;
        double DD;
        double LP;
        
    @display("bgb=560,196");
    submodules:
        coordinator: Coordinator {
            @display("p=259,136");
        }
        node0: Node {
            @display("p=48,47");
        }
        node1: Node {
            @display("p=486,47");
        }
    connections:
        node0.out --> node1.infromnode;
        node1.out --> node0.infromnode;
        coordinator.out1 --> node1.infromcoordinator;
        coordinator.out0 --> node0.infromcoordinator;
}

