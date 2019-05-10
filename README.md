# Custom-Built MIDI Keyboard

The goal was to make something that had everything I ever wanted in a MIDI controller:

* 49 fully weighted keys
* Motorized faders
* Nice back-lit rotary encoder knobs
* Back-lit RGB buttons
* An LED strip on the front of the keyboard that lights up in correspondence to the notes you're playing
* A housing bay that can be used to hold external modules (in my case I'll use my OB-6 desktop module)
* MIDI in capabilities for showing current values and possibly even Mainstage patch names
* Quick MIDI channel switcher below the keybed, for being able to go, for example, from piano to lead synth very quickly
* Nice fancy wooden structure
* Part of the front is see-through
* Laptop and iPad stands that attach to the sides of the structure, able to also securely hold an external SSD connected by USB
* Hides most cables within the structure
* Pitch and mod wheels on BOTH sides of the keybed for left-handed play
* Smart octave switches that can go octave down, octave up, or intelligently split half the keyboard: lower half going down, upper half going up
* Smart chord pad backer: in that you give it a voicing, and the firmware outputs from now on on a seperate channel following what you're playing

To accomplish this, I'm using the following hardware:

* Fatar VMK149's keybed taken out of the structure, nothing else from the VMK149 is being used in this project
* OB-6 desktop module
* WS2812B LED strip
* Arduino MEGA 2560 for reading the keyboard scan matrix
* Arduino UNO for controlling the LED strip
* A second Arduino MEGA 2560 for controlling the motorized faders and rotary encoder knobs and buttons
* Power cables/outlets as sources for the OB-6, Arduinos, and laptop/iPad
* Lots of wood
* A CNC machine
