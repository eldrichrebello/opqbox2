OPQBOX 2
========

Overview
--------

This repository will eventually hold schematics, firmware, software, and documentation for the second generation Open Power Quality metering device (OPQBox2). 

Currently, OPQBox2 is in the design stage.  We are circulating its design now in order to solicit feedback and make improvements prior to layout. We greatly appreciate your willingness to provide us with feedback.

Requirements
------------

The design of OPQBox2 is influenced by our experience with the first generation OPQBox.  Design and documentation information about our first generation hardware is available at the [OPQBox1 repository](https://github.com/openpowerquality/opqbox1). 

We performed a [pilot study of our first generation hardware and software](http://openpowerquality.org/technology/g1-pilot-study.html), and one of the results of this study are the following major requirements for OPQBox2:

  * Support event recording upon power cutout. OPQBox2 includes a Li-Po battery so that the box can record an event if power is interrupted prior to shutdown. Also to support this requirement, OPQBox2 includes nonvolatile 64K EEPROM memory.
  
  * Satisfy IEEE PQ standards.  This includes 256 samples per waveform and locking to 60Hz input.
  
  * Improve data quality through dual transformers. OPQBox2 has an internally fused UL-listed dual secondary transformer so that the transformer used to power the device is independent from the transformer used to collect PQ data.
  

Here are some additional details on the proposed changes for OPQBox2:

OPQBox1 Power | OPQBox2 Power
------------- | -------------
12V transformer for measurement | Isolated 5V DC-DC

that's all.




The directory structure is as follows:


 - Schematics. Contains schematics and bill of materials. 
 - Firmware. SMT32 Firmware and documentation.
 - Software: Raspberry PI and Linux software.

