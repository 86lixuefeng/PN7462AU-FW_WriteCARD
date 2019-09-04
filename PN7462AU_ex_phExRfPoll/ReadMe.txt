/******************phExRfpoll - Example for Customer Demo ***********************************/
/* Features */
NonRTOS Based
On top of Bare Metal HAL

/* Clif Modes */
1. Reader Mode- Supports TypeA, TypeB, Felica, ISO15693, ISO18000p3m3 protocols.
To enable any of the protocol corresponding flags have to be enabled.
Device Limit supporting per Technology is 1.
Supports upto read and write for all protocols.
Supports proprietary cards Mifare Classic, Mifare UltraLight till read and write
Supports authentication and reauthentication for Mifare Classic and Inventory read and fast Inventory read command for ICODE SLIX
Note- Since Inventory read is a manufacturer specific proprietary command 
thus inventory read will work only for NXP Manufactured ICode Cards one of which is ICODE SLIX.
Supports Topaz/Jewel Tags -  Command supported RID, READ8. Also supports Rall for type1 static tags but not tested due to unavailability of card.

2. Peer Mode
Supports Active F 212 till atr req only.

3. Card MODE
Emulates as Type A Card and supports till I-Block exchange.
This example only performs card mode activation(until RATS-ATS) to demonstrate the usage of autoColl API


