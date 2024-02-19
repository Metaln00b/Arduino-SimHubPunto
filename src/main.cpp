#include <stdio.h>
#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10); // Set CS Pin

#define BUF_SIZE                    64

#define ENGINE_SPEED_FACTOR         32

char simhub_message_buf[BUF_SIZE];

void splitBytes(uint16_t num, uint8_t *lowByte, uint8_t *highByte) {
    *lowByte = (uint8_t)(num & 0xFF);
    *highByte = (uint8_t)((num >> 8) & 0xFF);
}

void setup() {
    Serial.begin(115200);
    SPI.begin();

    if (CAN0.begin(MCP_ANY, CAN_50KBPS, MCP_8MHZ) == CAN_OK)
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

    memset(simhub_message_buf, 0x0, BUF_SIZE);
}

void process_message() {
    unsigned int revs;
    unsigned int speed_kmh;
    unsigned int fuel_percent;
    int water_temperature_degC;
    int turn_left;
    int turn_right;
    int brake;
    int oil_temperature_degC;
    
    sscanf(simhub_message_buf, "%u&%u&%u&%d&%d&%d&%d&%d",
        &revs,
        &speed_kmh,
        &fuel_percent,
        &water_temperature_degC,
        &turn_left,
        &turn_right,
        &brake,
        &oil_temperature_degC
    );

    uint8_t light_state = 0x00;

    if (turn_left == 1)
    {
        light_state = light_state | 0b01000000;
    }
    if (turn_right == 1)
    {
        light_state = light_state | 0b00100000;
    }

    uint8_t data180[6] = {0x00, 0x00, light_state, 0x00, 0x00, 0x00};
    uint8_t data282[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    uint8_t calculated_water_temperature_byte = 0;
    uint8_t misc_state = 0x80;

    if (water_temperature_degC < 90.0)
    {
        calculated_water_temperature_byte = water_temperature_degC * 0.8 + 50.0;
    }
    else if (water_temperature_degC >= 90.0 && water_temperature_degC < 91.0)
    {
        calculated_water_temperature_byte = 130;
    }
    else if (water_temperature_degC >= 91.0 && water_temperature_degC < 120.0)
    {
        calculated_water_temperature_byte = water_temperature_degC * 0.8 + 67.0;
    }
    else if (water_temperature_degC >= 120.0)
    {
        misc_state = misc_state | 0b00000001;
    }

    uint8_t calculated_revs = revs / ENGINE_SPEED_FACTOR;
    uint8_t data281[8] = {0x00, 0x00, misc_state, calculated_water_temperature_byte, 0x00, 0x00, calculated_revs, 0x00};

    unsigned char lowByte, highByte;
    splitBytes(speed_kmh * 10, &lowByte, &highByte);
    uint8_t data2a0[4] = {highByte, lowByte, 0x00, 0xCA};

    uint8_t electric_state = 0x08;

    if (brake == 1)
    {
        electric_state = electric_state | 0b01000000;
    }

    uint8_t fuel_state = 0x00;
    uint8_t fuel_percent_byte = (uint8_t)fuel_percent;
    if (fuel_percent_byte <= 15)
    {
        fuel_state = fuel_state | 0b00000010;
    }

    uint8_t data380[8] = {electric_state, 0x00, 0x40, 0x00, fuel_state, fuel_percent_byte, 0x00, 0x00};
    uint8_t data286[2] = {0x00, 0x00};
    uint8_t data382[1] = {0x00};

    CAN0.sendMsgBuf(0x180, 0, 6, data180);
    CAN0.sendMsgBuf(0x282, 0, 8, data282);
    CAN0.sendMsgBuf(0x281, 0, 8, data281);
    CAN0.sendMsgBuf(0x2a0, 0, 4, data2a0);
    CAN0.sendMsgBuf(0x380, 0, 8, data380);
    CAN0.sendMsgBuf(0x286, 0, 2, data286);
    CAN0.sendMsgBuf(0x382, 0, 1, data382);
}

void loop() {
    if (Serial.available() > 0)
    {
        Serial.readBytesUntil('{', simhub_message_buf, BUF_SIZE);
        int readCount = Serial.readBytesUntil('}', simhub_message_buf, BUF_SIZE);
        simhub_message_buf[min(readCount, BUF_SIZE - 1)] = 0x0;
        process_message();
        memset(simhub_message_buf, 0x0, BUF_SIZE);
    }
}