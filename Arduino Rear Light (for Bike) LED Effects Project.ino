#include <FastLED.h>

#define LED_PIN_11 11
#define LED_PIN_10 10
#define LED_PIN_9 9

#define NUM_LEDS_11 20
#define NUM_LEDS_10 9
#define NUM_LEDS_9 9

#define BUTTON_A5 A5
#define BUTTON_A4 A4
#define BUTTON_A3 A3
#define BUTTON_A2 A2
#define BUTTON_D8 8
#define BUTTON_D7 7
#define BRIGHTNESS_PIN A0
#define MAX_BOUNCING_BALLS 4


CRGB leds_11[NUM_LEDS_11];
CRGB leds_10[NUM_LEDS_10];
CRGB leds_9[NUM_LEDS_9];

int task = 0;
int a4PressCount = 0;
int brightness = 120;
int task3SpeedDelay = 50; // Default speed delay for task3


bool a3State = HIGH;
bool a2State = HIGH;
bool d10Blinking = false;
bool d9Blinking = false;
bool dynamicBlinkMode = false;


unsigned long previousMillis = 0;
const long interval = 500;
unsigned long previousBlinkMillis = 0;
const long blinkInterval = 500;
unsigned long CylonSpeedDelay = 50; // Adjust this value as needed
const long dynamicWipeInterval = 75; // Adjust this value for faster wiping
const long dynamicOffInterval = 450; // Duration LEDs stay off after wipe

struct BouncingBall {
  int position;
  int velocity;
  CRGB color;
};

BouncingBall balls[MAX_BOUNCING_BALLS];

CRGB colors[] = {
  CRGB::Red,
  CRGB::Green,
  CRGB::Blue,
  CRGB::Yellow,
  CRGB::Purple,
  CRGB::Cyan,
  CRGB::White,
  CRGB::OrangeRed,
  CRGB::RoyalBlue,
  CRGB::SeaGreen,
  CRGB::SpringGreen,
  CRGB::DeepPink,
  CRGB::DarkOrange,
  CRGB::Chartreuse,
  CRGB::Brown,
  CRGB::Aqua
};
int currentColorIndex = 0;

void setup() {
  pinMode(BUTTON_A5, INPUT_PULLUP);
  pinMode(BUTTON_A4, INPUT_PULLUP);
  pinMode(BUTTON_A3, INPUT_PULLUP);
  pinMode(BUTTON_A2, INPUT_PULLUP);
  pinMode(BUTTON_D8, INPUT_PULLUP);
  pinMode(BUTTON_D7, INPUT_PULLUP);

  pinMode(BRIGHTNESS_PIN, INPUT); //potentiometer

  FastLED.addLeds<WS2812, LED_PIN_11, GRB>(leds_11, NUM_LEDS_11);
  FastLED.addLeds<WS2812, LED_PIN_10, GRB>(leds_10, NUM_LEDS_10);
  FastLED.addLeds<WS2812, LED_PIN_9, GRB>(leds_9, NUM_LEDS_9);

  initializeBouncingBalls(); // Initialize bouncing balls
}

void loop() {

  // Read the potentiometer value and map it to the brightness range (10 to 240)
  int potValue = analogRead(BRIGHTNESS_PIN);
  brightness = map(potValue, 0, 1023, 10, 240);
  // Set the brightness
  FastLED.setBrightness(brightness);
  

  handleTaskSelection();
  switch (task) {
    case 0:
      task0();
      break;
    case 1:
      task1();
      break;
    case 2:
      task2();
      break;
    case 3:
      task3();
      break;
    case 4:
      allOff();
      break;
  }
}

void handleTaskSelection() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(BUTTON_A5);
  
  if (lastButtonState == LOW && currentButtonState == HIGH) {
    task = (task + 1) % 5;
    a4PressCount = 0; // Reset A4 press count when switching tasks
    allOff(); // Turn off all LEDs when switching tasks
  }
  
  lastButtonState = currentButtonState;
}

void task0() {
  static unsigned long lastUpdate = 0;
  static int theaterIndex = 0;
  static int colorIndex = 0;
  static int counter = 0;

  if (millis() - lastUpdate > 150) { // Adjust the interval as needed
    lastUpdate = millis();

    // Clear the strip
    for (int i = 0; i < NUM_LEDS_11; i++) {
      leds_11[i] = CRGB::Black;
    }

    // Set every third LED to the current color
    for (int i = theaterIndex; i < NUM_LEDS_11; i += 3) {
      leds_11[i] = colors[colorIndex];
    }

    theaterIndex = (theaterIndex + 1) % 3; // Move the index

    // Increment the counter and change color every 10 times
    counter++;
    if (counter >= 10) {
      colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
      counter = 0;
    }

    FastLED.show();
  }
}


void task1() {
  if (digitalRead(BUTTON_A3) == LOW) {
    a3State = !a3State;
    if (a3State) {
      d10Blinking = !d10Blinking;
      d9Blinking = false;
    } else {
      d10Blinking = false;
    }
    delay(250); // Debounce delay
  }
  
  if (digitalRead(BUTTON_A2) == LOW) {
    a2State = !a2State;
    if (a2State) {
      d9Blinking = !d9Blinking;
      d10Blinking = false;
    } else {
      d9Blinking = false;
    }
    delay(250); // Debounce delay
  }
  
  if (digitalRead(BUTTON_A4) == LOW) {
    a4PressCount = (a4PressCount + 1) % 4;
    delay(250); // Debounce delay
  }
  
  if (d10Blinking) {
    blinkRL(leds_10, NUM_LEDS_10);
  } else {
    fill_solid(leds_10, NUM_LEDS_10, CRGB::Black);
  }

  if (d9Blinking) {
    blinkRL(leds_9, NUM_LEDS_9);
  } else {
    fill_solid(leds_9, NUM_LEDS_9, CRGB::Black);
  }

  switch (a4PressCount) {
    case 0:
      fill_solid(leds_11, NUM_LEDS_11, CRGB::Red);
      break;
    case 1:
      blink(leds_11, NUM_LEDS_11);
      break;
    case 2:
      Cylon(leds_11, NUM_LEDS_11);
      break;
    case 3:
      fill_solid(leds_11, NUM_LEDS_11, CRGB::Black);
      break;
  }
  
  FastLED.show();
}

void task2() {
  blinkrightandleft(); // Always blink D10 and D9 in task 2

  if (digitalRead(BUTTON_A3) == LOW) {
    currentColorIndex = (currentColorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
    delay(250); // Debounce delay
  }

  if (digitalRead(BUTTON_A2) == LOW) {
    currentColorIndex = (currentColorIndex - 1 + (sizeof(colors) / sizeof(colors[0]))) % (sizeof(colors) / sizeof(colors[0]));
    delay(250); // Debounce delay
  }

  if (digitalRead(BUTTON_A4) == LOW) {
    dynamicBlinkMode = !dynamicBlinkMode;
    delay(250); // Debounce delay
  }
}

void task3() {
  static int effectIndex = 0;
  static unsigned long lastEffectChange = 0;
  const unsigned long effectChangeInterval = 250; // Debounce interval

  if (digitalRead(BUTTON_A4) == LOW && millis() - lastEffectChange > effectChangeInterval) {
    effectIndex = (effectIndex + 1) % 8; // Change effect
    lastEffectChange = millis();
  }

  if (digitalRead(BUTTON_A3) == LOW && millis() - lastEffectChange > effectChangeInterval) {
    currentColorIndex = (currentColorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
    lastEffectChange = millis();
  }

  if (digitalRead(BUTTON_A2) == LOW && millis() - lastEffectChange > effectChangeInterval) {
    currentColorIndex = (currentColorIndex - 1 + (sizeof(colors) / sizeof(colors[0]))) % (sizeof(colors) / sizeof(colors[0]));
    lastEffectChange = millis();
  }

  switch (effectIndex) {
    case 0:
      KITT(leds_11, NUM_LEDS_11);
      break;
    case 1:
      Sparkle(leds_11, NUM_LEDS_11);
      break;
    case 2:
      RunningLights(leds_11, NUM_LEDS_11);
      break;
    case 3:
      ColorWipe(leds_11, NUM_LEDS_11);
      break;
    case 4:
      RainbowCycle(leds_11, NUM_LEDS_11);
      break;
    case 5:
      TheaterChase(leds_11, NUM_LEDS_11);
      break;
    case 6:
      BouncingBalls(leds_11, NUM_LEDS_11);
      break;
    case 7:
      ColorLoop(leds_11, NUM_LEDS_11);
      break;
  }

  FastLED.show();
}


void blinkrightandleft() {
  static unsigned long lastUpdate = 0;
  static int ledIndex = 0;
  static bool wiping = false;
  static bool allOn = false;
  static bool turnOff = false;
  static unsigned long offStartTime = 0;

  if (dynamicBlinkMode) {
    if (allOn && !turnOff && millis() - lastUpdate >= dynamicWipeInterval) {
      lastUpdate = millis();
      fill_solid(leds_10, NUM_LEDS_10, CRGB::Black);
      fill_solid(leds_9, NUM_LEDS_9, CRGB::Black);
      FastLED.show();
      turnOff = true;
      offStartTime = millis();
    } else if (turnOff && millis() - offStartTime >= dynamicOffInterval) {
      lastUpdate = millis();
      ledIndex = 0;
      allOn = false;
      turnOff = false;
    } else if (millis() - lastUpdate >= dynamicWipeInterval) {
      lastUpdate = millis();

      if (!allOn) {
        // Dynamic mode: wipe effect
        fill_solid(leds_10, NUM_LEDS_10, CRGB::Black);
        fill_solid(leds_9, NUM_LEDS_9, CRGB::Black);

        for (int i = 0; i <= ledIndex; i++) {
          leds_10[i] = colors[currentColorIndex];
          leds_9[i] = colors[currentColorIndex];
        }

        ledIndex++;

        if (ledIndex >= NUM_LEDS_10) {
          allOn = true;
        }

        FastLED.show();
      }
    }
  } else {
    if (millis() - lastUpdate >= blinkInterval) {
      lastUpdate = millis();
      
      // Static mode: all LEDs blink
      for (int i = 0; i < NUM_LEDS_10; i++) {
        leds_10[i] = leds_10[i] ? CRGB::Black : colors[currentColorIndex];
      }
      for (int i = 0; i < NUM_LEDS_9; i++) {
        leds_9[i] = leds_9[i] ? CRGB::Black : colors[currentColorIndex];
      }
      
      FastLED.show();
    }
  }
}





void blink(CRGB* leds, int numLeds) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    for (int i = 0; i < numLeds; i++) {
      leds[i] = leds[i] ? CRGB::Black : CRGB::Red;
    }
  }
}

void blinkRL(CRGB* leds, int numLeds) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    for (int i = 0; i < numLeds; i++) {
      leds[i] = leds[i] ? CRGB::Black : CRGB::Orange;
    }
    FastLED.show();
  }
}

void toggle(CRGB* leds, int numLeds) {
  static int pos = 0;
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    fill_solid(leds, numLeds, CRGB::Black);
    leds[pos] = CRGB::Red;
    pos = (pos + 1) % numLeds;
  }
}

void Cylon(CRGB* leds, int numLeds) {
  static unsigned long lastUpdate = 0;
  static int pos = 0;
  static int direction = 1;

  if (millis() - lastUpdate > CylonSpeedDelay) {
    lastUpdate = millis();

    // Clear the strip
    fill_solid(leds, numLeds, CRGB::Black);

    // Light up the 3 LEDs around the current position
    for (int i = 0; i < 3; i++) {
      int ledPos = (pos + i) % numLeds;
      leds[ledPos] = CRGB::Red;
    }

    // Update the position
    pos += direction;
    if (pos == 0 || pos == numLeds - 3) {
      direction = -direction; // Reverse direction at the ends
    }

    FastLED.show();
  }
}

//task3()
void KITT(CRGB* leds, int numLeds) {
  static int position = 0;
  static int direction = 1;
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate >= task3SpeedDelay) {
    lastUpdate = millis();

    fill_solid(leds, numLeds, CRGB::Black);
    leds[position] = colors[currentColorIndex];

    position += direction;
    if (position == 0 || position == numLeds - 1) {
      direction *= -1;
    }

    FastLED.show();
  }
}

//task3()
void Sparkle(CRGB* leds, int numLeds) {
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate >= task3SpeedDelay) {
    lastUpdate = millis();

    fill_solid(leds, numLeds, CRGB::Black);
    int pos = random16(numLeds);
    leds[pos] = colors[currentColorIndex];
    FastLED.show();
  }
}
//task3()
void RunningLights(CRGB* leds, int numLeds) {
  static int position = 0;
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate >= task3SpeedDelay) {
    lastUpdate = millis();

    // Number of groups and group size
    int groupSize = 4;

    // Clear all LEDs
    fill_solid(leds, numLeds, CRGB::Black);

    // Loop through each LED, turning on a group of 4 at the current position
    for (int i = 0; i < groupSize; i++) {
      int ledIndex = (position + i) % numLeds;
      leds[ledIndex] = colors[currentColorIndex];
    }

    position++;
    FastLED.show();
  }
}

//task3()
void ColorWipe(CRGB* leds, int numLeds) {
  static int position = 0;
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate >= task3SpeedDelay) {
    lastUpdate = millis();

    if (position < numLeds) {
      leds[position] = colors[currentColorIndex];
      position++;
    } else {
      fill_solid(leds, numLeds, CRGB::Black);
      position = 0;
    }

    FastLED.show();
  }
}

//task3()
void RainbowCycle(CRGB* leds, int numLeds) {
  static int startIndex = 0;
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate >= task3SpeedDelay) {
    lastUpdate = millis();
    startIndex++;
    for (int i = 0; i < numLeds; i++) {
      leds[i] = ColorFromPalette(RainbowColors_p, startIndex + i * 2, 255, LINEARBLEND);
    }
    FastLED.show();
  }
}

//task3()
void TheaterChase(CRGB* leds, int numLeds) {
  static int position = 0;
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate >= task3SpeedDelay) {
    lastUpdate = millis();
    for (int i = 0; i < numLeds; i++) {
      if ((i + position) % 3 == 0) {
        leds[i] = colors[currentColorIndex];
      } else {
        leds[i] = CRGB::Black;
      }
    }
    FastLED.show();
    position++;
    if (position >= 3) position = 0;
  }
}



void BouncingBalls(CRGB* leds, int numLeds) {
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate >= task3SpeedDelay) {
    lastUpdate = millis();
    for (int i = 0; i < MAX_BOUNCING_BALLS; i++) {
      leds[balls[i].position] = CRGB::Black;
      balls[i].position += balls[i].velocity;
      if (balls[i].position < 0) {
        balls[i].position = 0;
        balls[i].velocity = -balls[i].velocity;
      } else if (balls[i].position >= numLeds) {
        balls[i].position = numLeds - 1;
        balls[i].velocity = -balls[i].velocity;
      }
      leds[balls[i].position] = balls[i].color;
    }
    FastLED.show();
  }
}

void initializeBouncingBalls() {
  for (int i = 0; i < MAX_BOUNCING_BALLS; i++) {
    balls[i].position = random(NUM_LEDS_11);
    balls[i].velocity = random(1, 3);
    balls[i].color = colors[random(sizeof(colors) / sizeof(colors[0]))];
  }
}

//task3()
void ColorLoop(CRGB* leds, int numLeds) {
  static int startIndex = 0;
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate >= task3SpeedDelay) {
    lastUpdate = millis();
    for (int i = 0; i < numLeds; i++) {
      leds[i] = colors[(startIndex + i) % (sizeof(colors) / sizeof(colors[0]))];
    }
    startIndex++;
    FastLED.show();
  }
}


void allOff() {
  fill_solid(leds_11, NUM_LEDS_11, CRGB::Black);
  fill_solid(leds_10, NUM_LEDS_10, CRGB::Black);
  fill_solid(leds_9, NUM_LEDS_9, CRGB::Black);
  FastLED.show();
}
