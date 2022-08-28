#include "rtpclient.h"

rtpClient::rtpClient(std::string dest_ip, uint16_t dest_port, uint16_t portbase)
{
    int status;
    this->portbase = portbase;
    this->destip = inet_addr(dest_ip.c_str());
    this->destip = ntohl(this->destip);
    this->destport = dest_port;

    RTPUDPv4TransmissionParams transparams;
    RTPSessionParams sessparams;

    sessparams.SetOwnTimestampUnit(1.0 / 10.0);

    sessparams.SetAcceptOwnPackets(true);
    transparams.SetPortbase(portbase);
    status = sess.Create(sessparams, &transparams);

    RTPIPv4Address addr(destip, destport);
    sess.AddDestination(addr);

    checkerror(status);
}

int rtpClient::checkerror(int rtperr)
{
    if(rtperr < 0)
    {
        std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
        return -1;
    }
    return 0;
}

void* sendMessage(void* rtpclient)
{
    int status;
    rtpClient *client = (rtpClient*)rtpclient;
    // RTPIPv4Address addr(client->destip, client->destport);

    // status = client->sess.AddDestination(addr);
    // status = client->checkerror(status);

    std::cout << "\nSending packet" << std::endl;
    status = client->sess.SendPacket(client->message, 10, 0, false, 10);
    status = client->checkerror(status);
    return nullptr;
}

void rtpClient::send_message(void *message, uint16_t dest_port, std::string dest_ip)
{
    this->message = message;
    this->destip = inet_addr(dest_ip.c_str());
    this->destip = ntohl(this->destip);
    this->destport = dest_port;
    pthread_create(&this->tid, nullptr, sendMessage, this);
}
