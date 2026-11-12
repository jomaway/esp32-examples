// BME280 - virtual copy of a real device (well, almost)
// Copyright DTViMS 2024

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

union dataUnion
{
    uint32_t f;
    uint8_t fBuff[sizeof(float)];
};

typedef struct
{
    uint32_t ADDRESS_attr;
    uint8_t i;
    bool read;
    uint8_t data[4];
    uint8_t resCount;
    uint8_t resData[25];
    uint32_t temp_attr;
    uint32_t press_attr;
    uint32_t hum_attr;
    // TODO: Put your chip variables here
} chip_state_t;

static bool on_i2c_connect(void *user_data, uint32_t address, bool connect);
static uint8_t on_i2c_read(void *user_data);
static bool on_i2c_write(void *user_data, uint8_t data);
static void on_i2c_disconnect(void *user_data);

void chip_init()
{
    chip_state_t *chip = malloc(sizeof(chip_state_t));
    chip->ADDRESS_attr = attr_init("threshold", 0x76);
    chip->temp_attr = attr_init("temp", 516209 << 4);
    chip->press_attr = attr_init("press", 5572000);
    chip->hum_attr = attr_init("hum", 25036);
    const i2c_config_t i2c_config = {
        .user_data = chip,
        .address = 0x76, // default addr
        .scl = pin_init("SCL", INPUT),
        .sda = pin_init("SDA", INPUT),
        .connect = on_i2c_connect,
        .read = on_i2c_read,
        .write = on_i2c_write,
        .disconnect = on_i2c_disconnect, // Optional
    };
    i2c_init(&i2c_config);
    printf("Hello from custom chip!\n");
}

void resetData(void *user_data)
{
    chip_state_t *chS = user_data;
    chS->i = 0;
    chS->resCount = 0;
    for (uint8_t i = 0; i < 25; i++)
        chS->resData[i] = 0;
    for (uint8_t i = 0; i < 4; i++)
        chS->data[i] = 0;
}

bool on_i2c_connect(void *user_data, uint32_t address, bool connect)
{
    chip_state_t *chS = user_data;
    chS->read = false;
    uint8_t myAddr = attr_read(chS->ADDRESS_attr);
    printf("connect to %x. My addr %x\n", address, myAddr);
    if (myAddr != address)
        return false;
    return true; /* Ack */
}

uint8_t on_i2c_read(void *user_data)
{
    chip_state_t *chS = user_data;
    chS->read = true;
    uint8_t res = 0x0;
    if (chS->i == 2 && chS->data[0] == 0xE0 && chS->data[0] == 0xB6)
    {
        res = 0x0;
    }
    else if (chS->i == 1 && chS->data[0] == 0xD0)
    {
        res = 0x60; // chip ID for BME280
    }
    else if (chS->i == 1 && chS->data[0] == 0x88 && chS->resCount == 0)
    {
        chS->resCount = 25;
        // temperature calibration data of the real device
        chS->resData[24] = 0x62;
        chS->resData[23] = 0x6c;
        chS->resData[22] = 0x80;
        chS->resData[21] = 0x66;
        chS->resData[20] = 50;
        chS->resData[19] = 0;
        // Pressure
        chS->resData[18] = 0x28;
        chS->resData[17] = 0x90;
        chS->resData[16] = 0x80;
        chS->resData[15] = 0xD6;
        chS->resData[14] = 0xD0;
        chS->resData[13] = 0x0B;
        chS->resData[12] = 0x3F;
        chS->resData[11] = 0x18;
        chS->resData[10] = 0xFC;
        chS->resData[9] = 0xFF;
        chS->resData[8] = 0xF9;
        chS->resData[7] = 0xFF;
        chS->resData[6] = 0xAC;
        chS->resData[5] = 0x26;
        chS->resData[4] = 0x0A;
        chS->resData[3] = 0xD8;
        chS->resData[2] = 0xBD;
        chS->resData[1] = 0x10;
        // Humidity
        chS->resData[0] = 0x4B;

        // for(uint8_t i = 0; i < 25; i++) // all calibration values
        //   chS->resData[i] = 1;
    }
    else if (chS->i == 1 && chS->data[0] == 0xE1 && chS->resCount == 0)
    {
        chS->resCount = 8; // (real 7) Gyver library error...
        // Humidity calibration data of the real device
        chS->resData[7] = 0x71;
        chS->resData[6] = 0x01;
        chS->resData[5] = 0x00;
        chS->resData[4] = 0x12;
        chS->resData[3] = 0x0F;
        chS->resData[2] = 0x00;
        chS->resData[1] = 0x1E;
        chS->resData[0] = 0x00;
        // for(uint8_t i = 0; i < 8; i++) // all calibration values second part request
        //   chS->resData[i] = i;
    }
    else if (chS->i == 1 && chS->data[0] == 0xF2)
    {
        res = 0x00; // Controls oversampling of humidity data. Read data
    }
    else if (chS->i == 1 && chS->data[0] == 0xF4)
    {
        res = 0x00; // Controls oversampling of temperature and pressure. Read data
    }
    else if (chS->i == 1 && chS->data[0] == 0xF5)
    {
        res = 0x00; // config. Read data
    }
    else if (chS->i == 1 && chS->data[0] == 0xFA && chS->resCount == 0)
    {
        chS->resCount = 3; // temperature. Read data
        union dataUnion dU;
        dU.f = attr_read(chS->temp_attr);
        for (uint8_t i = 0; i < 3; i++)
            chS->resData[i] = dU.fBuff[i];
        printf("temp debug %x [%x,%x,%x,%x]\n", dU.f, dU.fBuff[0], dU.fBuff[1], dU.fBuff[2], dU.fBuff[3]);
    }
    else if (chS->i == 1 && chS->data[0] == 0xF7 && chS->resCount == 0)
    {
        chS->resCount = 3; // Pressure. Read data
        union dataUnion dU;
        dU.f = attr_read(chS->press_attr); // 5572000 - normal
        for (uint8_t i = 0; i < 3; i++)
            chS->resData[i] = dU.fBuff[i];
        printf("press debug %x [%x,%x,%x,%x]\n", dU.f, dU.fBuff[0], dU.fBuff[1], dU.fBuff[2], dU.fBuff[3]);
    }
    else if (chS->i == 1 && chS->data[0] == 0xFD && chS->resCount == 0)
    {
        chS->resCount = 2; // Humidity. Read data
        union dataUnion dU;
        dU.f = attr_read(chS->hum_attr);
        for (uint8_t i = 0; i < 2; i++)
            chS->resData[i] = dU.fBuff[i];
        printf("Hum debug %x [%x,%x,%x,%x]\n", dU.f, dU.fBuff[0], dU.fBuff[1], dU.fBuff[2], dU.fBuff[3]);
    }

    printf("read debug %d [%x,%x,%x,%x]\n", chS->i, chS->data[0], chS->data[1], chS->data[2], chS->data[3]);

    if (chS->resCount == 0)
    {
        chS->i = 0;
    }
    else
    {
        chS->resCount--;
        res = chS->resData[chS->resCount];
    }
    printf("read %x\n", res);
    return res;
}

bool on_i2c_write(void *user_data, uint8_t data)
{
    chip_state_t *chS = user_data;
    chS->data[chS->i] = data;
    chS->i++;
    printf("write %x\n", data);
    if (chS->i == 2 && chS->data[0] == 0xE0 && chS->data[1] == 0xB6)
    {
        printf("reset command %x\n", data);
        resetData(user_data);
    }
    else if (chS->i == 2 && chS->data[0] == 0xF2)
    {
        // Controls oversampling of humidity data. write data
        resetData(user_data);
    }
    else if (chS->i == 2 && chS->data[0] == 0xF4)
    {
        // Controls oversampling of temperature and pressure. write data
        resetData(user_data);
    }
    else if (chS->i == 2 && chS->data[0] == 0xF5)
    {
        // config. write data
        resetData(user_data);
    }
    return true; // Ack
}

void on_i2c_disconnect(void *user_data)
{
    chip_state_t *chS = user_data;
    if (chS->read)
    {
        resetData(user_data);
    }
    printf("disconnect\n");
    // Do nothing
}