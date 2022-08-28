#ifndef RTPCLIENT_H
#define RTPCLIENT_H

#include "jrtplib3/rtpsession.h"
#include "jrtplib3/rtpudpv4transmitter.h"
#include "jrtplib3/rtpipv4address.h"
#include "jrtplib3/rtpsessionparams.h"
#include "jrtplib3/rtperrors.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <pthread.h>

using namespace jrtplib;

void* sendMessage(void* rtpclient);

class rtpClient
{
public:
    rtpClient(std::string dest_ip, uint16_t dest_port, uint16_t portbase);
    void send_message(void *message, uint16_t destport, std::string dest_ip);
    int checkerror(int rtperr);
    void* message;
    RTPSession sess;
    uint16_t portbase, destport;
    uint32_t destip;
    std::string ip;
    pthread_t tid;
    RTPUDPv4TransmissionParams transparams;
    RTPSessionParams sessparams;
};

#endif // RTPCLIENT_H
