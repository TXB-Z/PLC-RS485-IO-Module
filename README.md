# PLC-RS485-IO-Module
Design and development of a 4DI/4DO PLC I/O expansion module using STM32 and Modbus RTU over RS485.

## 1. Project Overview

This project focuses on the design and development of a digital I/O expansion module for PLC systems using RS485 communication.

The module is designed to provide additional digital input and output channels and communicate with a host device through the Modbus RTU protocol.

### Main Features
- 4 Digital Inputs (DI)
- 4 Digital Outputs (DO)
- STM32 microcontroller-based control system
- RS485 serial communication
- Modbus RTU protocol
- 24V DC power input

## 2. System Architecture

The system consists of the following main functional blocks:

- Power Supply
- STM32 Microcontroller
- Digital Input Interface
- Digital Output Interface
- RS485 Communication Interface

## 3. Hardware Design

### 3.1 Microcontroller
- STM32 microcontroller
- GPIO-based digital I/O control
- UART communication interface

### 3.2 Digital Input
- 4-channel digital input interface
- Optocoupler-based signal isolation
- Designed for 24V industrial signals

### 3.3 Digital Output
- 4-channel relay output
- Transistor driver circuit
- 24V DC relay coils

### 3.4 RS485 Interface
- RS485 transceiver
- Half-duplex communication
- UART-based data transmission

### 3.5 Power Supply
- 24V DC input
- Buck converter for 5V supply
- 3.3V supply for microcontroller


## 4. Firmware Development

The firmware is developed using STM32 HAL.

Main functions:
- Digital input acquisition
- Digital output control
- UART communication
- Modbus RTU data processing

## 5. Communication Testing

The communication interface is tested using:
- USB-to-RS485 converter
- PC-based serial communication software
- Modbus Poll

## 6. Development Status

- Hardware design: In development
- Firmware development: In development
- RS485 communication: Under testing
- Modbus RTU integration: Under testing
- Full system validation: Pending

## 7. Future Improvements

- Complete hardware validation
- Improve communication reliability
- Test with an actual PLC
- Develop additional monitoring functions

## 8. Tools & Technologies

- STM32CubeMX
- Keil MDK
- KiCad / Altium Designer
- Modbus RTU
- RS485
- Modbus Poll
