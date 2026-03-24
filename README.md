# LVGL Soldering Station UI Simulation

This project is an **embedded-style graphical user interface simulation** of a **2-in-1 soldering station**, developed using **LVGL** and the **SDL simulator**.

It demonstrates how a real embedded device can provide **interactive control, real-time feedback, and parameter management** through a modern UI.

---

## 🚀 Features

- Arc-based temperature control (knob-like interaction)
- Smooth animated temperature transitions
- Real-time temperature visualization
- Dynamic color feedback:
  - 🔴 Heating (temperature increasing)
  - 🔵 Cooling (temperature decreasing)
  - 🟢 Target reached
- Editable system parameters:
  - Start temperature
  - Standby temperature
  - Standby time
  - Sleep mode time
- Input validation (only valid numeric ranges allowed)
- Reboot/reset functionality
- Multi-screen UI navigation

---

## 🧠 System Behavior

The system simulates a **real soldering station control interface**:

- The user sets a **target temperature** using an arc component
- The system gradually updates the current temperature using a **timer-based animation**
- The current value is displayed in a **digital box**
- The background color of the box changes dynamically:
  - Red → temperature rising
  - Blue → temperature falling
  - Green → target reached

This allows the user to clearly observe:
- where the temperature starts
- where it is going
- how it changes over time

---

## 🖥️ Screens Overview

### 1. Main Menu
- Havya (Soldering Iron)
- Soğutucu (Cooling Unit)

### 2. Havya Screen
- Arc-based temperature adjustment
- Digital temperature display
- Real-time animated feedback

### 3. Extra Settings Screen
- Editable parameters using text input fields
- Keyboard interaction
- Value validation
- Reset (reboot) button

### 4. Cooling Screen
- Placeholder for future functionality

---

## 📸 Simulation Outputs

### 🔹 Main Screens

![Main Menu](sim_output/01_main_menu.png.png)  
![Havya Initial](sim_output/02_havya_initial.png.png)  
![Settings Initial](sim_output/03_havya_settings_initial.png.png)  
![Cooling Screen](sim_output/10_sogutucu_screen.png.png)  

---

### 🔹 Parameter Changes

![Settings Updated](sim_output/04_havya_settings_updated.png.png)  
![Start Temperature Updated](sim_output/05_havya_start_temp_updated.png.png)  

---

### 🔹 Temperature Transitions

![Heating](sim_output/06_havya_heating.png.png)  
![Cooling](sim_output/07_havya_cooling.png.png)  
![Target Reached](sim_output/08_havya_target_reached.png.png)  

---

### 🔹 System Reset

![After Reboot](sim_output/09_after_reboot.png.png) 

---

## ⚙️ Technologies Used

- C Programming Language
- LVGL (Light and Versatile Graphics Library)
- SDL (Simulation Layer)
- WSL (Windows Subsystem for Linux)
- Visual Studio Code

---

## 🛠️ Development Environment

This project was developed using:

- **VS Code** as the main editor
- **WSL terminal** for building and running the project
- LVGL + SDL for UI simulation

---

## ▶️ How to Run

Make sure you have **LVGL and SDL properly installed** on your system before running the project.

Then use the following commands:


cd /mnt/c/Users/LENOVO/lv_sim_eclipse_sdl/build

make -j8

../bin/main


>⚠️ Note:
  The path (/mnt/c/Users/LENOVO/...) may differ depending on your system.
  You should update the path according to where your project is located.

---

🎯 Purpose

This project aims to demonstrate:

- Embedded UI design principles
- Event-driven programming
- Real-time system feedback
- User interaction handling
- Parameter validation and control

---

🧩 Future Improvements

- Simulate real sensor data
- Implement cooling system behavior
- Improve UI design and animations
