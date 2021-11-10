#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
BluetoothSerial SerialBT;
int received;
const int flexPin1 = A6; // Pin connected to voltage divider output
const int flexPin2 = A7;//4, 5 16 17 18 19 21 22 23 27 25 32//D5 works fine
const int flexPin3 = A4;
const int flexPin4 = A5;
const int flexPin5 = A18;

// Change these constants according to your project's design
const float VCC = 3.3; // voltage at Ardunio 5V line
const float R_DIV = 2000.0; // resistor used to create a voltage divider
const float flatResistance = 3070.0; // resistance when flat
const float bendResistance = 5000.0; // resistance at 90 deg

void setup() {


Serial.begin(115200);
while (!Serial)
delay(10); // will pause Zero, Leonardo, etc until serial console opens
Serial.println("Adafruit MPU6050 test!");
// Try to initialize!
if (!mpu.begin()) {
Serial.println("Failed to find MPU6050 chip");
while (1) {
delay(10);
}
}
Serial.println("MPU6050 Found!");
mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
Serial.print("Accelerometer range set to: ");
switch (mpu.getAccelerometerRange()) {
case MPU6050_RANGE_2_G:
Serial.println("+-2G");
break;
case MPU6050_RANGE_4_G:
Serial.println("+-4G");
break;
case MPU6050_RANGE_8_G:
Serial.println("+-8G");
break;
case MPU6050_RANGE_16_G:
Serial.println("+-16G");
break;
}
mpu.setGyroRange(MPU6050_RANGE_500_DEG);
Serial.print("Gyro range set to: ");
switch (mpu.getGyroRange()) {
case MPU6050_RANGE_250_DEG:
Serial.println("+- 250 deg/s");
break;
case MPU6050_RANGE_500_DEG:
Serial.println("+- 500 deg/s");
break;
case MPU6050_RANGE_1000_DEG:
Serial.println("+- 1000 deg/s");
break;
case MPU6050_RANGE_2000_DEG:
Serial.println("+- 2000 deg/s");
break;
}
mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
Serial.print("Filter bandwidth set to: ");
switch (mpu.getFilterBandwidth()) {
case MPU6050_BAND_260_HZ:
Serial.println("260 Hz");
break;
case MPU6050_BAND_184_HZ:
Serial.println("184 Hz");
break;
case MPU6050_BAND_94_HZ:
Serial.println("94 Hz");
break;
case MPU6050_BAND_44_HZ:
Serial.println("44 Hz");
break;
case MPU6050_BAND_21_HZ:
Serial.println("21 Hz");
break;
case MPU6050_BAND_10_HZ:
Serial.println("10 Hz");
break;
case MPU6050_BAND_5_HZ:
Serial.println("5 Hz");
break;
}
Serial.println("");
delay(100);
SerialBT.begin("ESP32");
Serial.println("Start pairing!");
pinMode(flexPin1, INPUT);
pinMode(flexPin2, INPUT);
pinMode(flexPin3, INPUT);
pinMode(flexPin4, INPUT);
pinMode(flexPin5, INPUT);

}

void loop() {

 /* Get new sensor events with the readings */
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);
/* Print out the values */
Serial.print("Acceleration X: ");
Serial.print(a.acceleration.x);
Serial.print(", Y: ");
Serial.print(a.acceleration.y);
Serial.print(", Z: ");
Serial.print(a.acceleration.z);
Serial.println(" m/s^2");
Serial.print("Rotation X: ");
Serial.print(g.gyro.x);
Serial.print(", Y: ");
Serial.print(g.gyro.y);
Serial.print(", Z: ");
Serial.print(g.gyro.z);
Serial.println(" rad/s");
Serial.print("Temperature: ");
Serial.print(temp.temperature);
Serial.println(" degC");
Serial.println("");
delay(500);

// Read the ADC, and calculate voltage and resistance from it
int ADCflex1 = analogRead(flexPin1);
float Vflex1 = ADCflex1 * VCC / 1023.0;
float Rflex1= -R_DIV * (VCC / Vflex1 - 1.0);
//Serial.println("Resistance: " + String(Rflex) + " ohms");
//delay(1000);
// Use the calculated resistance to estimate the sensor's bend angle:
float angle1 = map(Rflex1, flatResistance, bendResistance, 0, 90.0);
//Serial.println("Bend1: " + String(angle1) + " degrees");
/* if(int(angle1)<=-189 && int(angle1)>=-194)
{
// SerialBT.println("HELP");
}*/
Serial.println();
if (Serial.available()) {
SerialBT.write(ADCflex1);
if (SerialBT.available()) {
SerialBT.println(angle1);
}
}

 // delay(2000);

 int ADCflex2 = analogRead(flexPin2);
float Vflex2 = ADCflex2 * VCC / 1023.0;
float Rflex2 = -R_DIV * (VCC / Vflex2 - 1.0);
//Serial.println("Resistance: " + String(Rflex2) + " ohms");

 // Use the calculated resistance to estimate the sensor's bend angle:(-200 to -300)
float angle2 = map(Rflex2, flatResistance, bendResistance, 0, 90.0);
//Serial.println("Bend2: " + String(angle2) + " degrees");
//Serial.println();
/*if(int(angle2)<=-200 && int(angle2)>=-300)
{
SerialBT.println("FOOD");
}*/
Serial.println();
if (Serial.available()) {
SerialBT.write(ADCflex2);
if (SerialBT.available()) {
SerialBT.println(angle2);
}
}
//
//
int ADCflex3 = analogRead(flexPin3);
float Vflex3 = ADCflex3 * VCC / 1023.0;
float Rflex3 = -R_DIV * (VCC / Vflex3 - 1.0);
//Serial.println("Resistance: " + String(Rflex3) + " ohms");
//
// // Use the calculated resistance to estimate the sensor's bend angle:(-100 to -150)
float angle3 = map(Rflex3, flatResistance, bendResistance, 0, 90.0);
//Serial.println("Bend3: " + String(angle3) + " degrees");
//Serial.println();
/* if(int(angle3)<=-100 && int(angle3)>=-150)
{
}*/
Serial.println();
if (Serial.available()) {
SerialBT.write(ADCflex3);
if (SerialBT.available()) {
SerialBT.println(angle3);
}
}
//
//
// delay(500);
int ADCflex4 = analogRead(flexPin4);
float Vflex4 = ADCflex4 * VCC / 1023.0;
float Rflex4 = -R_DIV * (VCC / Vflex4 - 1.0);
//Serial.println("Resistance: " + String(Rflex4) + " ohms");
//
// // Use the calculated resistance to estimate the sensor's bend angle:
float angle4 = map(Rflex4, flatResistance, bendResistance, 0, 90.0);
//Serial.println("Bend4: " + String(angle4) + " degrees");
Serial.println();
//
// delay(500);
int ADCflex5 = analogRead(flexPin5);
float Vflex5 = ADCflex5 * VCC / 1023.0;
float Rflex5 = -R_DIV * (VCC / Vflex5 - 1.0);
// Serial.println("Resistance: " + String(Rflex5) + " ohms");
//
// // Use the calculated resistance to estimate the sensor's bend angle:(-90 to -120)
float angle5 = map(Rflex5, flatResistance, bendResistance, 0, 90.0);
// Serial.println("Bend5: " + String(angle5) + " degrees");
//Serial.println();
/* if(int(angle5)<=-90 && int(angle5)>=-120)
{
SerialBT.println("WATER");
}*/
Serial.println();
if (Serial.available()) {
SerialBT.write(ADCflex5);
if (SerialBT.available()) {
SerialBT.println(angle5);
}
}
if(int(a.acceleration.x)> 7)

{

if(int (angle1)<-110 && int(angle1)>-160 && int (angle2)<-190 && int (angle3)<-180 && int (angle4)<-150 && int (angle5)<-150 )
{
SerialBT.println("A");
delay(500);
}
else if(int (angle1)<-170 && int (angle2)<-125 && int(angle2)>-210 && int (angle3)<-120 && int (angle3)>-160  && int (angle4)<-115 && int (angle4)>-140 && int (angle5)<-115 && int (angle5)>-145)
{
SerialBT.println("B");
delay(500);
}
else if(int (angle1)<-120 && int (angle2)<-150 && int (angle3)<-150 && int (angle4)<-135 && int (angle5)<-120)
{
SerialBT.println("C");
delay(500);
}
else if(int (angle1)<-140 && int (angle2)<-120 && int (angle3)<-160 && int (angle4)<-140 && int (angle5)<-140 )
{
SerialBT.println("D");
delay(500);
}
/*else if(int (angle1)<-160  && int (angle2)<-200 && int (angle3)<-170 && int (angle4)<-150  && int (angle5)<-170) 
{
SerialBT.println("E");
delay(500);
}

*/
else if(int (angle1)<-164 && int (angle2)<-220 && int (angle3)<-129 && int (angle4)<-120 && int (angle5)<-114)
{
SerialBT.println("F");
delay(500);
}
/*else if(int (angle1)<-119 && int (angle2)<-130 && int (angle3)<-200 && int (angle4)<-170 && int (angle5)<-170)
{
SerialBT.println("G");
delay(500);
}
*/
else if(int (angle1)<-145 && int (angle2)<-120 && int (angle3)<-124 && int (angle4)<-170 && int (angle5)<-143)
{
SerialBT.println("V");
delay(500);
}
else if(int (angle1)<-160 && int (angle2)<-197 && int (angle3)<-185 && int (angle4)<-162 && int (angle5)<-110)
{
SerialBT.println("I");
delay(500);
}

else if(int (angle1)<-130 && int (angle2)<-121 && int (angle3)<-124 && int (angle4)<-165 && int (angle5)<-145)
{
SerialBT.println("K");
delay(500);
}
else if(int (angle1)<-115 && int (angle2)<-127 && int (angle3)<-124 && int (angle4)<-170 && int (angle5)<-130)
{
SerialBT.println("L");
delay(500);
}
else if(int (angle1)<-152 && int (angle2)<-127 && int (angle3)<-125 && int (angle4)<-177 && int (angle5)<-119)
{
SerialBT.println("M");//ZH chinese
delay(500);
}


else if(int (angle1)<-174 && int (angle2)<-123 && int (angle3)<-205 && int (angle4)<-168 && int (angle5)<-120)
{
SerialBT.println("T");//chinese
delay(500);
}

else if(int (angle1)<-138 && int (angle2)<-120 && int (angle3)<-120 && int (angle4)<-125 && int (angle5)<-140)
{
SerialBT.println("W");
delay(500);
}
else if(int (angle1)<-171 && int (angle2)<-148 && int (angle3)<-202 && int (angle4)<-175 && int (angle5)<-174)
{
SerialBT.println("X");
delay(500);
}


else if(int (angle1)<-108 && int (angle2)<-189 && int (angle3)<-200 && int (angle4)<-160 && int (angle5)<-110)
{
SerialBT.println("Y");//
delay(500);
}
if(int (angle1)<-160  && int (angle2)<-200 && int (angle3)<-170 && int (angle4)<-150  && int (angle5)<-170) 
{
SerialBT.println("S");
delay(500);
}

}
//horizontal
else if(int(a.acceleration.x)<3 )
{

if(int (angle1)<-160  && int (angle2)<-200 && int (angle3)<-170 && int (angle4)<-150  && int (angle5)<-170) 
{
SerialBT.println("E");
delay(500);
}
else if(int (angle1)<-115 && int (angle2)<-127 && int(angle2)>-147 && int (angle3)<-124 && int (angle4)<-170 && int (angle5)<-130)
{
SerialBT.println("G");
delay(500);
}
else if(int (angle1)<-145 && int (angle2)<-120 && int (angle3)<-124 && int (angle4)<-170 && int (angle5)<-143)
{
SerialBT.println("H");
delay(500);
}
else if(int (angle1)<-160 && int (angle2)<-197 && int (angle3)<-185 && int (angle4)<-162 && int (angle5)<-110)
{
SerialBT.println("I");
delay(500);
}
else if(int (angle1)<-174 && int (angle2)<-123 && int (angle3)<-205 && int (angle4)<-168 && int (angle5)<-120)
{
SerialBT.println("U");//chinese ,horizontal of T
delay(500);
}
else if(int (angle1)<-130 && int (angle2)<-121 && int (angle3)<-124 && int (angle4)<-165 && int (angle5)<-145)
{
SerialBT.println("P");
delay(500);
}
else if(int (angle1)<-164 && int (angle2)<-220 && int (angle3)<-129 && int (angle4)<-120 && int (angle5)<-114)
{
SerialBT.println("Q");//horizontal of F
delay(500);
}
/*else if(int (angle1)<-140 && int (angle2)<-120 && int (angle3)<-160 && int (angle4)<-140 && int (angle5)<-140 )
{
SerialBT.println("Z");
delay(500);
}
*/

else if(int (angle1)<-152 && int (angle2)<-127 && int (angle3)<-125 && int (angle4)<-177 && int (angle5)<-119)
{
SerialBT.println("N");//ZH chinese
delay(500);
}
else if(int (angle1)<-108 && int (angle2)<-189 && int (angle3)<-200 && int (angle4)<-160 && int (angle5)<-110)
{
SerialBT.println("J");//
delay(500);
}
}
}
