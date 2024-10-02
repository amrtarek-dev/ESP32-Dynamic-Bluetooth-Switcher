# ESP32 - Bluetooth Dynamic MAC Address Switcher

This project demonstrates how to dynamically change the ESP32's Bluetooth Low Energy (BLE) MAC address and device name using a list of pre-defined MAC addresses. 
It also shows how to use the built-in button to cycle through different MAC addresses and update the BLE device name accordingly.

## Features
- Dynamically switch the ESP32's Bluetooth MAC address.
- Update the BLE device name to reflect the current MAC address.
- Use the built-in button (GPIO 0) to cycle through a list of MAC addresses.
- Automatic BLE advertising with the updated MAC and device name.

## Hardware Used
- **ESP32-WROOM**: This project is specifically designed for the ESP32-WROOM module.
- **Built-in Button (GPIO 0)**: Used to cycle through the MAC addresses.

## Software Used
- **Visual Studio Code (VSCode)**: The recommended editor for this project.
- **PlatformIO Plugin**: Used as the build and development environment within VSCode.

### Installing PlatformIO
1. Open VSCode.
2. Install the PlatformIO IDE plugin from the Extensions Marketplace.
3. Open the project folder in VSCode, and PlatformIO will automatically configure the environment.

## How the Project Works
1. **MAC Address List**: The project maintains a list of MAC addresses stored as strings.
2. **MAC Address Switching**: Each time the button is pressed (or after a delay), the ESP32 switches to the next MAC address in the list.
3. **Device Name Update**: The BLE device name is updated to include the current MAC address, ensuring that each MAC address has a unique identifier visible to BLE scanners.
4. **BLE Advertising**: BLE advertising is restarted every time a new MAC address is selected.

## Code Explanation
### MAC Address and BLE Device Name

The MAC addresses are stored in an array of strings. Each string is converted to a `uint8_t` array, which is the required format for the ESP32 to set the MAC address. The BLE device name is also updated based on the selected MAC address.

```cpp
const char* macAddresses[] = {
  "01:02:03:04:05:06",  // MAC Address 1
  "AA:BB:CC:DD:EE:02",  // MAC Address 2
  "AA:BB:CC:DD:EE:03"   // MAC Address 3
};
```
## Button Control
The built-in button on GPIO 0 is used to switch between the MAC addresses.

```cpp
const int buttonPin = 0;
```
Each time the button is pressed, the next MAC address is selected and applied, and BLE advertising is restarted with the updated device name.

BLE Advertising with Updated Name
The BLE device name is dynamically updated to reflect the current MAC address and is then restarted with the new name:

```cpp
BLEDevice::init(deviceName.c_str());
BLEDevice::getAdvertising()->start();
```
## How to Run the Project
- Clone or download this repository.
- Open the project folder in VSCode with the PlatformIO plugin.
- Connect your ESP32-WROOM to your computer.
- Build and upload the code using PlatformIO.
- Use a BLE scanner (such as on your phone) to view the device name and switch between MAC addresses.

## License
This project is open-source under the MIT License.

Author
Amr Tarek

## Contact
[AmrTarek.dev](https://amrtarek.dev)

[LinkedIn](https://www.linkedin.com/in/amrtarek-dev/)
