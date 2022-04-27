// lockbox.c
// by Kyle Bouwman

#include <wiringPi.h>
#include <stdio.h>

#define ledpin_open 0
#define ledpin_record 3
#define buttonpin_lock 1
#define buttonpin_record 4

//led state indicators
int ledstate_open = HIGH;
int ledstate_record = LOW;

//button state indicators
int buttonstate_lock = HIGH;
int lastbuttonstate_lock = HIGH;
int buttonstate_record = HIGH;
int lastbuttonstate_record = HIGH;

//button state change vars
long lastChangeTime_lock;
long lastChangeTime_record;
long capturetime = 50;

//record state
int recordstate = 0;
long recordtime = 5000; //5 seconds
long lastrecordtime;


//reading vars
int reading_lock;
int reading_record;

int main(void) {
    printf("Starting lockbox...\n");
    wiringPiSetup();

    pinMode(ledpin_open, OUTPUT);
    pinMode(ledpin_record, OUTPUT);
    pinMode(buttonpin_lock, INPUT);
    pinMode(buttonpin_record, INPUT);

    pullUpDnControl(buttonpin_lock, PUD_UP);
    pullUpDnControl(buttonpin_record, PUD_UP);

    while(1) {
        reading_lock = digitalRead(buttonpin_lock);
        if(reading_lock != lastbuttonstate_lock) {
            lastChangeTime_lock = millis();
        }
        
        reading_record = digitalRead(buttonpin_record);
        if(reading_record != lastbuttonstate_record) {
            lastChangeTime_record = millis();
        }
        
        //change in lockbutton state
        if(millis() - lastChangeTime_lock > capturetime) {
            //button state has changed
            if(reading_lock != buttonstate_lock) {
                buttonstate_lock = reading_lock;
                if(buttonstate_lock == LOW) {
                    printf("Lock button pressed\n");
                }
                else {
                    printf("Lock button released\n");
                    ledstate_open = LOW;
                }
            }
        }

        //change in recordbutton state
        if(millis() - lastChangeTime_record > capturetime) {
            //button state has changed
            if(reading_record != buttonstate_record) {
                buttonstate_record = reading_record;
                if(buttonstate_record == LOW) {
                    printf("Record button pressed\n");
                }
                else {
                    printf("Record button released\n");
                    recordstate = 1;
                    lastrecordtime = millis();
                    ledstate_record = HIGH;
                    printf("Record LED on\n");
                }
            }
        }

        if(recordstate && millis() - lastrecordtime > recordtime) {
            recordstate = 0;
            ledstate_record = LOW;
            printf("Record LED off\n");
        }

        digitalWrite(ledpin_open, ledstate_open);
        lastbuttonstate_lock = reading_lock;
        digitalWrite(ledpin_record, ledstate_record);
        lastbuttonstate_record = reading_record;
    }

    return 0;
}
