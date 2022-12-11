# miroLED-dice
It is ultralow power consumption electronic LED dice based on the attiny mcu and programmed in arduino.
Battery: CR2032

It comes with 4 operation modes:
- 1 random number, smooth deceleration effect
- 1 random number, flashing slow down effect
- 2 random numbers, smooth deceleration effect
- 2 random numbers, flashing slow down effect

Mode is selected easily with this procedure:
1. turn off dice
2. push the roll button and turn on dice
3. the number of lit LEDs shows the active mode
4. release the push button on the desired mode

The power consumption is very low, because:
- LEDs are very efficient
- MCU goes into the sleep mode and wakes up only after pressing the roll button

https://youtu.be/tzcsPdcC8-U
