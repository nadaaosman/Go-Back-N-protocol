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

#include "coordinator.h"

Define_Module(Coordinator);

void Coordinator::initialize()
{
    // TODO - Generated method body

    //first READ from the file coordinator.txt
    //we are going to read two things which are node to start and time to iniate the communication

    string nodeId, startTime;
    ifstream fin;
    //fin.open("D:/Fall 2023/Computer networks I/GoBackN/src/coordinator.txt");
    fin.open("../src/coordinator.txt");
    if(fin.is_open())
    {
        fin >> nodeId >> startTime; //since the file will contain only 2 strings
        //cout<< nodeId << startTime <<endl;
    }
    else
    {
        cout<<"file not found" <<endl;
    }
    fin.close();
    //create new message
    cMessage* msg = new cMessage;
    msg->setKind(0);
    if(nodeId == (string)"0")
    {
        //use scheduleAt or sendDelayed to send message to the intended not at the intended time
        msg->setName("node0");
        sendDelayed(msg, stoi(startTime), "out0");
        EV << nodeId << startTime;
    }
    else if (nodeId == (string)"1")
    {
        //use scheduleAt or sendDelayed to send message to the intended not at the intended time
        msg->setName("node1");
        sendDelayed(msg, stoi(startTime), "out1");
        EV << nodeId << startTime;
    }
}

void Coordinator::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    // TODO - Generated method body
//       string recived_coord=msg->getName();
//       if(recived_coord[0]=='0'){
//                 send(msg,"out0");
//             }else{
//                 send(msg,"out1");
//             }
}
