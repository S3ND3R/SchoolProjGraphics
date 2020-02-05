/**
    CSci-4611 Assignment #1 Text Rain
**/


import processing.video.*;

// Global variables for handling video data and the input selection screen
String[] cameras;
Capture cam;
Movie mov;
PImage inputImage;
boolean inputMethodSelected = false;

// Global variables for implenting falling text
String poem = "a drop fell on the apple tree another on the roof a half a dozen kissed the eaves and made the gables laugh";
int numDrops = 10;
Letter[] letters = new Letter[numDrops];
PFont f;
int lastDrop;
final int MAX_LETTERS = 50;
final int RAIN_DUR = 2000;
final int VELOCITY = 10;



//Global variable for implementing text object interaction
int threshold = 130;
final float MAX_THRESH = 250;

//debug toggle
int debug_mode = 0;
int startTime;
final int DISPLAY_DUR = 2000;


void setup() {
  size(1280, 720);  
  inputImage = createImage(width, height, RGB);
  f = loadFont("DejaVuSansMono-Bold-20.vlw");
  initializeLetters(poem);
  
}


void draw() {
  // When the program first starts, draw a menu of different options for which camera to use for input
  // The input method is selected by pressing a key 0-9 on the keyboard
  if (!inputMethodSelected) {
    cameras = Capture.list();
    int y=40;
    text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
    y += 40; 
    for (int i = 0; i < min(9,cameras.length); i++) {
      text(i+1 + ": " + cameras[i], 20, y);
      y += 40;
    }
    return;
  }


  // This part of the draw loop gets called after the input selection screen, during normal execution of the program.

  
  // STEP 1.  Load an image, either from a movie file or from a live camera feed. Store the result in the inputImage variable
  
  if ((cam != null) && (cam.available())) {
    cam.read();
    inputImage.copy(cam, 0,0,cam.width,cam.height, 0,0,inputImage.width,inputImage.height);
  }
  else if ((mov != null) && (mov.available())) {
    mov.read();
    inputImage.copy(mov, 0,0,mov.width,mov.height, 0,0,inputImage.width,inputImage.height);
  }


  // Fill in your code to implement the rest of TextRain here..

  // Tip: This code draws the current input image to the screen
  
  // flip the captured image so that it is mirrored and apply appropriate filter
  pushMatrix();
  translate(inputImage.width,0);
  scale(-1,1);
  if (debug_mode == 1) {
    float threshVal = threshold / MAX_THRESH;
    inputImage.filter(THRESHOLD, threshVal);
  } else {
    inputImage.filter(GRAY);
  }
  image(inputImage,0,0);
  popMatrix();
    
  textFont(f);
  fill(0);
  
  // display the threshold value after the up or down key has been pressed
  if (millis() - startTime < DISPLAY_DUR) {
    text(threshold, 20, 20);
  }
  
  
  addLetter(poem);
  for (int i = 0; i < letters.length; i++) {
    letters[i].update();
    letters[i].drawLetter();
  }
}


// the Letter class used to track letter locations on the screen
class Letter{
  char letter;
  int yPos, xPos, speed;
  
  Letter(char c, int y, int x, int s) {
    letter = c;
    yPos = y;
    xPos = x;
    speed = s;
  }
  
  void display() {
    println("letter (" + letter + "), Y (" + yPos + "), X (" + xPos + ")");
    println();
  }
  
  void update() {
    if (yPos < height - 10) {
      for (int i = 0; i < speed; i++){
        yPos ++;
        color cl = get(xPos, yPos);
        float bright = brightness(cl);
        if (bright > threshold) {
          fill(#39ff14);
        } else {
          fill (#1b03a3);
          while(bright < threshold && yPos > 0) {
            yPos --;
            cl = get(xPos, yPos);
            bright = brightness(cl); 
          }
        }
      }
    } else {
      yPos = 0;
    }
  }
  
  void drawLetter() {
    text(letter, xPos, yPos);
  }
   
} // End Letter

//Initializes the letter array from a passed string
void initializeLetters(String str) {
    for (int i = 0; i < letters.length; i++) {
      char character = str.charAt(int(random(str.length())));
      while (character < 'a' || character > 'z'){
        character = str.charAt(int(random(str.length())));
      }
      int speed = int(random(1,10));
      int xLoc = int(random(width - 5));
      int yLoc = 0;
      letters[i] = new Letter(character, yLoc, xLoc, speed);
    }
  
}

//adds a single random new letter from the passed string
void addLetter(String str) {
  if (numDrops < MAX_LETTERS && millis() - lastDrop > RAIN_DUR) {
    char character = str.charAt(int(random(str.length())));
    while (character < 'a' || character > 'z'){
      character = str.charAt(int(random(str.length())));
    }
    int speed = int(random(1,10));
    int xLoc = int(random(width - 5));
    int yLoc = 0;
    Letter l = new Letter(character, yLoc, xLoc, speed);
    letters = (Letter[])append(letters, l);
    lastDrop = millis();
    numDrops++;
  }
}

void keyPressed() {
  
  if (!inputMethodSelected) {
    // If we haven't yet selected the input method, then check for 0 to 9 keypresses to select from the input menu
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        println("Offline mode selected.");
        mov = new Movie(this, "TextRainInput.mov");
        mov.loop();
        inputMethodSelected = true;
      }
      else if ((input >= 1) && (input <= 9)) {
        println("Camera " + input + " selected.");           
        // The camera can be initialized directly using an element from the array returned by list():
        cam = new Capture(this, cameras[input-1]);
        cam.start();
        inputMethodSelected = true;
      }
    }
    return;
  }


  // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
  // Fill in your code to handle keypresses here..
  
  if (key == CODED) {
    if (keyCode == UP) {
      // up arrow key pressed
      if (threshold < 255) {
        threshold++;
      }
      startTime = millis();
    }
    else if (keyCode == DOWN) {
      // down arrow key pressed
      if (threshold > 0) {
        threshold--;
      }
      startTime = millis();
    }
  }
  else if (key == ' ') {
    // space bar pressed
    if (debug_mode == 0) {
      debug_mode = 1;
    } else {
      debug_mode = 0;
    }
  } 
  
}
