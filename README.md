# Linear light digital sensor

![Alt Text](https://github.com/serhiykobyakov/linear-light-digital-sensor/blob/main/Photodiode.jpg)

## What is it

Linear light sensor can be made using a photodione and [transimpedance amplifier](https://en.wikipedia.org/wiki/Transimpedance_amplifier). To get a digital output we need an ADC and a microcontroller to communicate with PC (Arduino for example).

Here I have only the software without all the other stuff (circuitry and mechanic stuff).

Arduino does all the job managing a 24bit ADC and also average the ADC reads for certain amount of time. Interesting feature: there are two command instead of one to start averaging and get the result. This appoach allow the main application to process something else (this way we do not block the main app) while Arduino averaging the data. The Pascal side of the code is really small because Arduino does all the work and math.

The software has been tested for errors, stability and speed.

## Install

### Arduino

1. Make directory "Photodiode" in the sketchbook directory on your PC (it is "Arduino" by default, check the preferences in Arduino IDE).
2. Put the Photodiode.ino into "Photodiode" directory.
3. Open Arduino IDE and set your Arduino board
4. Compile the sketch and upload it to the board.

### Free Pascal

1. Download Photodiode.pas and Photodiode.ini
2. Get ArduinoDevice.pas from [my Arduino device repository](https://github.com/serhiykobyakov/Arduino_device_FPC) 
3. Put in your directory jedi.inc, synafpc.pas, synaser.pas, synautil.pas from [Synapse repository](http://synapse.ararat.cz/doku.php/download)
4. Use repository info and and see the comments in files to get it work

## Contact
For reporting [bugs, suggestions, patches](https://github.com/serhiykobyakov/linear-light-digital-sensor/issues)

## License
The project is licensed under the [MIT license](https://github.com/serhiykobyakov/linear-light-digital-sensor/blob/main/LICENSE)
