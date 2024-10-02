#include "BluetoothSerial.h" // Bluetooth library
#include <BLEDevice.h>
#include <esp_bt.h>  // Required for setting the MAC address
#include <string.h>  // For converting strings to bytes

BluetoothSerial SerialBT;

const int buttonPin = 0;  // GPIO pin for the built-in button (boot button)
int buttonState = 0;      // Current state of the button
int lastButtonState = 1;  // Previous state of the button (1 = not pressed, 0 = pressed)
int currentDevice = 0;    // Current MAC address index

// List of MAC addresses to switch between
const char* macAddresses[] = {
  "24:0A:C4:5A:02:11",  // MAC Address 1
  "24:0A:C4:5A:02:12",  // MAC Address 2
  "24:0A:C4:5A:02:13"   // MAC Address 3
};
// List of MAC addresses to switch between
const char* addressesNames[] = {
  "01",  // MAC Address 1
  "02",  // MAC Address 2
  "03"   // MAC Address 3
};
const int totalDevices = sizeof(macAddresses) / sizeof(macAddresses[0]);

// Function to set the MAC address based on a string
void setBTNameMAC(const char* nameStr, const char* macStr) {
  String deviceName = "ESP32_";
  deviceName += String(nameStr);  // Append MAC address to the device name
  uint8_t mac[6];  // To hold the converted MAC address in byte form
  // Set the base MAC address using esp_base_mac_addr_set
  // Convert the string (e.g., "01:02:03:04:05:06") to a uint8_t array
  sscanf(macStr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
  esp_err_t result = esp_base_mac_addr_set(mac); 
  // Check if the MAC address was set successfully
  if (result == ESP_OK) {
      // Initialize BLE with the new device name
    BLEDevice::init(deviceName.c_str());
    // Start the Bluetooth server
    SerialBT.begin(deviceName.c_str());
    Serial.println("Bluetooth Started!");
    Serial.print("MAC address successfully set to:"); Serial.println(macStr);
    Serial.print("BLE device name updated to: "); Serial.println(deviceName);
  } else {
    Serial.print("Failed to set MAC address, error: "); Serial.println(result);
  }
}

void setup() {
  Serial.begin(115200);
  // Set the initial MAC address from the list and update device name
  setBTNameMAC(addressesNames[currentDevice], macAddresses[currentDevice]);
  // Start BLE advertising with the new MAC address
  BLEDevice::getAdvertising()->start();
  Serial.println("BLE advertising started with the new MAC address.");
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Check if the button is pressed (active low, changes from HIGH to LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Change the mac address
    currentDevice = (currentDevice + 1) % totalDevices;
    // Stop advertising before changing the name
    if(BLEDevice::getInitialized){
      Serial.println("It was initialized.");
      BLEDevice::deinit();
    }else{
      BLEDevice::stopAdvertising();
    }
    delay(100);
    // Set the initial MAC address from the list and update device name
    setBTNameMAC(addressesNames[currentDevice], macAddresses[currentDevice]);
    // Start BLE advertising with the new MAC address
    delay(100);
    BLEDevice::startAdvertising();
    Serial.println("BLE advertising started with the new MAC address.");
  }

  if (SerialBT.available()) {
    // Read the incoming data
    String command = SerialBT.readStringUntil('\n');
    command.trim(); // Remove any leading/trailing whitespace
    // Process the command
    if (command.equalsIgnoreCase("mac")) {
      SerialBT.println(macAddresses[currentDevice]);
    } else if (command.equalsIgnoreCase("reboot")) {
      SerialBT.println("Rebooting ...");
      ESP.restart();
    } else {
      SerialBT.println("Only Ask for mac");
    }
  }

  // Update the last button state
  lastButtonState = buttonState;
}
