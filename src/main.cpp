#include <stdio.h>
#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(9); // Punto
MCP_CAN CAN1(10); // F01

#define BUF_SIZE                    64

// Button-Pins
#define AKBTN_PIN                   3
#define BKBTN_PIN                   4
#define CKBTN_PIN                   5
#define DKBTN_PIN                   6
#define FKBTN_PIN                   7

#define ENGINE_SPEED_FACTOR         32

char simhub_message_buf[BUF_SIZE];

static uint8_t count = 0x00;
static uint8_t steering_count = 0xA0;
uint8_t steering_bit = 0b00100000;
uint8_t rpm_bit = 0b00000001;

void splitBytes(uint16_t num, uint8_t *lowByte, uint8_t *highByte) {
    *lowByte = (uint8_t)(num & 0xFF);
    *highByte = (uint8_t)((num >> 8) & 0xFF);
}

void sendConstantMessages() {
    // Ignition
    uint8_t ignition = 0x88;
    /// uint8_t illumination_dark = 0b00001000;
    uint8_t illumination_light = 0b00001010;

    ignition = ignition | illumination_light;
    uint8_t data12f[8] = { 0xFB, count, ignition, 0x1C, 0xF1, 0x05, 0x30, 0x86 };
    // TPMS
    uint8_t datab68[8] = { 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00 };

    CAN1.sendMsgBuf(0x12F, 0, 8, data12f);
    CAN1.sendMsgBuf(0xB68, 0, 8, datab68);

    // Buttons
    uint8_t data1d6[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    if (digitalRead(AKBTN_PIN) == LOW) {
        uint8_t newData[8] = { 0xC8, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        memcpy(data1d6, newData, sizeof(data1d6));
    }


    if (digitalRead(BKBTN_PIN) == LOW) {
        uint8_t newData[8] = { 0xC4, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        memcpy(data1d6, newData, sizeof(data1d6));
    }

    CAN1.sendMsgBuf(0x1D6, 0, 8, data1d6);

    // Drive-Mode
    // 1= Traction, 2= Comfort, 4= Sport, 5= Sport+, 6= DSC off,
    uint8_t drive_mode = 5;
    uint8_t data3a7[8] = { 0x00, count, 0x00, 0x00, drive_mode, 0x00, 0x00, 0x00 };
    CAN1.sendMsgBuf(0x3a7, 0, 8, data3a7);

    // Cruise & Lane
    uint8_t data327[8] = { 0xA2, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    CAN1.sendMsgBuf(0x327, 0, 8, data327);

    // Seatbelt
    uint8_t data581[8] = { 0x40, 0x4D, 0, 0x28, 0xFF, 0xFF, 0xFF, 0xFF };
    CAN1.sendMsgBuf(0x581, 0, 8, data581);

    // Language & Units
    // uint8_t language = 0;
    // uint8_t temp_unit = 0;
    // uint8_t unit = 0;
    // uint8_t data291[8] = { language, temp_unit, unit, 0x00, 0x00, 0x00, 0x00, 0x00 };
    // CAN1.sendMsgBuf(0x291, 0, 8, data291);

    // Airbag
    uint8_t data0d7[8] = { count, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    CAN1.sendMsgBuf(0x0d7, 0, 8, data0d7);

    // Brake
    uint8_t data36f[3] = { count, count, 0x00 };
    CAN1.sendMsgBuf(0x36f, 0, 3, data36f);
    
    // Steering Lock
    uint8_t data2a7[8] = { steering_bit, steering_count, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    CAN1.sendMsgBuf(0x2a7, 0, 8, data2a7);
    steering_bit ^= 0b00000001;
    
    // Adaptive Cruise
    uint8_t adaptive_cruise = 0xFF;
    uint8_t data33b[8] = { 0x00, 0x00, adaptive_cruise, 0x00, 0x00, 0x00, 0x00, 0x00 };
    CAN1.sendMsgBuf(0x33b, 0, 8, data33b);

    // ABS 1
    uint8_t data36e[8] = { 0x00, count, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    CAN1.sendMsgBuf(0x36e, 0, 8, data36e);

    // ABS 2
    uint8_t datab6e[8] = { count, count, count, count, count, count, count, 0x00 };
    CAN1.sendMsgBuf(0xb6e, 0, 8, datab6e);
}

void setup() {
    Serial.begin(115200);
    SPI.begin();

    if (CAN0.begin(MCP_ANY, CAN_50KBPS, MCP_8MHZ) == CAN_OK &&
        CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
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
    CAN1.setMode(MCP_NORMAL);

    pinMode(AKBTN_PIN, INPUT_PULLUP);
    pinMode(BKBTN_PIN, INPUT_PULLUP);
    pinMode(CKBTN_PIN, INPUT_PULLUP);
    pinMode(DKBTN_PIN, INPUT_PULLUP);
    pinMode(FKBTN_PIN, INPUT_PULLUP);

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

    uint8_t turn_normal = 0b00000001;
    // uint8_t turn_emergency = 0b00000011;
    // uint8_t turn_fast = 0b00000010;
    uint8_t turn_state = 0b10000000;

    if (turn_left == 1)
    {
        light_state = light_state | 0b01000000;
        turn_state = turn_normal | 0b00010000;
    }
    if (turn_right == 1)
    {
        light_state = light_state | 0b00100000;
        turn_state = turn_normal | 0b00100000;
    }

    uint8_t data180[6] = {0x00, 0x00, light_state, 0x00, 0x00, 0x00};
    uint8_t data282[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    // Turn Signal
    uint8_t data1f6[8] = { turn_state, 0xF2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    // Lights
    uint8_t data21a[8] = { 0x04, 0x12, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00 };


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

    // RPM
    if (revs > 40950)
    {
        revs = 40950;
    }
    uint8_t revs_data[2] = { 0x00, 0x00 };

    uint16_t rpm_bits = revs / 10;
    
    // Toggle Bit
    rpm_bit ^= 0b00000001;

    // Set Bit 0 (Toggle) and Bits 4-7
    revs_data[0] = (rpm_bit & 0x01) | ((rpm_bits & 0x0F) << 4);

    // Byte 2 (Bits 0-7)
    revs_data[1] = (rpm_bits >> 4) & 0xFF;

    uint8_t data0f3[8] = { 0x76, revs_data[0], revs_data[1], 0x02, 0x01, 0xC4, 0x00, 0x00 };

    unsigned char lowByte, highByte;
    splitBytes(speed_kmh * 10, &lowByte, &highByte);
    uint8_t data2a0[4] = {highByte, lowByte, 0x00, 0xCA};

    // Speed
    if (speed_kmh > 255)
    {
        speed_kmh = 255;
    }
    uint8_t speed_data[2] = { 0x02, 0x00 };

    // Byte 2 (Bits 6-7 and optional Bits 4-5 for decimal)
    speed_data[0] |= (speed_kmh & 0x03) << 6;
    // speed_data[0] |= ((speed_kmh & 0x0C) >> 2) << 4;

    // Byte 3 (Bits 0-7)
    // speed_data[1] |= (speed_kmh >> 4) & 0xFF;
    speed_data[1] |= (speed_kmh >> 2) & 0x3F;

    uint8_t data1a1[8] = { count, count, speed_data[0], speed_data[1], 0x01, 0x00, 0x00, 0x00 };


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

    // Fuel
    if (fuel_percent > 100.0f)
    {
        fuel_percent = 100.0f;   
    }
    if (fuel_percent < 0.0f)
    {
        fuel_percent = 0.0f;
    }

    // Calculate percentage
    float left_tank_liters = 42.5f * (fuel_percent / 100.0f);
    float right_tank_liters = 30.5f * (fuel_percent / 100.0f);

    // Convert Liters to Ohms (Ohm)
    uint16_t left_tank_ohms = (uint16_t)(25 + ((42.5f - left_tank_liters) / 42.5f) * (950 - 25));
    uint16_t right_tank_ohms = (uint16_t)(25 + ((30.5f - right_tank_liters) / 30.5f) * (961 - 25));

    uint8_t left_tank_byte0 = left_tank_ohms & 0xFF;
    uint8_t left_tank_byte1 = (left_tank_ohms >> 8) & 0xFF;
    uint8_t right_tank_byte0 = right_tank_ohms & 0xFF;
    uint8_t right_tank_byte1 = (right_tank_ohms >> 8) & 0xFF;

    uint8_t data349[8] = { left_tank_byte0, left_tank_byte1, right_tank_byte0, right_tank_byte1, 0x00, 0x00, 0x00, 0x00 };

    // Temperature
    if (water_temperature_degC < -48.0f)
    {
        water_temperature_degC = -48.0f;
    }
    if (water_temperature_degC > 205.0f)
    {
        water_temperature_degC = 205.0f;
    }
    if (oil_temperature_degC < -48.0f)
    {
        oil_temperature_degC = -48.0f;
    }
    if (oil_temperature_degC > 206.0f)
    {
        oil_temperature_degC = 206.0f;
    }

    uint8_t water_temperature_byte = (uint8_t)((water_temperature_degC + 48.0f) / 253.0f * 255.0f);
    uint8_t oil_temperature_byte = (uint8_t)((oil_temperature_degC + 48.0f) / 254.0f * 255.0f);

    uint8_t data3f9[8] = { 0x02, count, count, 0x00, water_temperature_byte, oil_temperature_byte, count, count };

    CAN1.sendMsgBuf(0x1f6, 0, 8, data1f6);
    CAN1.sendMsgBuf(0x21a, 0, 8, data21a);
    CAN1.sendMsgBuf(0x0f3, 0, 8, data0f3);
    CAN1.sendMsgBuf(0x1a1, 0, 8, data1a1);
    CAN1.sendMsgBuf(0x349, 0, 8, data349);
    CAN1.sendMsgBuf(0x3f9, 0, 8, data3f9);

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

    sendConstantMessages();
    
    if (count >= 0xFF)
    {
        count = 0x00;
    }
    else 
    {
        count++;
    }
    
    
    if (steering_count >= 0xFF)
    {
        steering_count = 0xA0;
    }
    else
    {
        steering_count++;
    }
}