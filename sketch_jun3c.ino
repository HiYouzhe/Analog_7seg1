const int analogPin = A0; 
const int comPins[] = {42, 43, 44, 45}; 
const int segPins[] = {46, 47, 48, 49, 50, 51, 52, 53}; 
const byte numPatterns[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

// ==========================================
// 電位定義 (已依您的硬體反相修正)
// ==========================================
#define COM_ON  LOW   // PNP 電晶體導通
#define COM_OFF HIGH  // PNP 電晶體截止
#define SEG_ON  LOW   // 低電位點亮
#define SEG_OFF HIGH  // 高電位熄滅

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(comPins[i], OUTPUT);
    digitalWrite(comPins[i], COM_OFF); 
  }
  for (int i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);
    digitalWrite(segPins[i], SEG_OFF); // 預設全部熄滅
  }
}

void loop() {
  // 1. 讀取類比值 (0 ~ 1023)
  int val = analogRead(analogPin);
  
  // 為了避免類比訊號微幅飄動（例如明明接地卻讀到 1 或 2）
  // 這裡設定當讀取值小於等於 2 時，就視為絕對的 0V
  if (val <= 2) {
    // -----------------------------------------------------------
    // 【情境一】當輸入電壓為 0 時：只顯示由 COM4 控制的部分（顯示 0）
    // -----------------------------------------------------------
    // 強制熄滅不該亮的位數 (COM1, COM2, COM3)
    digitalWrite(comPins[0], COM_OFF);
    digitalWrite(comPins[1], COM_OFF);
    digitalWrite(comPins[2], COM_OFF);
    
    // 只顯示 COM4 (索引為 3)，數值為 0，無小數點
    show(3, 0, false);
    delay(2);
    
  } else {
    // -----------------------------------------------------------
    // 【情境二】當有輸入電壓時：只顯示在被 COM2 ~ COM4 控制的部分
    // -----------------------------------------------------------
    // 將電壓轉換為 0 ~ 500 的整數 (對應 0.00V ~ 5.00V)
    int displayVal = map(val, 0, 1023, 0, 500); 

    int d1 = displayVal / 100;         // 個位數 (如 4)
    int d2 = (displayVal / 10) % 10;   // 十分位 (如 2)
    int d3 = displayVal % 10;          // 百分位 (如 5)

    // 強制熄滅 COM1 (最左邊第一位)
    digitalWrite(comPins[0], COM_OFF);

    // 顯示在 COM2 (索引 1)：個位數 + 小數點 (例如 "4.")
    show(1, d1, true);
    delay(2);
    
    // 顯示在 COM3 (索引 2)：十分位 (例如 "2")
    show(2, d2, false);
    delay(2);
    
    // 顯示在 COM4 (索引 3)：百分位 (例如 "5")
    show(3, d3, false);
    delay(2);
  }
}

// 顯示副程式
void show(int pos, int num, bool dot) {
  // 先強制關閉所有 COM，避免殘影
  for (int i = 0; i < 4; i++) {
    digitalWrite(comPins[i], COM_OFF);
  }

  // 設定 A ~ G 段碼
  byte pattern = numPatterns[num];
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], bitRead(pattern, i) ? SEG_ON : SEG_OFF);
  }
  
  // 獨立處理小數點 (DOT 對應 PIN 53)
  digitalWrite(segPins[7], dot ? SEG_ON : SEG_OFF);

  // 最後開啟對應的位數 COM
  digitalWrite(comPins[pos], COM_ON);
}