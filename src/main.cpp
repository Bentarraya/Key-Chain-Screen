#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>
#include "VideoFrame.h"

// ================= KONFIGURASI =================
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64
#define OLED_ADDR      0x3C

#define I2C_SDA        8      // sesuaikan dengan board lu
#define I2C_SCL        9
#define TOUCH_PIN      2      // output modul touch (TTP223 dll)
#define TOUCH_ACTIVE   HIGH   // TTP223 default HIGH saat disentuh

// Kalau nama array/konstanta di VideoFrame.h beda, ubah 3 baris ini aja
#define VIDEO_FRAMES       video_frames   // const unsigned char video_frames[][1024] PROGMEM
#define VIDEO_FRAME_COUNT  TOTAL_FRAMES   // 425 frame
#define VIDEO_FRAME_DELAY  FRAME_DELAY    // 33 ms (~30 fps)

// Durasi tiap tahap sequence mata (ms)
#define EYE_DEFAULT_MS 3000
#define EYE_HAPPY_MS   3000
#define EYE_LAUGH_MS   2500
#define EYE_ANGRY_MS   3500

#define DEBOUNCE_MS      60    // anti-bouncing per sentuhan
#define DOUBLE_TAP_MS    800   // jeda maksimal antar 2 sentuhan
// ===============================================

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RoboEyes<Adafruit_SSD1306> roboEyes(display);

enum Mode { MODE_EYES, MODE_VIDEO };
Mode currentMode = MODE_EYES;

// state sequence mata
uint8_t eyeStep = 0;
unsigned long eyeStepStart = 0;

// state video
uint16_t videoIdx = 0;
unsigned long lastFrameTime = 0;

// state touch
bool lastTouch = false;
unsigned long lastTouchTime = 0;
uint8_t touchCount = 0;
unsigned long firstTouchTime = 0;

void startEyes() {
  currentMode = MODE_EYES;
  eyeStep = 0;
  eyeStepStart = millis();
  roboEyes.setMood(DEFAULT);   // mulai dari ekspresi default
}

void startVideo() {
  currentMode = MODE_VIDEO;
  videoIdx = 0;
  lastFrameTime = 0;
}

void updateEyes() {
  unsigned long now = millis();

  switch (eyeStep) {
    case 0:  // default (tanpa ekspresi) -> senang
      if (now - eyeStepStart >= EYE_DEFAULT_MS) {
        eyeStep = 1;
        eyeStepStart = now;
        roboEyes.setMood(HAPPY);
      }
      break;
    case 1:  // senang -> ketawa
      if (now - eyeStepStart >= EYE_HAPPY_MS) {
        eyeStep = 2;
        eyeStepStart = now;
        roboEyes.setMood(HAPPY);
        roboEyes.anim_laugh();
      }
      break;
    case 2:  // ketawa -> marah
      if (now - eyeStepStart >= EYE_LAUGH_MS) {
        eyeStep = 3;
        eyeStepStart = now;
        roboEyes.setMood(ANGRY);
      }
      break;
    case 3:  // marah -> balik default (loop)
      if (now - eyeStepStart >= EYE_ANGRY_MS) {
        eyeStep = 0;
        eyeStepStart = now;
        roboEyes.setMood(DEFAULT);
      }
      break;
  }

  roboEyes.update();  // gambar mata (non-blocking)
}

void updateVideo() {
  unsigned long now = millis();
  if (now - lastFrameTime < VIDEO_FRAME_DELAY) return;
  lastFrameTime = now;

  display.clearDisplay();
  display.drawBitmap(0, 0, VIDEO_FRAMES[videoIdx], SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.display();

  videoIdx++;
  if (videoIdx >= VIDEO_FRAME_COUNT) videoIdx = 0;  // loop terus
}

void checkTouch() {
  bool touched = (digitalRead(TOUCH_PIN) == TOUCH_ACTIVE);
  unsigned long now = millis();

  // rising edge + debounce = 1 sentuhan valid
  if (touched && !lastTouch && (now - lastTouchTime > DEBOUNCE_MS)) {
    lastTouchTime = now;

    if (touchCount == 0) {
      touchCount = 1;            // sentuhan pertama, tunggu yang kedua
      firstTouchTime = now;
    } else if (now - firstTouchTime <= DOUBLE_TAP_MS) {
      touchCount = 0;            // sentuhan kedua tepat waktu -> pindah animasi
      if (currentMode == MODE_EYES) startVideo();
      else                          startEyes();
    }
  }
  lastTouch = touched;

  // kalau sentuhan kedua telat, anggap batal
  if (touchCount == 1 && (now - firstTouchTime > DOUBLE_TAP_MS)) {
    touchCount = 0;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);

  Wire.begin(I2C_SDA, I2C_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("OLED ga ketemu, cek wiring!"));
    for (;;) delay(1000);
  }

  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 60);  // 60 fps
  roboEyes.setAutoblinker(ON, 3, 2);
  roboEyes.setIdleMode(ON, 2, 2);

  startEyes();  // boot -> default -> senang -> ketawa -> marah -> loop
}

void loop() {
  checkTouch();

  if (currentMode == MODE_EYES) updateEyes();
  else                          updateVideo();
}