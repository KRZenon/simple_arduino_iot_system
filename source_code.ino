#include <SoftwareSerial.h>
#include <DallasTemperature.h>

SoftwareSerial mySerial(2, 3);

int ledPin13 = 13; // LED ở chân D13 (có sẵn trên Arduino)
int ledPin12 = 12; // LED ở chân D12
int fan = 6;       // Quạt ở chân D6
int temp = 4;      // Chân Data của DS18B20 kết nối vào chân D4

bool manualControl = false; // Biến để xác định liệu điều khiển là tự động (dựa trên nhiệt độ) hay thủ công (từ ứng dụng)

OneWire oneWire(temp);
DallasTemperature sensors(&oneWire);

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  pinMode(ledPin13, OUTPUT);
  pinMode(ledPin12, OUTPUT);
  pinMode(fan, OUTPUT);
}

void loop() {
  // Kiểm tra và điều khiển quạt và đèn dựa trên dữ liệu từ ứng dụng
  checkAppCommands();

  // Đọc nhiệt độ từ cảm biến DS18B20
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  // Nếu đang trong chế độ tự động (dựa trên nhiệt độ)
  if (!manualControl) {
    // Hiển thị nhiệt độ dưới dạng số thập phân trên Serial Monitor
    Serial.print("T:");
    Serial.print(temperature, 2); // Hiển thị 2 chữ số thập phân
    Serial.println();

    // Hiển thị nhiệt độ dưới dạng số thập phân trên BLE Scanner
    mySerial.print("T:");
    mySerial.print(temperature, 2); // Hiển thị 2 chữ số thập phân
    mySerial.println();

    // Kiểm tra và điều khiển quạt và đèn dựa trên nhiệt độ
    if (temperature > 30.0) {
      digitalWrite(fan, HIGH);  // Bật quạt
      digitalWrite(ledPin12, LOW);
    } else {
      digitalWrite(fan, LOW);  // Tắt quạt
      digitalWrite(ledPin12, HIGH);
    }
  }

  delay(1000); // Đợi 1 giây để giảm tần suất kiểm tra nhiệt độ
}

void checkAppCommands() {
  while (mySerial.available()) {
    char command = mySerial.read();
    Serial.println("DATA RECEIVED:");

    switch (command) {
      case '0':
        digitalWrite(ledPin13, LOW);
        Serial.println("LED 13 off");
        manualControl = true; // Chuyển sang chế độ điều khiển thủ công
        break;

      case '1':
        digitalWrite(ledPin13, HIGH);
        Serial.println("LED 13 on");
        manualControl = true; // Chuyển sang chế độ điều khiển thủ công
        break;

      case '2':
        digitalWrite(ledPin12, LOW);
        Serial.println("LED 12 off");
        manualControl = true; // Chuyển sang chế độ điều khiển thủ công
        break;

      case '3':
        digitalWrite(ledPin12, HIGH);
        Serial.println("LED 12 on");
        manualControl = true; // Chuyển sang chế độ điều khiển thủ công
        break;

      case 'Q':
        digitalWrite(fan, HIGH);
        Serial.println("Fan on");
        manualControl = true; // Chuyển sang chế độ điều khiển thủ công
        break;

      case 'q':
        digitalWrite(fan, LOW);
        Serial.println("Fan off");
        manualControl = true; // Chuyển sang chế độ điều khiển thủ công
        break;

      case 'A':
        manualControl = false; // Chuyển sang chế độ tự động
        break;

      case 'a':
        manualControl = true; // Chuyển sang chế độ thủ công
        break;

      // Add more cases for additional commands if needed

      default:
        // Handle unexpected characters or commands
        break;
    }
  }
}
