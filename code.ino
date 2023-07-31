// Define the analog input pin for the soil moisture sensor
const int soilMoist = A0;

// Define the digital output pin for the water pump motor
const int dcMotor = 9;

// Define the minimum moisture level at which the plant needs to be watered
const int minMoist = 50;

// Boolean variable to track whether the water pump is currently running or not
bool waterPump = false;

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
