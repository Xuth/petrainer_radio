# petrainer_radio
investigations in receiving, decoding, and transmitting signals for the one of the petrainer shock collars

I've seen writeups for various  petrainer shock collars/remotes on the web and the different iterations seem to use slightly different encoding schemes but on a simple ASK 433 MHz transmitter.

I purchased a set of these:  https://www.amazon.com/gp/product/B00W6ZHZMI/ (and since writing this initially I have purchased several more sets).

Along with that I purchased a set of very simple/cheap transmitter/receiver pairs: https://www.amazon.com/gp/product/B07B9X46LB/ which can send/receive the necessary signals using simple arduino micro controllers.


The basic protocol to the point that I've decoded it:

* startcode is a low of 8000 uS (or more) followed by high of 800 uS
* 0 bit is a low of 800 uS followed by high of 200 uS
* 1 bit is a low of 1600 uS followed by high of 200 uS

message is start code followed by 41 bits

based on a sample of 8 units/4 remotes, it looks like it's best if we group the first bit with the start code (or treat it as part of the start code) and then the subsequent 40 bits into
five 8 bit bytes, MSB first order.

The solitary bit with the start code is a 0 for the units I've purchased (Again, let's treat it as though it was part of the start code or maybe it's reserved so that if the protocol changes it can be set high).

* byte 0: 
  * the first (high) nibble (4 bits) specifies which of the two devices.
    * 1000 for device 1 and
    * 1111 for device 2
  * the second nibble specifies the command
    * 0001: shock
    * 0010: vibrate
    * 0100: audio
    * 1000: light
    * (can you do multiple functions at once by or'ing the functions?)

* byte 1: First byte of the paired key.
* byte 2: Second byte of the paired key.
  * This is the "paired key".  Each remote has a 16 bit integer associated with it so that the remote can't affect other units not paired to it.  16 bits is probably fine for short range use and it takes 1/15th of a second to send a full message so trying all combinations would take over an hour.  But of course none of this is cryptographically signed or anything like that so anyone who records the usage could copy it.

* byte 3: intensity level associated with the command
  * 0-100 for shock/vibrate
  * 0 for audio and light which don't have levels associated with them

* byte 4: this appears to be an inversion of byte 0, 
  * reverse the bit order
        and then invert the individual bits.
  * this appears to be a simplistic checksum.  Assuming such, it is a check against shock instead of vibrate for instance and similarly which collar
	and if the 16 bit paired ID isn't correct it likely just doesn't do anything (what are the odds that another of these things
	is in the area with the ID off by one and that's the bit that gets messed up?)

I've not explored sending invalid commands, multiple commands, invalid checksums or trying to pair the device with other device ids.  However I can easily control the eight collars I have from an arduino.  First I paired all of them with their individual remotes (so I could still use their remotes with them) and then read the associated keys and verified that my guesses about the protocol were valid.  Now I can send commands to each of the individual devices.

More info and my working notes are in signal_decoding.txt
