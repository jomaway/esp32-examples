# ESP32 Tutorial

![PlatformIO](https://img.shields.io/badge/platformio-ready-orange?logo=platformio)
![Wokwi](https://img.shields.io/badge/wokwi-supported-green?logo=wokwi)
![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)

Welcome to the ESP32 Tutorial repository! This collection of small, self-contained examples is designed to introduce and demonstrate fundamental principles of embedded systems using the ESP32 microcontroller.

Aimed at beginners, these tutorials use the familiar **Arduino framework**, making it easy to get started with microcontroller programming. Each example is straightforward and focuses on a specific concept or functionality.

You can build and upload the examples to an ESP32 development board using the **PlatformIO extension** for VS Code. For those without hardware, the projects can also be simulated using the **Wokwi extension** within VS Code.

## 🚀 Setup Instructions

Getting started is quick and easy! This project includes a preconfigured **VS Code workspace** to streamline the setup. Just follow the steps below:

### 1. **Install VS Code**

If you haven’t already, download and install [Visual Studio Code](https://code.visualstudio.com/).

### 2. **Install Required Extensions**

Make sure the following extensions are installed in VS Code:

- [**PlatformIO IDE**](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) – for building and uploading code to your ESP32.
- [**Wokwi for VS Code**](https://marketplace.visualstudio.com/items?itemName=Wokwi.wokwi-vscode) – for simulating projects without hardware.

> **Tip:** After opening the workspace, VS Code may prompt you to install any missing extensions.

### 3. **Open the Workspace**

1. Clone or download this repository.
2. Open the included `esp32-tutorial.code-workspace` file in VS Code.

```bash
code esp32-tutorial.code-workspace
```

### 4. Run the Examples

- To *build and upload* to a physical ESP32 board, use the PlatformIO controls in the status bar or sidebar.
- To *simulate with wokwi* (requires account), _build_ the project and start the **Wokwi Simulator** by opening the `diagram.json` and click the play button. 


## Learning Path

The examples are not strictly at a fixed order but if you want to go through them step by step, you can follow this learning path.

1. GPIOs and Serial Output

  - [Blink an LED](/esp32-led-blink)
  - [Controll multiple LEDs at once](/esp32-led-array)
  - [Simple print debugging](/esp32-serial-debug)
  - [Press a button](/esp32-btn-press)
  - [Toggle a button](/esp32-btn-toggle)

2. Non Blocking GPIO usage

  - [Blink LED with `millis()`](/esp32-led-blink-millis)
  - [Toggle a button with `millis()`](/esp32-btn-toggle-millis)
  - [Toggle a button with a shift register](/esp32-btn-toggle-shift)

3. ADC and PWM

  - [Read a Poti](/esp32-adc-poti)
  - [Fade an LED](/esp32-led-fade)
  - [Controll a Servo with](/esp32-servo)


## 📄 License

This project is licensed under the MIT License.
