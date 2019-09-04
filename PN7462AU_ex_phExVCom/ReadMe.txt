/******************phExVCom - Example for Customer Demo ***********************************/
/* Features */
Includes CDC Class Implementation for the COM Port demonstration on PN7462AU.

Description:
-----------
The example demonstrates the how PN7462AU can be used a VCOM.
The example demonstrates how the Type A Polling, RF Field ON and RF Field OFF can be performed
using the COM port commands.

Serial COM Application Settings:
--------------------------------
Baud Rate	9600
Data Bits	8
Stop Bits	1
Parity		None
Flow Control 	None

Steps:
=====
1. Load the Application binary in PN7462AU and Run.
2. Open the Serial Port application and select the COM Port (PN7462AU VCOM).
3. In the Terminal Type as follows:
   a) Command T - For Starting the Type A Polling.
   b) Command O - For RF Field ON
   c) Command F - For RF Field OFF.
4. Send the Command T in the terminal and the Type A Polling starts.
   Place the Card and UID information of the Card is displayed till the card is removed.
5. Send the Command O the Field Will ON always.
6. Send the Command F the Field will be OFF always.

COM Port Driver:
===============
COM Port driver is present in the following folder of the example:

\Platform\PN7462AU\external\CDCLib\inf\PN7462AU_VCOM.inf





