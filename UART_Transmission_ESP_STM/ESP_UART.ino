int receivedNumber = 0;  // Variable to store the received number

void setup() {
  Serial.begin(115200);  // Debugging via USB Serial
  Serial.println("ESP32 is ready. Please enter a number:");
}

void loop() {
  // Check if data is available in the UART buffer (waiting for user input)
  if (Serial.available()) {  
    char incomingChar = Serial.read();  // Read one character at a time
    
    // Debugging: Print received character
    Serial.print("Received: ");
    Serial.println(incomingChar);

    // Process data (you can modify this section as needed)
    if (incomingChar >= '0' && incomingChar <= '9') {
      // If the incoming character is a digit, append it to the number
      receivedNumber = receivedNumber * 10 + (incomingChar - '0');
    } 
    else if (incomingChar == '\n' || incomingChar == '\r') {
      // If a newline or carriage return is received, process the number
      Serial.print("You sent the number: ");
      Serial.println(receivedNumber);
      
      // Clear the number buffer for the next input
      receivedNumber = 0;
    }
  }
}
