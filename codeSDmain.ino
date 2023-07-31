#include <SD.h> // Include the SD card library

// Define the analog input pin for the soil moisture sensor
const int soilMoist = A0;

// Define the digital output pin for the water pump motor
const int dcMotor = 9;

// Define the minimum moisture level at which the plant needs to be watered
const int minMoist = 50;

// Boolean variable to track whether the water pump is currently running or not
bool waterPump = false;

// File to store data on the SD card
File dataFile;

// Setup function runs once when the Arduino starts
void setup()
{
    // Set the soil moisture pin as an input
    pinMode(soilMoist, INPUT);

    // Set the water pump motor pin as an output and turn it off initially
    pinMode(dcMotor, OUTPUT);
    digitalWrite(dcMotor, LOW);

    // Initialize serial communication at 9600 baud rate
    Serial.begin(9600);

    // Initialize the SD card
    if (SD.begin())
    {
        Serial.println("SD card initialized.");
    }
    else
    {
        Serial.println("SD card initialization failed!");
    }

    // Open the data file in write mode
    dataFile = SD.open("data_log.csv", FILE_WRITE);

    // Write the header to the CSV file
    dataFile.println("Timestamp,Soil Moisture (%)");
    dataFile.close();
}

// Loop function runs repeatedly after setup
void loop()
{
    // Read the analog value from the soil moisture sensor
    int soilMoisture = analogRead(soilMoist);

    // Map the analog value to a percentage representing soil moisture
    int moisturePercentage = map(soilMoisture, 0, 1023, 0, 100);

    // Print the current soil moisture percentage to the serial monitor
    Serial.print("Soil Moisture: ");
    Serial.print(moisturePercentage);
    Serial.println("%");

    // Log data to the SD card
    logData(moisturePercentage);

    // Check if the moisture level is below the minimum threshold and the water pump is not already running
    if (moisturePercentage < minMoist && !waterPump)
    {
        // Turn on the water pump
        digitalWrite(dcMotor, HIGH);
        Serial.println("Watering the plant...");
        waterPump = true;
    }
    // Check if the moisture level is equal to or above the minimum threshold and the water pump is currently running
    else if (moisturePercentage >= minMoist && waterPump)
    {
        // Turn off the water pump
        digitalWrite(dcMotor, LOW);
        Serial.println("Stopping the motor.");
        waterPump = false;
    }

    // Add a delay of 1000 milliseconds (1 second) before looping again
    delay(1000);
}

// Function to log data to the CSV file
void logData(int moisture)
{
    // Get the current timestamp
    unsigned long currentTime = millis();

    // Open the data file in append mode
    dataFile = SD.open("data_log.csv", FILE_WRITE);

    // Log data if the file is available and opened
    if (dataFile)
    {
        // Write timestamp and moisture data to the file
        dataFile.print(currentTime);
        dataFile.print(",");
        dataFile.println(moisture);

        // Close the file
        dataFile.close();
    }
    else
    {
        Serial.println("Error opening data file.");
    }
}
