//Ultrasonic Sensor Parameters
#define OBS_COUNT 200           //Total Observations for averaging
#define THRESHOLD_DISTANCE 20   //Distance of Keyboard Drawer
#define MAX_DIST 100

//Ultrasonic Sensor
#define TRIG_PIN 11
#define ECHO_PIN 12

//Lighting
#define LED_PIN_KEYBOARD 8
#define LED_PIN_STORAGE 2

//IR Proximity Sensor
#define IR_SENSOR_PIN 3

//Storage Contact Switch
#define STORAGE_SENSOR_PIN 4

#include <NewPing.h>

class sensorQue {
	int que[OBS_COUNT];
public:
	void initialize();
	void push(int n);
	float avg();
};
void sensorQue::initialize() {
	for (int i = 0; i < OBS_COUNT; ++i) {
		que[i] = 70;
	}
}
void sensorQue::push(int n) {
	for (int i = OBS_COUNT - 1; i > 0; --i) {
		que[i] = que[i - 1];
	}
	que[0] = n;
}
float sensorQue::avg() {
	float average = 0;
	for (int i = 0; i < OBS_COUNT; ++i) {
		average += que[i];
	}
	average /= OBS_COUNT;
	return average;
}

NewPing ultSndSensor(TRIG_PIN, ECHO_PIN, MAX_DIST);
sensorQue sensorValues;

void setLed() {
	int irValue = digitalRead(IR_SENSOR_PIN);
	if (sensorValues.avg() > THRESHOLD_DISTANCE && irValue == 0) {
		digitalWrite(LED_PIN_KEYBOARD, LOW);
		if (sensorValues.avg() > THRESHOLD_DISTANCE) {
			digitalWrite(LED_BUILTIN, LOW);
		}
	}
	else {
		digitalWrite(LED_PIN_KEYBOARD, HIGH);
		if (sensorValues.avg() < THRESHOLD_DISTANCE) {
			digitalWrite(LED_BUILTIN, HIGH);
		}
	}
	int storageValue = digitalRead(STORAGE_SENSOR_PIN);
	if (storageValue == 0) {
		digitalWrite(LED_PIN_STORAGE, HIGH);
	}
	else {
		digitalWrite(LED_PIN_STORAGE, LOW);
	}
}


void setup() {
	sensorValues.initialize();
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LED_PIN_KEYBOARD, OUTPUT);
	pinMode(LED_PIN_STORAGE, OUTPUT);
	pinMode(STORAGE_SENSOR_PIN, INPUT);
	pinMode(IR_SENSOR_PIN, INPUT);

}

void loop() {
	int obs = ultSndSensor.ping_cm();
	while (obs == 0) {
		obs = ultSndSensor.ping_cm();
	}
	sensorValues.push(obs);
	setLed();
}
