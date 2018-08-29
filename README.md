# Charging Station with Locker

This is a simple charging station kiosk project with a locker feature using Arduino MEGA as the main controller.

## About the Project

* EEPROM was used to manage the state, passwords and timers of the project.

* This project currently supports up to two (2) users.

* Passwords for each user and admin supports up to eight (8) characters.

* Disabling the solenoid lock(s) (opening the vault) when active, can be accessed thru the user's page with the use of its respective registered password or thru the Admin's access.

### Parts list

```
1 Arduino Mega 2560
1 16x2 LCD (i2C)
1 4x4 Keypad Module
1 Piezoelectric Buzzer
2 Micro Switch

4 Opto-isolator
4 NPN Power Transistor

2 5v Solenoid Lock
2 5v Relay Switch

2 10k ohms 1/4w Resistor
4 1k ohms 1/4w Resistor
4 240 ohms 1/4w Resistor
4 1N4001 Diode

2 Smartphone Charger
```

## Schematic Diagram

https://www.digikey.com/schemeit/project/charger-lock-A9QGMDO4022G/

## Built with

Arduino IDE: https://www.arduino.cc/en/Main/Software

Wire/i2C library (built-in)
Keypad by Mark Stanley, Alexander Brevig
LiquidCrystal I2C by Frank de Brabander
EEPROMex by Thijs Elenbaas

## Author

* **Jansen Ducusin** - *Initial work* - [Senducusin](https://github.com/senducusin)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

