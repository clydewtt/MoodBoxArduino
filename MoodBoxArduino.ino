#include <FastLED.h>

#define LED_PIN     2
#define NUM_LEDS    60

int delayVal = 40; // timing delay in milliseconds inbetween color alternations

int redColor = 0;
int greenColor = 0;
int blueColor = 0;
String inputMood = "whatever"; // Default mood is happy.

// Maybe add a fade pattern mode (leds fade in and out in between color changes

// PatternModes: 1 = Linear, 2 = Random, 3 = Reverse, 4 = Fade
int patternMode = 4;

CRGB leds[NUM_LEDS];
int indices[NUM_LEDS];

void setup() {
  Serial.begin(9600);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  // Make array of all the indices of the led strip to later randomize the led strip order
  for (int i = 0; i < NUM_LEDS; i++) {
    indices[i] = i;
    }
}

void loop() {
  // Checks for transmitted data
  if (Serial.available() > 0) {
    // Save the user input in a variable.
    String userInput = Serial.readString();
    userInput.trim();

    // Differienates between a number and string user input
    if (userInput.toInt() >= 1 && userInput.toInt() <= 4) {
      String patternModeName;
      patternMode = userInput.toInt();

      if (patternMode == 1) {
        patternModeName = "Linear";
      } else if (patternMode == 2) {
        patternModeName = "Random";
      } else if (patternMode == 3) {
        patternModeName = "Reverse";
      } else if (patternMode = 4) {
        patternModeName = "Fade";
        }
      Serial.println("Pattern mode chosen: " + patternModeName);
    } 
    // If the user's input was a string
    else {
      inputMood = userInput;
//      inputMood.trim();
      Serial.println("Mood chosen: " + inputMood);
//      Serial.println("Found" + inputMood.find("\n");

    }
  } else {
    // In this case the user would not have chosen a mood.
    Serial.println("No mood chosen, repeating last choice");
  }

  // Depending on the user mood, alternate between different colors.
  if (inputMood == "happy") {
    int i = 0;
    while (i < 3) {
      // Choose a happy color
      if (i == 0) {
        // Make color green
        redColor = 0;
        greenColor = 128;
        blueColor = 0;
      } else if (i == 1) {
        // Make color yellow
        redColor = 255;
        greenColor = 255;
        blueColor = 0;
      } else if (i == 2) {
        // Make color blue
        redColor = 0;
        greenColor = 150;
        blueColor = 255;
      }

      // Change the pixels' colors
      alternatePixels();

      i++; // increment the index

    }

  } else if (inputMood == "alright") {
    // Choose an alright color
    int i = 0;
    while (i < 2) {
      if (i == 0) {
        // Make color gray
        redColor = 192;
        greenColor = 192;
        blueColor = 192;
      } else if (i == 1) {
        // Make color navy
        redColor = 0;
        greenColor = 0;
        blueColor = 128;
      }

      // Change the pixels' colors
      alternatePixels();

      i++; // increment the index
    }

  } else if (inputMood == "sad") {
    // Choose a sad color
    int i = 0;
    while (i < 2) {
      if (i == 0) {
        // Make color purple
        redColor = 128;
        greenColor = 0;
        blueColor = 128;
      } else if (i == 1) {
        // Make color orange
        redColor = 255;
        greenColor = 165;
        blueColor = 0;
      }

      // Change the pixels' colors
      alternatePixels();

      i++; // increment the index
    }

  } else if (inputMood == "mad") {
    // Choose a mad color
    int i = 0;
    while (i < 2) {
      if (i == 0) {
        // Make color red
        redColor = 255;
        greenColor = 0;
        blueColor = 0;
      } else if (i == 1) {
        // Make color brown
        redColor = 59;
        greenColor = 30;
        blueColor = 0;
      }

      // Change the pixels' colors
      alternatePixels();

      i++; // increment the index
    }
  } else if (inputMood == "whatever") {
    redColor = rand() % 256;
    greenColor = rand() % 256;
    blueColor = rand() % 256;

    alternatePixels();
    }
}

// Alternates the pixels' color depending on the mood and pattern chosen.
void alternatePixels()
{

  
  // 1 = Linear pattern
  if (patternMode == 1) {
    FastLED.setBrightness(255);
    FastLED.show();
    
    for (int i = 0; i < NUM_LEDS; i++) {
      updatePixel(i);
    }
  }

  // 2 = Random pattern
  else if (patternMode == 2) {
    FastLED.setBrightness(255);
    FastLED.show();
    
    // Shuffles the order of the indexes which will make pixels
    // update in a random order.
    shuffleArray();

    for (int i = 0; i < NUM_LEDS; i++) {
      updatePixel(indices[i]);
    }
  }

  // 3 = Reverse pattern
  else if (patternMode == 3) {
    FastLED.setBrightness(255);
    FastLED.show();
    
    for (int i = NUM_LEDS - 1; i > -1; i--) {
      updatePixel(i);
    }
  }

  // 4 = Fade pattern
  else {
      FastLED.setBrightness(0);
      FastLED.show();
    
    // Set color on all leds first
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(redColor, greenColor, blueColor);
      FastLED.show();
      }

    // Gradually brighten the leds to 255
    for (int brightnessValue = 0; brightnessValue <= 255; brightnessValue++) {
      FastLED.setBrightness(brightnessValue);
      FastLED.show();
      delay(5); // Short pause to show the transition of the led's brightness
    }

    // Show leds at max brightness for half a second
    delay(500);

    // Gradually dim the leds to 0
    for (int brightnessValue = 255; brightnessValue >= 0; brightnessValue--) {
      FastLED.setBrightness(brightnessValue);
      FastLED.show();
      delay(5); // Short pause to show the transition of the led's brightness
    }

    // Short pause before moving on to the next color
    delay(100);
  }
}

void updatePixel(int index)
{
  leds[index] = CRGB(redColor, greenColor, blueColor);
  FastLED.show();

  // Short pause inbetween color changes.
  delay(delayVal);
}

// Implement Fisher-Yates algorithm to shuffle array
void shuffleArray()
{
  int last = 0;
  int temp = indices[last];
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int index = random(NUM_LEDS);
    indices[last] = indices[index];
    last = index;
  }
  indices[last] = temp;
}
