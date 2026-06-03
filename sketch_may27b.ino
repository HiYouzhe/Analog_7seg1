// 定義 VR 輸入腳位
const int vrPin = A0;

// 定義 LED 的起始與結束腳位（使用連續腳位可以讓程式用 for 迴圈精簡化）
const int startLedPin = 22; 
const int endLedPin = 37;
const int ledCount = 16; // 總共 16 顆 LED

void setup() {
  // 初始化串口通訊
  Serial.begin(9600);
  
  // 使用迴圈一次將 Pin 22 ~ 37 設定為輸出模式
  for (int i = startLedPin; i <= endLedPin; i++) {
    pinMode(i, OUTPUT);
  }
  
  Serial.println("--- ATmega2560 16-LED 電平指示器測試 ---");
}

void loop() {
  int analogIn = analogRead(vrPin);
  // 這樣只要 VR 轉到 880 以上（比如你的 887），就會被強制當作 880 計算
  int safeAnalogIn = constrain(analogIn, 0, 880);
  
  // 【修正重點】將最大對映範圍從 1023 改為 880
  int ledsToLight = map(safeAnalogIn, 0, 880, 0, ledCount);
  
  // 3. 根據計算結果點亮對應數量的 LED
  for (int i = 0; i < ledCount; i++) {
    int currentPin = startLedPin + i; 
    
    if (i < ledsToLight) {
      digitalWrite(currentPin, HIGH); // 點亮
    } else {
      digitalWrite(currentPin, LOW);  // 熄滅
    }
  }
  
  // 4. 計算實際電壓值並輸出到串口監視器
  float voltage = analogIn * (5.0 / 1023.0); // 電壓計算維持 1023 基準以反映真實電壓
  
  Serial.print("輸入原始值: ");
  Serial.print(analogIn);
  Serial.print(" | 當前電壓: ");
  Serial.print(voltage, 2);
  Serial.print("V | 點亮 LED 數量: ");
  Serial.println(ledsToLight);
  
  delay(50);
}
