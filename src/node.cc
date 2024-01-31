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

#include "node.h"

Define_Module(Node);
vector<TransmittedMsg_Base*> sendWindow;
vector<string> sendWindowError;
 vector<string> all_messages;

int seqNumber = 0;
int ackNumber = 0;
int senderID;
int startWindow=0;
int endWindow;
 int movingwindow=0;
int bufferedMsgs=0;
bool flag_timout = false; //flag for timeout Event
bool flag_nack = false;
int nextmsg=0;
 int messageindex=0;
int modifiedBit;
int eD=0;
int modified=-1;
string lost="No";
int duplicate=0;
ofstream output("output.txt");


void Node::modificationError() {
    //int modifiedBit = int(uniform(0, data.size() * 8));
    modifiedBit = int(uniform(0, 9));
    int modifiedChar = modifiedBit / data.size();
    // invert the bit
    data[modifiedChar][modifiedBit] = data[modifiedChar][modifiedBit] ^ 1;
    //to modify payload after modification error
    dataToChar();
    string payload(charData.begin(), charData.end());
    transmittedMsg->setPayload(payload.c_str());
}

void Node::delayError() {
    totalTime += getParentModule()->par("ED").doubleValue();
}

void Node::duplicationError(int i) {
    duplicate=1;
    TransmittedMsg_Base* tempMsg = new TransmittedMsg_Base;
    tempMsg = transmittedMsg->dup();
    cMessage *convertedMsg = dynamic_cast<cMessage*>(tempMsg);
    double DD = getParentModule()->par("DD").doubleValue();
    // double pt =getParentModule()->par("PT").doubleValue();
    output << "At time[" << simTime().dbl()+getParentModule()->par("PT").doubleValue()*i << "], Node["<<senderID<<"] , Introducing channel error with code =" << "[" << errorFlags[0] << errorFlags[1] << errorFlags[2] << errorFlags[3] << "]" << endl;
    output << "At time[" << simTime().dbl()+(i+1)*+getParentModule()->par("PT").doubleValue() << "], Node["<<senderID<<"] sent frame with seq_num=["<< transmittedMsg->getSeqNumber() <<"] and payload=["<< transmittedMsg->getPayload()<<"]"<<
            " and trailer=["<< toBinary(transmittedMsg->getParity())<<"]" << "Modified [" << modified << "]" << "Lost [" << lost << "]" << ", Duplicate [" << duplicate << "], Delay [" << eD << "]"<< endl;
    // sendWithError();

       sendDelayed(convertedMsg, totalTime , "out");

    duplicatedMsg = new TransmittedMsg_Base;
    duplicatedMsg->setPayload(transmittedMsg->getPayload());
    duplicatedMsg->setSeqNumber(transmittedMsg->getSeqNumber());
    duplicatedMsg->setKind(transmittedMsg->getKind());
    duplicatedMsg->setParity(transmittedMsg->getParity());
    duplicate=2;
    output << "At time[" << simTime().dbl()+getParentModule()->par("PT").doubleValue()*i << "], Node["<<senderID<<"] , Introducing channel error with code =" << "[" << errorFlags[0] << errorFlags[1] << errorFlags[2] << errorFlags[3] << "]" << endl;
    output << "At time[" << simTime().dbl()+(i+1)*+getParentModule()->par("PT").doubleValue()+DD << "], Node["<<senderID<<"] sent frame with seq_num=["<< transmittedMsg->getSeqNumber() <<"] and payload=["<< transmittedMsg->getPayload()<<"]"<<
            " and trailer=["<< toBinary(transmittedMsg->getParity())<<"]" << "Modified [" << modified << "]" << "Lost [" << lost << "]" << ", Duplicate [" << duplicate << "], Delay [" << eD << "]"<< endl;
    totalTime=totalTime + DD;
    TransmittedMsg_Base* tempMsg2 = new TransmittedMsg_Base;
    tempMsg2 = transmittedMsg->dup();
    cMessage *convertedMsg2 = dynamic_cast<cMessage*>(tempMsg2);
       sendDelayed(convertedMsg2, totalTime , "out");
    // sendWithError();
}

void Node::applyErrors(int i) {
    eD=0;
    modified=-1;
    lost="No";
    duplicate=0;
    double pt =getParentModule()->par("PT").doubleValue();
    double td = getParentModule()->par("TD").doubleValue();
    totalTime = pt*(i+1)+td;
    if (errorFlags[1] == '1') // Loss error will dominate
    {
        lost="Yes";
        output << "At time[" << simTime()+getParentModule()->par("PT").doubleValue()*(i+1) << "], Node["<<senderID<<"] sent frame with seq_num=["<< transmittedMsg->getSeqNumber() <<"] and payload=["<< transmittedMsg->getPayload()<<"]"<<
                " and trailer=["<< toBinary(transmittedMsg->getParity())<<"]" << "Modified [" << modified << "]" << "Lost [" << lost << "]" << ", Duplicate [" << duplicate << "], Delay [" << eD << "]"<< endl;
        return;
    }
    else
    {
        if (errorFlags[0] == '1') // Modification error
        {
            modificationError();
            modified=modifiedBit;
        }

        if (errorFlags[3] == '1') // Delay error
        {
            eD=getParentModule()->par("ED").doubleValue();
            delayError();
        }

        if (errorFlags[2] == '1') // Duplication error
        {
            duplicationError(i);
            return;
        }
    }
    //OUTPUT
    if( !flag_timout && !flag_nack)
    {
       output << "At time[" << simTime().dbl()+getParentModule()->par("PT").doubleValue()*i << "], Node["<<senderID<<"] , Introducing channel error with code =" << "[" << errorFlags[0] << errorFlags[1] << errorFlags[2] << errorFlags[3] << "]" << endl;
        output << "At time[" << simTime().dbl()+getParentModule()->par("PT").doubleValue()*(i+1)<< "], Node["<<senderID<<"] sent frame with seq_num=["<< transmittedMsg->getSeqNumber() <<"] and payload=["<< transmittedMsg->getPayload()<<"]"<<
        " and trailer=["<< toBinary(transmittedMsg->getParity())<<"]" << "Modified [" << modified << "]" << "Lost [" << lost << "]" << ", Duplicate [" << duplicate << "], Delay [" << eD << "]"<< endl;
    }
    else
    {
            totalTime=totalTime+0.001;
            output << "At time[" << (simTime().dbl()+getParentModule()->par("PT").doubleValue()*i +0.001)<< "], Node["<<senderID<<"] , Introducing channel error with code =" << "[" << errorFlags[0] << errorFlags[1] << errorFlags[2] << errorFlags[3] << "]" << endl;
             output << "At time[" << (simTime().dbl()+getParentModule()->par("PT").doubleValue()*(i+1)+0.001)<< "], Node["<<senderID<<"] sent frame with seq_num=["<< transmittedMsg->getSeqNumber() <<"] and payload=["<< transmittedMsg->getPayload()<<"]"<<
             " and trailer=["<< toBinary(transmittedMsg->getParity())<<"]" << "Modified [" << modified << "]" << "Lost [" << lost << "]" << ", Duplicate [" << duplicate << "], Delay [" << eD << "]"<< endl;
        }
    TransmittedMsg_Base* tempMsg = new TransmittedMsg_Base;
    tempMsg = transmittedMsg->dup();
    cMessage *convertedMsg = dynamic_cast<cMessage*>(tempMsg);
        sendDelayed(convertedMsg, totalTime , "out"); //not sure
    }


int Node::circularInc(int frameExpected) {
    int windowSize = getParentModule()->par("WS").intValue();
    if (frameExpected >= windowSize)
        frameExpected -= (windowSize);
    else
        frameExpected++;
    return frameExpected;
}

int Node::messageType(cMessage *msg) {
    return msg->getKind();
}

void Node::dataToChar() {
    charData.clear();
    for (int i = 0; i < data.size(); i++) {
        charData.push_back((char) data[i].to_ulong());
    }
}

void Node::charToData() {
    data.clear();
    for (int i = 0; i < charData.size(); i++) {
        std::bitset<8> character(charData[i]);
        data.push_back(character);
    }
}

void Node::framing() {
    dataToChar();
    vector<char> temp;
    for (char i : charData) {
        if (i == '/' || i == '$')
            temp.push_back('/');
        temp.push_back(i);
    }
    charData = temp;
    // inserting flag at the beginning and the end of the message
    charData.insert(charData.begin(), '$');
    charData.insert(charData.end(), '$');
    charToData();
    string payload(charData.begin(), charData.end());
    transmittedMsg->setPayload(payload.c_str());
}

void Node::deframing(string payload) {
    charData.clear();
    // = TransmittedMsg_Base->getPayload();
    for (int i = 0; i < payload.length(); i++)
        charData.push_back(payload[i]);
    charData.erase(charData.begin() + charData.size() - 1);
    charData.erase(charData.begin());
    vector<char> temp;
    for (int i = 0; i < charData.size();) {
        if (charData[i] == '/') // if ESC char, ignore it and take the next
                {
            temp.push_back(charData[i + 1]);
            i = i + 2;
        } else {
            temp.push_back(charData[i]);
            i++;
        }
    }
    charData = temp;
}
std::bitset<8> Node::checksum(const std::vector<std::bitset<8>>& data) {
        // Initialize the sum to the binary representation of 0
        std::bitset<8> sum("00000000");

        // Iterate through each 8-bit bitset in the vector and perform binary addition
        for (const auto& bits : data) {
            sum = addBinary(sum, bits);
        }
        // Take the complement of the first component
        sum.flip(); // Flip all bits to get the complement
        return sum;
    }
  // Function to perform binary addition of two 8-bit bitsets
    std::bitset<8> Node::addBinary(const std::bitset<8>& a, const std::bitset<8>& b) {
        std::bitset<8> sum;
        std::bitset<9> carry;
        carry[0] = 0;
        for (int i = 0; i < 8; ++i) {
            // XOR operation to get the sum bit
            sum[i] = a[i] ^ b[i] ^ carry[i];
            // Calculate the carry for the next iteration
            carry[i+1] = (a[i] & b[i]) | (carry[i] & (a[i] ^ b[i]));
        }
        if (carry[8] == 1) {
            sum = addBinary(sum,std::bitset<8>("00000001"));
        }

        return sum;
    }

void Node::stringToBitset(string string) {
    data.clear();
    for (int i = 0; i < string.size(); i++) {
        data.push_back(bitset<8>(string[i]));
    }
}

void Node::initialize()
{
    // TODO - Generated method body
    // sendWindow.resize(getParentModule()->par("WS").intValue() + 1);
    endWindow=getParentModule()->par("WS").intValue()-1;

}
void Node::handlecoordinator(cMessage *msg){
    //The intended node reads from the meant file
    if (strcmp(msg->getName(), "node0") == 0) {
        readFile("../src/input0.txt");
        senderID = 0;
    } else if (strcmp(msg->getName(), "node1") == 0) {
        readFile("../src/input1.txt");
        senderID = 1;
    }
      for (int i = startWindow; i <= endWindow; i++) {
                transmittedMsg = new TransmittedMsg_Base;
                transmittedMsg = sendWindow[i];
                flag_timout=false;
                flag_nack=false;
                errorFlags = sendWindowError[i];
                stringToBitset(transmittedMsg->getPayload());
                transmittedMsg->setFrameType(2); // Frame of data
                framing();
                stringToBitset(transmittedMsg->getPayload());
                transmittedMsg->setParity(static_cast<int>(checksum(data).to_ulong()));
                TransmittedMsg_Base* tempMsg = new TransmittedMsg_Base;
                tempMsg = transmittedMsg->dup();
                applyErrors(i);
                nextmsg++;
                sendWindow[i]->setTime(simTime().dbl()+ getParentModule()->par("PT").doubleValue()*(i+1)+ getParentModule()->par("TO").doubleValue());
                // set timer
              scheduleAt(simTime().dbl()+ getParentModule()->par("PT").doubleValue()*(i+1)+ getParentModule()->par("TO").doubleValue(), tempMsg);
      }
}
void Node::handlerecievermsg(cMessage *msg){
    TransmittedMsg_Base* received = dynamic_cast<TransmittedMsg_Base*>(msg);

    if (received->getSeqNumber() == ackNumber) {
//                    check is corrputed or not corrputed
        stringToBitset(received->getPayload());
        if (checksum(data) == received->getParity()) {
            deframing(received->getPayload());
            //! TODO output the received message -> hint(updated in the charData vector)
            received->setFrameType(1); //Ack
            ackNumber = circularInc(ackNumber);
            received->setAckNumber(ackNumber);
                output << "Uploading payload [" << received->getPayload() <<"and seq_num [" <<received->getSeqNumber() << "] to the network layer "<< endl;
            if(!lostAck())
            {
           output << "At time[" << simTime().dbl()+getParentModule()->par("PT").doubleValue()<< "], Node["<<1-senderID<<"] Sending [ACK] with number ["<< ackNumber << "], loss[No] "<< endl;
           cMessage *convertedMsg = dynamic_cast<cMessage*>(received);
           sendDelayed(convertedMsg,getParentModule()->par("PT").doubleValue()+getParentModule()->par("TD").doubleValue(),"out");

            }
             else
             {
                    output << "At time[" << simTime().dbl()+getParentModule()->par("PT").doubleValue() << "], Node["<<1-senderID<<"] Sending [ACK] with number ["<< ackNumber << "], loss[Yes] "<< endl;
             }
        }
        //else
        //send a nack frame to the sender
        else {
//NACK
            received->setFrameType(0);
            received->setAckNumber(ackNumber);
            if(!lostAck())
            {
                //OUTPUT
                cMessage *convertedMsg = dynamic_cast<cMessage*>(received);
                output << "At time[" << simTime().dbl()+getParentModule()->par("PT").doubleValue()<< "], Node["<<1-senderID<<"] Sending [NACK] with number ["<< ackNumber << "], loss[No] "<< endl;
                sendDelayed(convertedMsg,getParentModule()->par("PT").doubleValue()+getParentModule()->par("TD").doubleValue(),"out");
             }
             else
             {
                    output << "At time[" << simTime().dbl()+getParentModule()->par("PT").doubleValue()<< "], Node["<<1-senderID<<"] Sending [NACK] with number ["<< ackNumber << "], loss[Yes] "<< endl;
             }
        }
    }
    //send the ack of the last received frame
    else {
        received->setFrameType(1); //Ack
        received->setAckNumber(ackNumber);
        if(!lostAck())
        {
            cMessage *convertedMsg = dynamic_cast<cMessage*>(received);
      output << "At time[" << simTime().dbl()+getParentModule()->par("PT").doubleValue()<< "], Node["<<1-senderID<<"] Sending [ACK] with number ["<< ackNumber << "], loss[No] "<< endl;
      sendDelayed(convertedMsg,getParentModule()->par("PT").doubleValue()+getParentModule()->par("TD").doubleValue(),"out");
        }
        else
        {
        output << "At time[" << simTime().dbl()<< "], Node["<<1-senderID<<"] Sending [ACK] with number ["<< ackNumber << "], loss[Yes] "<< endl;
        }
    }
}
void Node::handleack(cMessage *msg)
{
    TransmittedMsg_Base* received = dynamic_cast<TransmittedMsg_Base*>(msg);
    EV << "start" << startWindow<< endl;
    EV << "end" << endWindow<<endl;
    EV << "nextmsg" << nextmsg<< endl;
    EV << "in ack" << received->getAckNumber()<< endl;
    int timing=0;
      // cancel the timer
       while (sendWindow[startWindow]->getSeqNumber()!= received->getAckNumber()) {
           sendWindow[startWindow]->setTime(0);
                            startWindow++;
                            if(endWindow <sendWindow.size()-1)
                            endWindow++;

                    }
       EV << "eendafter while" << endWindow<<endl;
      // send the new window
            for (int i = nextmsg; i <= endWindow && i<sendWindow.size(); i++) {
                transmittedMsg = new TransmittedMsg_Base;
//                sendWindow[i]->setTime(1);
                transmittedMsg = sendWindow[i];
                flag_timout=false;
                flag_nack=false;
                errorFlags = sendWindowError[i];
                stringToBitset(transmittedMsg->getPayload());
                transmittedMsg->setFrameType(2); // Frame of data
                framing();
                stringToBitset(transmittedMsg->getPayload());
                transmittedMsg->setParity(static_cast<int>(checksum(data).to_ulong()));
                TransmittedMsg_Base* tempMsg = new TransmittedMsg_Base;
                tempMsg = transmittedMsg->dup();
//                bufferedMsgs++;
                 nextmsg++;
                applyErrors(timing);
                sendWindow[i]->setTime(simTime().dbl()+ getParentModule()->par("PT").doubleValue()*(timing+1)+ getParentModule()->par("TO").doubleValue());
                EV << "aloooooo1111inhandletimout" << (simTime().dbl()+ getParentModule()->par("PT").doubleValue()*(timing+1)+ getParentModule()->par("TO").doubleValue()) << endl;
                EV << "aloooooo22222inhandletimout" << (sendWindow[i]->getTime()) << endl;
                EV << "timing" << timing<< endl;
                // set timer
              scheduleAt(simTime().dbl()+ getParentModule()->par("PT").doubleValue()*(timing+1)+ getParentModule()->par("TO").doubleValue(), tempMsg);
              timing++;
                }
      }
void Node::handletimout(cMessage *msg){
//    TransmittedMsg_Base *received = check_and_cast<TransmittedMsg_Base*>(msg);
    TransmittedMsg_Base* received = dynamic_cast<TransmittedMsg_Base*>(msg);
    int indextimout=startWindow;
    while (sendWindow[indextimout]->getSeqNumber()!= received->getSeqNumber() && indextimout < sendWindow.size()-1) {
                       indextimout++;
                      }
    EV << "mytime: " << sendWindow[indextimout]->getTime() << endl;
    EV << "simtime: " << simTime().dbl() << endl;
    EV << "seq: " << received->getSeqNumber() << endl;
    if(sendWindow[indextimout]->getTime()==simTime().dbl())
    {
        output << "Timeout event at [" << simTime().dbl() << "] at Node[" << senderID << "] for frame with seq_num= [" << received->getSeqNumber() << "]" <<endl;

    int timing=0;
    double timout=0;
//    nextmsg=indextimout;
    flag_timout=true;
    flag_nack=false;
    EV << "messages: " << sendWindow.size()<< endl;
    EV << "start: " << startWindow<< endl;
    EV << "end: " << endWindow<< endl;
    EV << "nextmsg: " << nextmsg<< endl;
    for (int i = indextimout; i < nextmsg &&i<sendWindow.size() ; i++) {
        transmittedMsg = new TransmittedMsg_Base;
        transmittedMsg = sendWindow[i];
        timout=getParentModule()->par("PT").doubleValue()*(timing+1)+ getParentModule()->par("TO").doubleValue()+0.001;
        EV << "timout" << timout << endl;
        if (i==indextimout)
        { errorFlags ="0000";
        }
        else{
            errorFlags = sendWindowError[i];
        }
        stringToBitset(transmittedMsg->getPayload());
        transmittedMsg->setFrameType(2); // Frame of data
        framing();
        stringToBitset(transmittedMsg->getPayload());
        transmittedMsg->setParity(static_cast<int>(checksum(data).to_ulong()));
        TransmittedMsg_Base* tempMsg = new TransmittedMsg_Base;
        tempMsg = transmittedMsg->dup();
//        cMessage *convertedMsg = dynamic_cast<cMessage*>(tempMsg);
//        nextmsg++;
        applyErrors(timing);
        // set timer
        sendWindow[i]->setTime(simTime().dbl()+timout);
        EV << "aloooooo1111inhandletimout" << (simTime().dbl()+timout) << endl;
        EV << "aloooooo22222inhandletimout" << (sendWindow[i]->getTime()) << endl;
        EV << "timing" << timing<< endl;
      scheduleAt(simTime().dbl()+timout, tempMsg);
      timing++;
        }
    flag_timout=false;
    }
}
void Node::handlenack(cMessage *msg){
//    TransmittedMsg_Base *received = check_and_cast<TransmittedMsg_Base*>(msg);
    TransmittedMsg_Base* received = dynamic_cast<TransmittedMsg_Base*>(msg);
      int indexnack=startWindow;
      while (sendWindow[indexnack]->getSeqNumber()!= received->getAckNumber()  && indexnack < sendWindow.size()-1) {
          indexnack++;
                        }
      int timing=0;
      double timout=0;
//      nextmsg=indexnack;
      flag_nack=true;
      flag_timout=false;
      for (int i = indexnack; i < nextmsg && i<sendWindow.size(); i++) {
          transmittedMsg = new TransmittedMsg_Base;
          transmittedMsg = sendWindow[i];
          timout=(getParentModule()->par("PT").doubleValue())*(timing+1)+ getParentModule()->par("TO").doubleValue()+0.001;
          if (i==indexnack)
          { errorFlags ="0000";

          }
          else{
              errorFlags = sendWindowError[i];
          }
          stringToBitset(transmittedMsg->getPayload());
          transmittedMsg->setFrameType(2); // Frame of data
          framing();
          stringToBitset(transmittedMsg->getPayload());
          transmittedMsg->setParity(static_cast<int>(checksum(data).to_ulong()));
          TransmittedMsg_Base* tempMsg = new TransmittedMsg_Base;
          tempMsg = transmittedMsg->dup();
  //        cMessage *convertedMsg = dynamic_cast<cMessage*>(tempMsg);
//          nextmsg++;
          EV << "nada: "  << endl;
          applyErrors(timing);
          EV << "salma: "<< endl;
          // set timer
          sendWindow[i]->setTime(simTime().dbl()+timout);
        scheduleAt(simTime().dbl()+timout, tempMsg);
        timing++;
          }
      flag_nack=false;
}
void Node::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    double timeOut = getParentModule()->par("TO").doubleValue();
    double pt =getParentModule()->par("PT").doubleValue();
    double td = getParentModule()->par("TD").doubleValue();
    EV << "Received a message: " << msg->getName() << endl;
//    TransmittedMsg_Base *received = check_and_cast<TransmittedMsg_Base*>(msg);
     TransmittedMsg_Base* received = dynamic_cast<TransmittedMsg_Base*>(msg);
    if(!(msg->isSelfMessage()))
    {
        if (msg->getKind() == 0) {
            EV << "coordinator: " << msg->getName() << endl;
            handlecoordinator(msg);
        }
        else {
            if (received->getFrameType() == 2) {
                EV << "datarecieved: " << msg->getName() << endl;
               handlerecievermsg(msg);
            }
            else if (received->getFrameType() == 1) {
                EV << "ack: " << msg->getName() << endl;
               handleack(msg);
                }
            else {
                EV << "notack: " << msg->getName() << endl;
                      // handle nack
                handlenack(msg);
            }
            }
        }

    else
        {
        EV << "timout"<< endl;
        handletimout(msg);
        }

        }

void Node::readFile(string fileName) {

    ifstream fin;
    string line, errorBits, message;
    fin.open(fileName); // open the file

    if (fin.is_open()) // just a causal safety check, Not necessary
    {
        while (fin) {
            // read each line of the file.
            getline(fin, line);

            // This part is to divide the line into 2 parts, one for error bits and one the rest of the message.
            if (line.size()) //without this condition it throws out of range error, apparently it read an empty line from the input file
            {
                errorBits = line.substr(0, 4);
                message = line.substr(5, line.size() - 5);

                /*cout << line << endl
                 << errorBits << endl
                 << message << endl
                 << endl;
                 */
                TransmittedMsg_Base *tempMsg = new TransmittedMsg_Base;
                tempMsg->setPayload(message.c_str());
                tempMsg->setSeqNumber(seqNumber);
                tempMsg->setKind(1); //Transmitted message not configuration message
                seqNumber = circularInc(seqNumber);
                // push errorBits and message in their vectors
                sendWindow.push_back(tempMsg);
                sendWindowError.push_back(errorBits);
            }
        }
    }

    fin.close(); // close the file
}

string Node::toBinary(int n)
{
    string r;
    if(n == 0){r = "0000"; return r;}
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}

bool Node::lostAck()
{
    double noLossProbability=  1-(getParentModule()->par("LP").doubleValue());

    double weight=((double)rand()) / RAND_MAX ;
    if (weight <= noLossProbability)
        return false;
    else
        return true;
}









