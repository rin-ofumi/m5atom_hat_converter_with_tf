#include <M5Atom.h>
#include "SD.h"

SPIClass SPI2;

void setup() {
  M5.begin(true,false,true);

  //SDカード用SPIポート初期化（SCK:G23, MISO:G33, MOSI:G19, SS:物理ピン無し"-1"）
  SPI2.begin(23, 33, 19, -1);

  //SDカード初期設定
  if (!SD.begin(-1, SPI2)) {
    Serial.println("SD Card Mount Failed!");
    return;
  }

  //SDカード種別取得
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("None SD Card!");
    return;
  } else if (cardType == CARD_MMC) {
    Serial.println("SD Card Type: MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SD Card Type: SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SD Card Type: SDHC");
  } else {
    Serial.println("SD Card Type: UNKNOWN");
  }

  //SDカード容量取得
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  Serial.println();

  //SDカードのルートディレクトリを開く
  File dir = SD.open("/");

  //SDカード内のディレクトリ・ファイル名を表示
  while (1) {
    File entry =  dir.openNextFile();
    if (!entry) {
      break;
    }
    Serial.println(entry.name());
    entry.close();
  }
}

void loop() {
}
