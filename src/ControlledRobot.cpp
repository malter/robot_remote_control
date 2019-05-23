#include "ControlledRobot.hpp"
#include <iostream>

using namespace std;
using namespace interaction;


ControlledRobot::ControlledRobot(TransportSharedPtr commandTransport,TransportSharedPtr telemetryTransport):
    commandTransport(commandTransport),
    telemetryTransport(telemetryTransport)
{

}

void ControlledRobot::update()
{
    while (receiveRequest() != NO_DATA){};
}

ControlMessageType ControlledRobot::receiveRequest()
{
    std::string msg;
    int result = commandTransport->receive(&msg,interaction::Transport::NOBLOCK);
    if (result){
        ControlMessageType reqestType = evaluateRequest(msg);
        return reqestType;
    }
    return NO_DATA;
}

ControlMessageType ControlledRobot::evaluateRequest(const std::string& request)
{

    uint16_t* type = (uint16_t*)request.data();
    ControlMessageType msgtype = (ControlMessageType)*type;
    std::string serializedMessage(request.data()+sizeof(uint16_t),request.size()-sizeof(uint16_t));

    switch (msgtype){
        case CURRENT_POSE:{
            commandTransport->send(serializeCurrentPose());
            return CURRENT_POSE;
        }
        case TARGET_POSE:{
            targetPose.ParseFromString(serializedMessage);
            commandTransport->send(serializeControlMessageType(TARGET_POSE));
            return TARGET_POSE;
        }
        case TWIST_COMMAND:{
            twistCommand.ParseFromString(serializedMessage);
            commandTransport->send(serializeControlMessageType(TWIST_COMMAND));
            return TWIST_COMMAND;
        }
        
        default:{
            commandTransport->send(serializeControlMessageType(NO_DATA));
            return msgtype;
        } 
    }

}



void ControlledRobot::setCurrentPose(const interaction::Pose& pose){
    currentPose = pose;
    sendTelemetry(pose,CURRENT_POSE);
}

void ControlledRobot::addControlMessageType(std::string &buf, const ControlMessageType& type){
    int currsize = buf.size();
    buf.resize(currsize + sizeof(uint16_t));
    uint16_t typeint = type;
    uint16_t* data = (uint16_t*)(buf.data()+currsize);
    *data = typeint;
}

std::string ControlledRobot::serializeControlMessageType(const ControlMessageType& type){
    std::string buf;
    addControlMessageType(buf,type);
    return buf;
}

std::string ControlledRobot::serializeCurrentPose()
{
    std::string buf;
    addControlMessageType(buf,CURRENT_POSE);
    currentPose.AppendToString(&buf);
    return buf;

}