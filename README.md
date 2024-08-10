# Arduino-Rear-Light-for-Bike-LED-Effects-Project

This project is an Arduino-based rear light system for bikes, featuring dynamic LED effects controlled by push buttons. The system is designed using WS2812B LED strips and the FastLED library, providing various lighting effects that can enhance visibility and add a cool factor to your bike.

# Features
Task Selection with A5 Button: Easily switch between different lighting modes using the A5 button. Each task offers a unique LED effect tailored for bike rear lighting.

# Dynamic Lighting Effects:

Theater Marquee Chasing Lights: A multi-color loop effect with colors changing after every 10 cycles, creating an eye-catching rear light display.
KITT Scanner: A distinctive LED pattern that follows a sequence including RightToLeft, LeftToRight, OutsideToCenter, and CenterToOutside, mimicking the famous KITT car effect.
Running Lights: Groups of LEDs chase each other, reminiscent of holiday lights, providing a vibrant and dynamic effect.
Additional Effects: Sparkle, Rainbow Cycle, Theater Chase, Bouncing Balls, and Color Loop effects offer various options to keep your bike's rear light stylish and noticeable.
Brightness Control: Adjust the brightness of all LED strips (D11, D10, and D9) using a potentiometer connected to A0, with a range from 10 to 240, ensuring visibility in different lighting conditions.

Non-Blocking Timing: All effects use a non-blocking timing approach to avoid using the delay() function, ensuring smooth transitions between effects and responsive button controls.

# Hardware Requirements
Arduino Uno
WS2812B LED Strips
Push Buttons: Connected to pins A5, A4, A3, A2, D8, and D7.
Potentiometer: Connected to A0 for brightness control.
Software Requirements
Arduino IDE
FastLED Library
Getting Started
Clone the Repository:

# bash
Copy code
git clone https://github.com/your-username/arduino-bike-rear-light.git

# Open the Project:
Open the Arduino Rear Light (for Bike) LED Effects Project.ino file in Arduino IDE.

# Upload the Code:
Connect your Arduino to your computer and upload the code.

# Enjoy the Effects:
Mount the LED strips on your bike, and use the buttons to switch between tasks and display dynamic rear light effects.

# How It Works
Task 0: Theater Marquee Chasing Lights with a color loop that changes every 10 cycles.
Task 1: Control blinking effects on D10 and D9 with the A3 and A2 buttons. A4 cycles through different LED states on D11.
Task 2: Simultaneous blinking of D10 and D9, with A3 and A2 changing colors. A4 switches between static and dynamic blink modes.
Task 3: Cycle through multiple effects like KITT, Sparkle, Running Lights, and more with A4. Change colors using A3 and A2.

# Future Improvements
Add more effects and fine-tune the existing ones.
Explore power-saving options for extended battery life.
Contributions
Contributions are welcome! Feel free to open issues or submit pull requests with improvements or new features.

# Acknowledgments
Thanks to the creators of the FastLED library for making such a powerful tool for LED control.
Inspiration from various online Arduino projects and bike safety initiatives.
