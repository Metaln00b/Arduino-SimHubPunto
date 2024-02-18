#include <mcp_can.h>

#include <SPI.h>

//#define CAN0_INT 2 // Set INT to pin 2
MCP_CAN CAN0(10); // Set CS to pin 10

#define BUF_SIZE                64

char simhub_message_buf[BUF_SIZE];

void setup() {
    Serial.begin(115200);
    SPI.begin();

    if(CAN0.begin(MCP_ANY, CAN_50KBPS, MCP_8MHZ) == CAN_OK)
    {
        Serial.println("MCP2515 Initialized Successfully!");
        delay(1000);
    }
    else
    {
        Serial.println("Error Initializing MCP2515...");
        delay(2000);
        setup();
    }
    CAN0.setMode(MCP_NORMAL);
    
    //pinMode(CAN0_INT, INPUT);

    memset(simhub_message_buf, 0x0, BUF_SIZE);
}

void loop() {
    byte data180[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    CAN0.sendMsgBuf(0x180, 0, 6, data180);

    byte data282[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    CAN0.sendMsgBuf(0x282, 0, 8, data282);

    byte data281[8] = {0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x40, 0x00};
    CAN0.sendMsgBuf(0x281, 0, 8, data281);

    byte data2a0[4] = {0x00, 0xAF, 0x00, 0xCA};
    CAN0.sendMsgBuf(0x2a0, 0, 4, data2a0);

    byte data380[8] = {0x08, 0x00, 0x40, 0x00, 0x00, 0x64, 0x00, 0x00};
    CAN0.sendMsgBuf(0x380, 0, 8, data380);

    byte data286[2] = {0x00, 0x00};
    CAN0.sendMsgBuf(0x286, 0, 2, data286);

    byte data382[1] = {0x00};
    CAN0.sendMsgBuf(0x382, 0, 1, data382);

    delay(100);
}

void process_message() {
    unsigned int revs;
    unsigned int speed_kmh;
    unsigned int fuel_percent;
    float water_temperature_degC;
    int turn_left;
    int turn_right;
    int brake;
    float oil_temperature_degC;
    
    sscanf(simhub_message_buf, "%u&%u&%u&%f&%d&%d&%d&%f",
        &revs,
        &speed_kmh,
        &fuel_percent,
        &water_temperature_degC,
        &turn_left,
        &turn_right,
        &brake,
        &oil_temperature_degC
    );
}

void dontloop() {
    if (Serial.available() > 0)
    {
        Serial.readBytesUntil('{', simhub_message_buf, BUF_SIZE);
        int readCount = Serial.readBytesUntil('}', simhub_message_buf, BUF_SIZE);
        simhub_message_buf[min(readCount, BUF_SIZE - 1)] = 0x0;
        process_message();
        memset(simhub_message_buf, 0x0, BUF_SIZE);
    }
}