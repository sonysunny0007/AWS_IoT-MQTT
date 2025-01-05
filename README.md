# ESP32 AWS IoT Example

This is a simple example demonstrating how to connect an ESP32 device to AWS IoT using MQTT. The example demonstrates how to publish data to AWS IoT and how to receive messages from AWS IoT.

## Features

- Connects ESP32 to a WiFi network.
- Establishes a secure MQTT connection with AWS IoT using certificates.
- Publishes a JSON message with simulated sensor data to an MQTT topic.
- Subscribes to an MQTT topic and receives messages sent to that topic.

## Prerequisites

Before running this code, ensure you have the following:

- **ESP32 development board**.
- **AWS IoT setup** (including device certificates and private keys).
- A **Wi-Fi network** to connect the ESP32 to the internet.
- **Libraries**:
  - `WiFiClientSecure`
  - `PubSubClient` (install via the Arduino Library Manager, version 2.6.0)

## How to Use

1. **Wi-Fi Credentials**: Replace the following values with your Wi-Fi credentials:
   - `ssid` - Your Wi-Fi network name.
   - `password` - Your Wi-Fi password.

2. **AWS IoT Setup**: Replace the AWS IoT credentials with the correct values:
   - `awsEndpoint` - Your AWS IoT endpoint.
   - `certificate_pem_crt` - Your AWS IoT device certificate.
   - `private_pem_key` - Your AWS IoT private key.
   - `rootCA` - Your root CA certificate.

3. **Upload the Code**: Upload the code to your ESP32 board using the Arduino IDE or any other suitable development environment.

4. **Monitor Serial Output**: Open the serial monitor to view the connection status and messages sent to and received from AWS IoT.

## Example Output

The ESP32 will publish the following message every 10 seconds:

```json
{"uptime":"12","temp":"50","humid":"23"}
