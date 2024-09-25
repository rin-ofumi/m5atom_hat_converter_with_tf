// M5Atom Lite/Matrix/ATOMS3/ATOMS3 Lite/ATOMS3R用
// 「HAT Converter with TF」のサンプルプログラム
// @rin_ofumi 2024.9.25
//
// 確認した機種
// - M5Atom Lite    未確認
// - M5Atom Matrix  未確認
// - M5ATOMS3       OK
// - M5ATOMS3 Lite  未確認
// - M5ATOMS3R      OK
// - M5ATOMS3R CAM  未確認
//
// 開発環境
// - Arduino IDE ver2.3.2
//
// ボードマネージャバージョン
// - M5Stack 2.1.2
//
// 使用ライブラリバージョン
// - M5Unified 0.1.17

#include <SD.h>
#include <M5Unified.h>

SPIClass SPI_EXT;

void setup() {
  M5.begin();

  //機種判定（ATOM系とATOMS3系の区別）
  const char* name;
  switch (M5.getBoard()) {
    case m5::board_t::board_M5AtomS3R:      name = "ATOMS3R";     break;
    case m5::board_t::board_M5AtomS3Lite:   name = "ATOMS3Lite";  break;
    case m5::board_t::board_M5AtomS3:       name = "ATOMS3";      break;
    case m5::board_t::board_M5Atom:         name = "ATOM";        break;
    default:                                name = "Who am I ?";  break;
  }
  Serial.printf("> board name = [%s]\n", name);

  //画面向き指定（逆は”3”）
  M5.Lcd.setRotation(1);

  //機種判定に応じ、ピン割り当てを変更
  if (name == "ATOMS3" or name == "ATOMS3Lite" or name == "ATOMS3R") {
    //ATOMS3系 SDカード用SPIポート初期化（SCK:G7, MISO:G8, MOSI:G6, SS:物理ピン無し"-1"）
    SPI_EXT.begin(7, 8, 6, -1);
  } else if (name == "ATOM") {
    //ATOM系 SDカード用SPIポート初期化（SCK:G23, MISO:G33, MOSI:G19, SS:物理ピン無し"-1"）
    SPI_EXT.begin(23, 33, 19, -1);
  }

  //SDカード初期設定
  if (!SD.begin(-1, SPI_EXT, 15000000)) {
    M5.Lcd.println("SD Card Mount Failed!");
    Serial.println("> SD Card Mount Failed!");
    return;
  }

  //SDカード種別取得
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    M5.Lcd.println("None SD Card!");
    Serial.println("> None SD Card!");
    return;
  } else if (cardType == CARD_MMC) {
    M5.Lcd.println("SD Card Type: MMC");
    Serial.println("> SD Card Type: MMC");
  } else if (cardType == CARD_SD) {
    M5.Lcd.println("SD Card Type: SDSC");
    Serial.println("> SD Card Type: SDSC");
  } else if (cardType == CARD_SDHC) {
    M5.Lcd.println("SD Card Type: SDHC");
    Serial.println("> SD Card Type: SDHC");
  } else {
    M5.Lcd.println("SD Card Type: UNKNOWN");
    Serial.println("> SD Card Type: UNKNOWN");
  }

  //SDカード容量取得
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  M5.Lcd.printf("SD Card Size: %lluMB\n", cardSize);
  M5.Lcd.println();
  Serial.printf("> SD Card Size: %lluMB\n", cardSize);
  Serial.print("\n");

  //SDカードのルートディレクトリを開く
  File dir = SD.open("/");

  //SDカード内のディレクトリ・ファイル名を表示
  while (1) {
    File entry =  dir.openNextFile();
    if (!entry) {
      break;
    }
    M5.Lcd.println(entry.name());
    Serial.println(entry.name());
    entry.close();
  }
}

void loop() {
}
