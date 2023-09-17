# Arduino Railroad Simulation with FreeRTOS and FastLED

This project is an Arduino-based railroad simulation that uses the FreeRTOS library for multitasking and the FastLED library for controlling an 8x8 WS2812B LED matrix. The simulation allows users to select a station and view the estimated time until the next train's arrival.

## Table of Contents

- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Wokwi Simulation](#testing-the-system-in-wokwi-simulation)
- [Project Structure](#project-structure)
- [Customization](#customization)
- [Contributing](#contributing)
- [License](#license)

## Hardware Requirements

To run this simulation, you will need the following hardware components:

- Arduino Uno R3 or compatible board
- WS2812B 8x8 LED matrix
- Appropriate power supply for the LED matrix
- Jumper wires for connecting the LED matrix to the Arduino

## Software Requirements

Before you begin, ensure you have the following software tools installed:

- Arduino IDE or VSCode with the Arduino extension (for code editing)
- arduino-cli (for building and flashing the code)
- FreeRTOS library for Arduino (installable via Arduino Library Manager)
- FastLED library for Arduino (installable via Arduino Library Manager)

## Installation

1. Clone this repository to your local machine:
```bash
git clone https://github.com/DoidoRK/railroad.git
``` 

2. Open the Arduino IDE or VSCode with the Arduino extension and navigate to the project directory.

3. Make sure you have the FreeRTOS and FastLED libraries installed using the Arduino Library Manager. If not, you can install them from the Arduino IDE's Library Manager.

## Usage

1. Connect your Arduino Uno R3 to the WS2812B LED matrix using the appropriate wiring. Refer to the documentation for your LED matrix for wiring details.

2. In the Arduino IDE or VSCode, select the correct board (Arduino Uno R3 or compatible) and port for your Arduino.

3. Open the `railroad.ino` file and upload it to your Arduino board using arduino-cli or the Arduino IDE.

4. Once the code is uploaded, the simulation will start running on the LED matrix. You will see trains moving between stations.

5. Use the buttons or input method of your choice (e.g., a keypad or serial monitor) to allow the user to select a station and view the time until the next train's arrival.

## Testing the System in Wokwi Simulation

You can test the Arduino railroad simulation system within a virtual environment using [Wokwi](https://wokwi.com/projects/375861346897523713), an online platform for simulating and testing Arduino projects. This allows you to evaluate the project's functionality without physical hardware. Follow these steps to simulate the system:

1. Visit the [Wokwi website](https://wokwi.com/projects/375861346897523713) and create an account if you don't have one.

2. Click the "Run" button to start the simulation. You'll see the LED matrix display the railroad simulation, just as it would on physical hardware.

3. You can interact with the simulation by using the virtual serial monitor or other input methods as specified in your code.

4. Test different scenarios and configurations to ensure the system behaves as expected.

5. Once you are satisfied with the simulation, you can also share the Wokwi circuit with others for collaborative testing and debugging.

Using Wokwi for simulation provides a convenient way to verify and validate your project's functionality before deploying it to physical hardware.

## Project Structure

The project structure is organized as follows:

- `railroad.ino`: The main Arduino sketch file containing the simulation logic.
- `config.h`: Configuration file for customizing simulation parameters.
- `tasks.h` and `tasks.cpp`: Files for defining and managing FreeRTOS tasks.
- `led_matrix.h` and `led_matrix.cpp`: Files for controlling the WS2812B LED matrix using FastLED.
- `README.md`: This README file.

## Customization

You can customize various aspects of the simulation by editing the `config.h` file. This file contains configuration options for train routes, station names, LED matrix pin assignments, and more.

## Contributing

Contributions to this project are welcome. If you have ideas for improvements or new features, please open an issue or create a pull request. For major changes, please discuss them in the issues section before implementing.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.