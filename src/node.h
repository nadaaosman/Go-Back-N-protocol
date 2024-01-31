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

#ifndef __GO_BACK_N_NODE_H_
#define __GO_BACK_N_NODE_H_

#include <omnetpp.h>
#include <vector>
#include <queue>
#include <bitset>
#include "TransmittedMsg_m.h"
#include <iostream>
#include <string>
#include <fstream>


using namespace std;

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Node : public cSimpleModule
{

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);


  public:
     //utility functions for errors
     void modificationError();
     void lossError();
     void duplicationError(int i);
     void delayError();
     void sendWithError(double totalTime, TransmittedMsg_Base* msg);
     void applyErrors(int i);

     //Reading from input text files
     void readFile(string fileName);

     //utility function for debugging
     void printMsg(TransmittedMsg_Base* msg);

     //reset private data members each message
     void initializeParameters();

     //utility functions for Go back N
     void stringToBitset(string string);
     void dataToChar();
     void charToData();
     void framing();
     void deframing(string payload);
     void sendAckWithLp();
     void handlecoordinator(cMessage *msg);
     void handlenack(cMessage *msg);
     void handletimout(cMessage *msg);
     void handlerecievermsg(cMessage *msg);
     void handleprocessing(cMessage *msg);
     void handleack(cMessage *msg);
//     int parityByteCalc(vector<bitset<8> > dataToCheck);
     int messageType(cMessage *msg);
     int circularInc(int frameExpected);
     std::bitset<8> checksum(const std::vector<std::bitset<8>>& data);
     std::bitset<8> addBinary(const std::bitset<8>& a, const std::bitset<8>& b);
     int parityByteCalc(vector<bitset<8> > dataToCheck);
     bool isBetween(int number);
     bool lostAck();

     string toBinary(int n);

   private:
     vector<bitset<8> > data; //for parity byte calculation
     vector<char> charData; //for framing and deframing

     class TransmittedMsg_Base* transmittedMsg;
     string errorFlags;
     class  TransmittedMsg_Base* duplicatedMsg;
     double totalTime;

};

#endif
