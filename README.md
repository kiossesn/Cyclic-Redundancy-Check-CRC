# Cyclic-Redundancy-Check-CRC
Implementation of a CRC algorithm (networking) on a virtual communication between a sender and a receiver, written in C++.\
[CRC_documentation.docx](https://github.com/user-attachments/files/16405935/CRC_documentation.docx)

<pre>
Give n:
16

Give k:
12

Give a binary number P as prototype (must be 5 bits):11001


SENDER SIDE...
D : 111010101100
D+zeros : 1110101011000000
Remainder : 1011
Encoded Data (Data + Remainder) :1110101011001011

TRANSMISSION...
Original: 1110101011001011
Noisy:    1100101011001011

RECEIVER SIDE...
Received: 1100101011001011
Remainder in receiver : 0110
there is some error in data

</pre>
