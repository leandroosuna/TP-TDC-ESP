#include "main.h"


SoftwareSerial mySerial(4,5);
byte data[10];
byte dataOut[10];
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    ledBlink();
    Serial.begin(9600);
    mySerial.begin(9600);
    debugln("ESP online");
    setupWiFi();
    dataOut[0] =255;
    dataOut[2] =255;
    
}

bool handleRec()
{
    if (data[0] != 255 || data[9] != 255)
    {
        return false;
    }

    int mm = (data[1] << 24) |
        (data[2] << 16) |
        (data[3] << 8) |
        data[4];


    int obj = (data[5] << 24) |
        (data[6] << 16) |
        (data[7] << 8) |
        data[8];

    newPoints(mm, obj);

    return true;
}
bool lastOK = true;

void send(int k, int obj)
{
    dataOut[0] = 255;
    dataOut[1] = (k >> 24) & 0xFF;
    dataOut[2] = (k >> 16) & 0xFF;
    dataOut[3] = (k >> 8) & 0xFF;
    dataOut[4] = k & 0xFF;

    dataOut[5] = (obj >> 24) & 0xFF;
    dataOut[6] = (obj >> 16) & 0xFF;
    dataOut[7] = (obj >> 8) & 0xFF;
    dataOut[8] = obj & 0xFF;
    
    dataOut[9] = 255;
    
    mySerial.write(dataOut,10);
}   

void loop()
{

    MDNS.update();
    digitalWrite(LED_BUILTIN, lastOK);
    delay(1);

    if(mySerial.available() > 10)
    {
        mySerial.readBytes(data, 10);
        
        lastOK = handleRec();
        if(!lastOK)
        {
            Serial.println("rec fail");
        }
        digitalWrite(LED_BUILTIN, LOW);
    }

    if(anyChange())
    {
        send(getK(), getObj());
    }
    
}

void ledBlink()
{
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
}

