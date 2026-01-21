# Arduino based Darkroom Enlarger Timer _(WIP)_

Here you can find the code, electronics schematics and any 3d printing related to this project. <br/>
An early proof of concept sketch on wokwi is available [here](https://wokwi.com/projects/437627156224860161). <br/>
This is based on a Nano type board but you can adapt it to use anything you have on hand.

## Bill of Materials
- 1 x 3-Pin Male Panel Mount AC Power Supply Socket
- 1 x 2-Contact switch (to turn everything on and off)
- 2 x fuse holder
- 1 x 0.6A fuse (for the arduino)
- 1 x fuse up to specs with your enlarger lamp (usually 1-1.5Amps)
- 1 x 240VAC/24VAC min 0.6A transformer
- 1 x bridge rectifier
- 1 x 2200uF/35V electrolytic capacitor
- 1 x LM7812
- 1 x LM7805
- 1 x 1N4007 diode
- 1 x BC547
- 1 x 10KΩ 1/8Watt resistor
- 1 x 12VDC 2(DC)+3(AC) contact relay (capable of 240 example https://grobotronics.com/12v-dpdt-8a-250v.html)
- 1 x BC547
- 6 x pushbuttons
- 1 x 5 position terminal block (display)
- 1 x 3 position terminal block (AC power)
- 2 x 2 position terminal blocks (enlarger and safelight outlets)
- 1 x Led Display 8-Digit with MAX7219 - **Red** (https://grobotronics.com/led-display-8-digit-with-max7219-red.html)
- 1 x Arduino (I used Nano clone but feel free to modify anything you need to use your prefered board)
- 2 x power outlets (for the enlarger and the safelight)
- 6 x 2 position terminal blocks (optional, for the buttons)
- 1 x Box large enough to hold everything (or the one I have for 3d printing)
- 1 x Breadboard (160mm x 100mm was somewhat comfortable)
- thin wire, multiple colors, 5-12V
- thick wire, multiple colors, 240V
- Solder

### Tools used
- Soldering iron
- Wire cutter
- Lighter to strip the wires
- **Optional** Non-plastic brush to clean mistakes
- **Optional** Wax from a candle to insulate the 240V side of the board for safety. (I used UV resin in my first make of the board and had to clean it out many times to correct mistakes)

## Upcoming updates:
- [X] Bill of materials
- [X] Electronics schematics
- [X] 3d Model to 3D Print a box
- [ ] Assign a button to stop the exposure once the enlarger has started

<br/>
<br/>
<br/>

> [!CAUTION]
> This project involves working with 220V AC and is therefore ~~kind of~~ dangerous. Proceed at your own risk.

Licensed under GPL-V3
Copyright (c) 2025 Ioannis Mamais
