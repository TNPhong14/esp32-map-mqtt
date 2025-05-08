#include <WiFi.h>
#include <WiFiClientSecure.h> // Rõ ràng bao gồm WiFiClientSecure
#include <PubSubClient.h>

// Thông tin WiFi
const char* ssid = "RD-2.4Ghz";
const char* password = "@12345678";

// Thông tin HiveMQ Cloud
const char* mqtt_server = "d2aa02aedbf34bef80d67bcc4e63f070.s1.eu.hivemq.cloud";
const int mqtt_port = 8883; // Cổng TLS
const char* mqtt_user = "TNPhong";
const char* mqtt_pass = "Tranngocphong14@";
const char* mqtt_client_id = "ESP32_Map_Client";
const char* mqtt_topic = "esp32/map/data";

// Khởi tạo WiFi và MQTT
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Dữ liệu lộ trình mẫu
const char* distance = "25.6 km";
const char* duration = "45 mins";

void setup() {
  Serial.begin(115200);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Cấu hình MQTT
  espClient.setInsecure(); // Bỏ qua xác minh chứng chỉ TLS (dùng cho thử nghiệm)
  client.setServer(mqtt_server, mqtt_port);

  // Kết nối MQTT
  reconnectMQTT();
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect(mqtt_client_id, mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Gửi dữ liệu lộ trình qua MQTT
  String payload = "{\"distance\":\"" + String(distance) + "\",\"duration\":\"" + String(duration) + "\"}";
  client.publish(mqtt_topic, payload.c_str());
  Serial.println("Published: " + payload);

  delay(10000); // Gửi mỗi 10 giây
}