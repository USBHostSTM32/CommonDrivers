# CommonDrivers

## Overview

This repository contains the firmware for the Drive-by-Wire (DBW) system of the Pix Moving vehicle. The system is designed to handle the core functionalities of drive-by-wire, processing input data from the Thrustmaster T818 steering wheel via USB, applying force feedback commands, and managing CAN bus communication, which is necessary for controlling vehicle actuators and obtaining feedback from the vehicle.

## Authors

- Alessio Guarini
- Antonio Vitale

## Modules (Under Construction)

### dbw_kernel.h

The `dbw_kernel.h` file contains type definitions and function prototypes for the DBW Kernel module. This module is responsible for the core functionalities of the drive-by-wire system, integrating various modules to ensure safe vehicle operation. It manages input data from the steering wheel, applies force feedback commands, and handles CAN bus communication.

### rotation_manager.h

The `rotation_manager.h` file handles the control of steering wheel force feedback. It includes definitions and function prototypes for the rotation manager, enabling force feedback control and position control using a PID controller.

### pid_regulator.h

The `pid_regulator.h` file defines the PID regulator, including parameters and functions for initializing the regulator, computing the output, and modifying the regulator parameters. This module is crucial for precise control of steering wheel force and position.

### urb_sender.h

The `urb_sender.h` file manages USB requests. It defines the URB Sender module, responsible for handling USB communications and sending interrupt packets. This module ensures correct and reliable communication via USB.

### t818_ff_manager.h

The `t818_ff_manager.h` file is dedicated to managing force feedback for the T818 steering wheel. It includes functions to initialize and control force feedback effects such as spring, constant force, and gain. The module communicates with the device via USB to manage these effects.

### button.h

The `button.h` file handles button states and interactions. It defines structures and functions for detecting button presses, including long and short presses, and updating button states based on received inputs.

### auto_control.h

The `auto_control.h` file contains the interface for the automatic control module, which generates logical values to be transmitted on the CAN bus. It manages the vehicle's state, including gears (PARKING, REVERSE, NEUTRAL, DRIVE), and updates the state based on input commands and internal logic.

### auto_data_feedback.h

The `auto_data_feedback.h` file defines the automatic data feedback module. This module collects and provides feedback data related to speed, steering, braking, and other vehicle parameters, ensuring precise and responsive vehicle control.

### can_parser.h

The `can_parser.h` file handles the translation of logical values from the automatic control module into a CAN frame payload. It includes definitions and function prototypes for converting automatic control data into a format compatible with the CAN bus.

### can_manager.h

The `can_manager.h` file manages CAN message initialization and transmission. It includes configuration of transmission parameters, ensuring that CAN messages are sent correctly and reliably.

### common_drivers.h

The `common_drivers.h` file contains definitions and prototypes for common utility functions used across various modules, such as mathematical operations and value transformations. It provides essential support functionalities for the proper operation of the system.

### t818_drive_control.h

The `t818_drive_control.h` file handles the drive commands for the T818 steering wheel, integrating the logic needed to interpret input signals and convert these signals into appropriate drive actions.

### usbh_hid_parser.h, usbh_hid_t818.h, usbh_hid.h

These files are responsible for managing the USB HID interface. They define the structures and functions necessary to interact with HID devices via USB, ensuring proper communication between the firmware and the T818 steering wheel.

### delayus.h

under construction
