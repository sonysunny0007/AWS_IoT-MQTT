/* ESP32 AWS IoT
 *  
 * Simplest possible example (that I could come up with) of using an ESP32 with AWS IoT.
 *  
 * Author: Anthony Elder 
 * License: Apache License v2
 */
#include <WiFiClientSecure.h>
#include <PubSubClient.h> // install with Library Manager, I used v2.6.0

const char* ssid = "GNXS-549580";
const char* password = "password1172";

const char* awsEndpoint = "a1bhyjsrk0nqzi-ats.iot.ap-south-1.amazonaws.com";

// Update the two certificate strings below. Paste in the text of your AWS 
// device certificate and private key. Add a quote character at the start
// of each line and a backslash, n, quote, space, backslash at the end 
// of each line:

// xxxxxxxxxx-certificate.pem.crt
const char* certificate_pem_crt = \

"-----BEGIN CERTIFICATE-----\n" \
"MIIDWTCCAkGgAwIBAgIUGyL0jQLS62tfNB2O4ilxge4kJJ8wDQYJKoZIhvcNAQEL\n" \
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n" \
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIxMTIxNTEwMDky\n" \
"NVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n" \
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANGUrRM9Ti6EvruABsoG\n" \
"g1ruSbbdKzaOahSb4WZDLB+UfAMZM+rqUAk1lbrvdvpn+r/8a/GI4MxFTmjZIf7O\n" \
"rUnEb5FUIy4fCiF6kYmo35TZAgo8VsYK6VA0otVq6OkJTrU5ENKHcN1NPaAQ+tJu\n" \
"jjyg72BbeOGJ4+M1Ulieu6+GPu3E3ju/Nv5DR8tWrqJhuifKK3DBv+PgghCTVGKF\n" \
"lIW73fuOHvDTRGZbslHXfpSl5cxtzSw51QPK8U760boONEyhhamLOQKaODCUeUHQ\n" \
"XpKkjjfZ2UHJhaW6w2ZloeFv9A1A2J3pmkMlVneMw5lHODabECQ0qrgZbDfpLnTz\n" \
"SLMCAwEAAaNgMF4wHwYDVR0jBBgwFoAUiFJRhHhF6FQapx6SbwM8/RV9puowHQYD\n" \
"VR0OBBYEFA0n7QKXPN+Y8MTpCWdlAubkVsdBMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n" \
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBX3zztKzLaIxZDZAdg12bEPfMu\n" \
"CDxPxJi/uZI1JVWPm6UgtuzpbBpSGoRSGyfXlybVQyQ7xWpbp+7XsAFp4Zd/xqI/\n" \
"UI53oGaBPeOBcmqgmrvm+x7iKC5Omw0MpT398Go6B/7EtZgk8db8SHzF3KM2aTFM\n" \
"9f3kCRlsL7fhpzILv52dk5wfVsE6DKhIukIhaFwRQW6lMB2EV73PTMR4jXr3qflW\n" \
"YmAtTS98sTMQ61vn509qtbSZObFRaxgWVOHXwEyUTID6N91mSTsqs5OHKJkWiGj7\n" \
"7w+QHsOtQmwe9ZEGaBTpWsGv9pntaGgtXLqUyf+P6oiJEz74ElX9PcOXrPzo\n" \
"-----END CERTIFICATE-----\n";

// xxxxxxxxxx-private.pem.key
const char* private_pem_key = \

"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEpAIBAAKCAQEA0ZStEz1OLoS+u4AGygaDWu5Jtt0rNo5qFJvhZkMsH5R8Axkz\n" \
"6upQCTWVuu92+mf6v/xr8YjgzEVOaNkh/s6tScRvkVQjLh8KIXqRiajflNkCCjxW\n" \
"xgrpUDSi1Wro6QlOtTkQ0odw3U09oBD60m6OPKDvYFt44Ynj4zVSWJ67r4Y+7cTe\n" \
"O782/kNHy1auomG6J8orcMG/4+CCEJNUYoWUhbvd+44e8NNEZluyUdd+lKXlzG3N\n" \
"LDnVA8rxTvrRug40TKGFqYs5Apo4MJR5QdBekqSON9nZQcmFpbrDZmWh4W/0DUDY\n" \
"nemaQyVWd4zDmUc4NpsQJDSquBlsN+kudPNIswIDAQABAoIBAQCsdAGCrlRx1Ms5\n" \
"cBymCnv1y9QeQ5l2V/OUJ7xuMGHTTpRI3GkrKUlYep5NZxRo8jyT07pi0nXfwDvH\n" \
"7LubdyeCZsBDPo+AOQxWBgxR0QLA8VxLXeEtLvdTByziHgmvD/iTCHT+N3K4twE5\n" \
"gB9UHqKt4uN2T26Fb7h0UoEC6QKZZ4kp6LhvUOXsEOQHBmdRzV2cqMjIv3LRMo80\n" \
"+OhVb8SBLEMdF00GH1AXgqPQMk7vMh+NFY+PCySe+C12KLQ3W2VLey5d33y+CjUo\n" \
"MrXT2hYeYKJa9znVTWxLE7V0gWyc5FEJWYam4Ef2MnJvMaCpT5sA97dkPjzYs7QC\n" \
"KoRkKmLhAoGBAPLfxHwY22Q3s4bbbDTvDn1KQllUowGhNQ5KWymYKIEXVhvJumxI\n" \
"XMzRk92bAdeJ5s3TFYEDqjsGch6OqHPz+eoU2hwTJtJMlnzVwvSXD0jE1tpA/M0c\n" \
"2vQN/xN5O/OA4ovcZlxqMyWEtj/rj4oURFjj8lBU125WOuWXOzXFbzGDAoGBANzo\n" \
"SS8jLFXGDLtNfWcuTAoCSTGyM0zrQPeBYpOpvtC/iXrFn3zzkhlsEMPEgrRtM8Z2\n" \
"3S6ABO3bbfIwqZa8D1JAK2+QWsCHi316dq+cpC9CvwaqnKJbKFCooNbp73FFLjtH\n" \
"kOFmZzecyfSGkU7n43V7dltBgunkJwL5r7Em9dURAoGBAM9eJ1EdmBSVSn9WUalQ\n" \
"gzqOcN/818DId5hf/D4jYFH4LtlDABz8G9/1AR8w/f2eDKaoF7A0BOCovD9C5qk6\n" \
"IqOuchuObpMYOVUU3oQfhjlHd3qaCj7G6FuAp81ksb9SyBtKb0twkYLzMPiak8/9\n" \
"Re3vkjDsf5QBo//rg+6BSFMpAoGADNDE/iESP1+f+XXDYqsC6km7qVBnPX0gZbtx\n" \
"1pMwaLfT5/VzDTKsNSFZz6Gk24bCTC81Ia93mbzzIevEyuPaF7F1U6GSQj0a0Iq8\n" \
"O+/gboV2w5I+SAT1z8gYnJ8xruaOig3az9i7ADTuWms8HtHBFl21PhIM4DjGMpv9\n" \
"n09dT6ECgYBHIvk+EsIsItisBohQFdD/M3VSsLRu4aD9AXur/IH3ZY67f3nAFdxh\n" \
"B8ghLKQebC034eE2zdN/lLytr/+7If79b0gaYPjwRIH1pZY+zjwGltSEJT+yCAw9\n" \
"wD24rHjYH+G1eRfKF4N0hHqYvDd8LRkO294nv2sV7lqgmfbTX8iUzg==\n" \
"-----END RSA PRIVATE KEY-----\n";

/* root CA can be downloaded in:
  https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem
*/
const char* rootCA = \

"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----\n";


WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

void setup() {
  Serial.begin(115200); delay(50); Serial.println();
  Serial.println("ESP32 AWS IoT Example");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());
  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();

  if (millis() - lastPublish > 10000) {
//    String msg = String("Hello from sonydevice: ") + ++msgCount;
//    boolean rc = pubSubClient.publish("outTopic", msg.c_str());
//    Serial.print("Published, rc="); Serial.print( (rc ? "OK: " : "FAILED: ") );
//    Serial.println(msg);
    String msg = String("Hello from sonydevice: ");
    pubSubClient.publish("outTopic", "{\"uptime\":\"12\",\"temp\":\"50\",\"humid\":\"23\"}" );
    Serial.println("{\"uptime\":\"12\",\"temp\":\"50\",\"humid\":\"23\"}");
    lastPublish = millis();
  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthingXXXX");
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe("outTopic");
  }
  pubSubClient.loop();
}
