/******************phExMain - Example for Customer Demo ***********************************/
/* Features */
Includes Both CT and Clif
Includes PAL for both CT and Clif
Includes DiscLoop for Clif
RTOS Based

/* Clif Modes */
1. Reader Mode- Supports TypeA, TypeB, Felica, ISO15693, ISO18000p3m3 protocols
Device Limit supporting per Technology is 1.
Supports upto read and write for all protocols.
Supports proprietary cards Mifare Classic, Mifare UltraLight and Mifare UltraLightC till read and write
Supports authentication and reauthentication for Mifare Classic and Inventory read command for ICODE SLIX
Note- Since Inventory read is a manufacturer specific proprietary command 
thus inventory read will work only for NXP Manufactured ICode Cards one of which is ICODE SLIX.
Supports Topaz/Jewel Tags -  Command supported RID, READ8, WRITE-NE8

2. Peer Mode
Supports Active F 212 till PSL req only.
Supports Passive A 106 and performs dep exchange.
API Given for active dep transmit for f212 but not tested

3. Card MODE
Emulates as Type A Card and support till Rats exchange.

/* CT Mode*/
ISO7816 Profile- 
phExMain_Ct.c :- Supports SCosta card(ISO7816 profile) Presence check,Activation,PPS Exchange for higher baud rate,Apdu exchange with card according to card's protocol(Either T0 or T1). 
In this file ,Supporting Reading and writing binaries to SCosta card for selected EF.


Note:
=====
The Example is applicable only for PN7462. Not Applicable for PN7362/PN7360.


