//MCGE PM2.5 Sensor Firmware
//Waits for the serial port to be opened by a terminal program.
//Streams data from the PM2.5 sensor, to the serial port.
//On-board LGB LED gives feedback when readings are successful or fail.
//Three LED pins turn on at certain PM2.5 concentraiton thresholds.

//Libraries
    #include <Wire.h>
    #include "Adafruit_PM25AQI.h"
    #include <Adafruit_DotStar.h>

//Air quality sensor instance
    Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

//Definitions and global constants
    #define DATAPIN 41
    #define CLOCKPIN 40

    int greenPin = 12;
    int yellowPin = 11;
    int redPin = 10;
    int startTime;

//Initialize on-board RGB DotStar
    Adafruit_DotStar dot(1, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
  
    
    void setup() {
      // Wait for serial monitor to open
      Serial.begin(115200);
      while (!Serial) delay(10);

      //Initialize on-board RGB LED
      dot.begin(); dot.show();

      //Initialize Digital LED Pins
      pinMode(greenPin, OUTPUT); pinMode(yellowPin, OUTPUT); pinMode(redPin, OUTPUT);
      digitalWrite(redPin,LOW);digitalWrite(greenPin,LOW);digitalWrite(yellowPin,LOW);
     
      Serial.println("PMSA003I Air Quality Sensor");
     
      // Wait one second for sensor to boot up
      delay(1000);

      Serial1.begin(9600);
      
     // Connect to the sensor over hardware serial
      if (! aqi.begin_UART(&Serial1)) { 
        Serial.println("Could not find PM 2_5 sensor!");
        while (1) delay(50);}
     
      Serial.println("PM2_5 found!"); startTime = millis();
    }
     
    void loop() {
      delay(500);
      PM25_AQI_Data data;

      //Aquire data from the PM2.5 Sensor; stream to Serial
      if (! aqi.read(&data)) {
        Serial.print("PM 2_5 (ug/m^3): "); Serial.print('\t'); Serial.print("Failed"); Serial.print('\t'); Serial.println((millis()-startTime)/1000);
        dot.setPixelColor(0,0,100,0); dot.show();
        delay(500);  // Try again if reading isn't successful
        return;
      }

      //Print the reading to the serial port
      Serial.print("PM 2_5 (ug/m^3): "); Serial.print('\t'); Serial.print(data.pm25_standard); Serial.print('\t'); Serial.println((millis()-startTime)/1000);
      dot.setPixelColor(0,100,0,0); dot.show();

      //Scale the redness of the RGB LED based on the reading
      //dot.setPixelColor(0,0,data.pm25_standard,0); dot.show();

        //Turn on or off digital LED pins based on PM2.5 reading
        if(data.pm25_standard > 5){digitalWrite(greenPin, HIGH);} else{digitalWrite(greenPin, LOW);}
        if(data.pm25_standard > 100){digitalWrite(yellowPin, HIGH);} else{digitalWrite(yellowPin, LOW);}
        if(data.pm25_standard > 300){digitalWrite(redPin, HIGH);} else{digitalWrite(redPin, LOW);}
      
      delay(250);
    }
