# DER 2024 ECU Firmware v2.1.1

Designed and writen by Cole Bardin (cab572)

Updated: 9/25/2024

## Platform

### Microcontroller

STM32F767ZI, an ARM Cortex-M7 based microcontroller. The MCU is on a custom breakout PCB and is running at SYSCLK max of 216 MHz.

### Environment

The project can be build and flashed using the STM32CubeIDE. It is an Eclipse based IDE with many added features to improve development on STM boards, such as their IOC file type. This IDE also offers good debugging tools.

[IDE Download Link](https://www.st.com/en/development-tools/stm32cubeide.html)

## Firmware

### Architecture

The entire program is embedded within the `app_data_t` data structure found in `app.h`. This encapsulates all variables, structs, driver interfaces, etc for the program. An instance of `app_data_t` named `app` lives inside the `app.c` file, and a pointer to `app` is be passed to tasks and functions to handle data access. 

All data structures are typedef'd to simplify the code. Also the terms 'data structure' and 'type' will be used interchangeable. In C, a data structure can be typedef'd into a custom data type for convenience.

The `app_data_t` type contains highest priority data such as fault flags, throttle and brake percentage, etc. The structure also contains a `board_t` data structure.

The `board_t` type contains representations of physical devices on the ECU board such as the STM32F7, potentiometers, CANBus transceiver, etc. These devices are represented with their own custom typedef'd data structures.

The `stm32f767_t` type contain the interfaces for the STM HAL. It holds all the handles, mutexes, and other data types used to interface with the MCU hardware.

Data structures like the `poten_t` and `canbus_t` are wrappers around the physical devices on the board. Instances of these structs are stored in the `board` struct for conventient and reliable access.

Each RTOS task receives a pointer to the `app_data_t` instance so that they can parse out the required interfaces and data.

For example, the CANBus task would require access to the `canbus_t` instance that was initialized during setup. So it would make sense that the task function declares a local `canbus_t *` that points to the instance stored within the shared `app_data_t` instance.

### File Organization

The base firmware is written in C and uses FreeRTOS middleware. It is a rather standard STM32Cube project layout.

Within the `Core` subdirectory, the `Inc` and `Src` directories contain the header and source code files respectively. These two directories have identical internal structures.

Inside of both `Inc` and `Src`, lives the `ext_drivers` and `tasks` subdirectories. Along with `main.c/h`, `app.c/h`, and `board.c/h` files.

The `ext_drivers` subdirectory holds the custom driver files for the MCU, ICs and other devices on the board.

The `tasks` subdirectory contains a file for each task. A task file includes two functions: One function to create the task. And another function for the task body. The task function should be locally scoped to the source file while the task creation function should be included in the header file.

### Program Entry

Like normal C programs, it begins in the `main()` function in `main.c`. After initializing the STM peripherals, the FreeRTOS kernel is also initialized. Then a function called `app_create()` is called. Lastly, the FreeRTOS kernel is started. 

The main function calls `app_create()` located in `app.c`. This function recursively initializes all the nested data structures within the `app_data_t` instance. Lastly, it creates the FreeRTOS tasks to be run.

Once `app_create()` is finished, the kernel is started. After the `osKernelStart()` function, there is an infinite loop trap. This is because `osKernelStart()` should never return. If it does return for some reason, the infinite loop should catch the control and stop `main()` from returning. This behavior is specific to microcontrollers.

## FreeRTOS Tasks

### APPS Task (Accelerator Pedal Position Sensor)

As defined in the rules, the APPS sytem must read from two electrically isolated sensors to determine the accelerator position. The car uses two linear potentiometers to acoomplish this.

The APPS task reads both of the potentiometers and uses their calibration to calculate their percentages. Then the data is checked for implausibility to comply with secion T.4.2.5 of the rules. In the event of a sustained disagreement between the two sensors, the firmware enters a non-recoverable state. In this error state, the firmware will disable the motor controller, which will allow the motor to free spin, as well as repeatedly sending a 0% torque request to the motor controller. 

If there is no detected error with the APPS data, the task will then populate a CANBus packet with a torque command for the motor controller. The pedal position data is converted to the format specified in the motorcontroller's data sheet. 

Then, the task places the CANBus packet inside a message queue and sends a notification to the CANBus Task, letting it know there is a torque command to send. Lastly, the task delays itself for the appropriate amount of OS ticks such that it operates at the desired frequency.

### BPPC Task (Brake Pedal Plausibility Check)

The BPPC task reads the throttle and brake position data supplied by other tasks. Using thresholds specified by the rules, it determines if the throttle is engaged low (5%) or high (25%). It also determines if the brakes are engaged above a threshold that we consider them "engaged".

If the brakes are engaged and the throttle is above 25%, a BPPC fault has occured. The BPPC fault flag should be set so the ErrorHandle Task can respond appropriately. The BPPC fault is a soft fault, meaning it is recoverable. If there is an active BPPC fault, the throttle must released below 5% to unset the fault.

After handling whether or not to set or unset the BPPC fault flag, the task then delays so that it can run at a desired frequency.

### BSE Task (Brake System Encoder)

The BSE task is very similar to the APPS task. However, it reads the pressure transducers attached to the brake lines instead. The APPS task was given 2 out of 3 of the ADCs on the STM32F7 since accelerator data precision is much more important than the brakes. The BSE task is then left to use the remaining ADC for both sensors, requiring that the ADC channel is changed in between reads.

After acquiring the data, similar safety checks are performed and the average percentage is calculated. The brake light enabling or disabling is then handled for the current state of the brake pedal.

Lastly, the task delays such that it runs at the desired frequency.

### CANBus Task

The CANBus task handles sending CANBus messages from other tasks. In this firmware version, it handles sending torque commands to the motor controller. 

This is an event driven task, meaning it does not delay itself for a predetermined amount of time. Instead, it waits for other tasks to send it a notification that there is something to do.

Upond receiving that notificaiton, it checks where the notificaion value. The task that notifies the CANBus task can set bits in the notificaiton value. For example, the APPS task sets bit 0 and the CANBus receive ISR sets bit 0. The task can check the notificaion value to determine if the CANBus packet in the message queue is a torque command to send to the motor controller or a received message to log.

CANBus packets are placed into a message queue for the CANBus task. Once notified, the task can retreive the packet and act on it accordingly. After processing the CANBus packet, the task awaits another notificaiton.

Currently, no other tasks create and send CANBus messages.

Incomming messages are processed by the `HAL_CAN_RxFifo0MsgPendingCallback()` interrupt in `stm32f7xx_it.c`. Incomming messages are typically from the AMS sending status updates. The data is processed in constant time thanks to the array of pointers called `ams_data_dest` in the interrupt handler. The 8-bytes of the CANBus message are broken into 4 2-byte sections. The first section is the packet identifier, and the other 3 data values. Each identifier correlates to a specific 3 data points. For example, identifier 0 will always contain the AMS State, AIR State, and Output Current. Using this format and array of poiners allows the packets processed in constant time.

### Error Task

The Error task reads error flags set by other tasks and sets larger error states. This allows errors of different processes to be handled in specific ways. Currently there are two types of system faults: hard faults and soft faults. This task also monitors the status of shutdown subsystems and drives the Safety System Arm (SSA) light on the dashboard.

Hard faults are non-recoverable and tasks can change their behavior to maximize safety. In this version of the firmware, the hard faults are APPS, BSE, Coolant Overtemp, and Cascadia Error. If a hard fault is present, the motor controller is disabled. If the hard fault is a coolant overtemp, then the firmware state is set to false. This will trigger shutdown and disconnect HV from the motor controller. The rules do not specify deactivating the tractive system upon APPS and BSE faults. So it was determined that only a coolant fault would require disabling the tractive system. 

Soft faults, on the other hand, are recoverable and do not require that the car be put into a full safety mode. The current soft faults include BPPC, CLI, ACC, CANBus, and Dashboard. Nothing happens on soft faults and they can usually be ignored for critical functionality.

Each iteration of the Error task reads the status of all shutdown subsystems as well as the error signal from the motor controller. This helps update SSA conditions and providing useful information to the driver. The shutdown signals read are BMS, IMD, and BSPD. If the AIRs are open and there are no current faults in the shutdown circuit, then the SSA light is illuminated.

This task also updates the RTC registers.

### RTD Task (Ready To Drive)

The RTD task checks the three safety criteria (defined in EV9.4.2) before allowing the car to be "started". These criteria include: Tractive System Active, Brakes engaged, and RTD button pressed by driver. If all three of these are met, the task will set the RTD flag. This RTD flag allows the APPS task to send torque commands to the motor controller. The RTD task reads the TSAL signal, RTD Button State, and the Cascadia OK State. This task can be broken into two conditions:

If the RTD flag is unset, meaning the car is not RTD, and the conditions are met, the buzzer is enabled for 3 seconds before setting the RTD flag to be true. While the flag is unset, the task delays for only 50ms.

If the RTD flag is set, meaning the car previously went through the RTD sequence, it checks for conditions to unset RTD. While in RTD, if TSAL disables or the RTD switch is released, then the shutdown circuit is tripped and RTD is unset. This is not specified in the rules, but it was determined to be desired behavior that removing the RTD key should disable the car. The shutdown circuit is tripped by setting firmware state to false for 100ms then setting it back to true. This will make the AIRs open and the SSA light enable. Allowing the driver to put the car back by pressing SSA.

### ACC Task (Accelerometer)

The ACC task just utilizes the MPU6050 driver to read all the data values. It updates the `mpu6050_t` data structure. For now, the information is not being used. But it is intended to be recorded to the SD card in later FW versions.

### CLI Task (Command Line Interface)

The CLI Task handles all incomming commands from the user. When connected to the serial port, each character received triggers the `HAL_UART_RxCpltCallback()` interrupt in `stm32f7xx_it.c`. The characters are stored in a buffer. Once a newline has been sent, the interrupt callback notifies the CLI task to process the command. 

Once notified, the CLI task copies the buffer and tokenizes it. Once broken into tokens, the command can be dynamically processed comparing them to the entries in the `command_t` array.

### Coolant Task

The Coolant task reads pressure sensor, flow sensor, and 2 temperature sensors. It then determines if there is an overtemp condition. As well, this task sets the pump PWM duty cycle. In the current firmware version, it does not dynamically change the pump duty cycle since the coolant sensors have yet to be calibrated.

### Dashboard Taks

The Dashboard task sends data from the ECU to the Dashboard Raspberry Pi via UART. The current version of the firmware only sends data for throttle, brake, RTD, BMS fail, IMD fail, and ECU fault.

