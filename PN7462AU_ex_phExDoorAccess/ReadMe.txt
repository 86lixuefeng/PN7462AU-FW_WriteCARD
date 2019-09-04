/********************phExDoorAccess-Example for customer demo**********************/

/*features*/
This application is using a MIFARE DESFire EV1 card for the authentication.
Data exchange is done over contactless interface.
Software key store or SAM key store is used for storing the authentication key.
By default, the software key store is enabled.
SAM is a key storage element and it should be inserted in the CT main.
Supports- Type A, B or F, ISO15693, ISO18000-3M3
This authenticate the card using the key stored in SAM, if SAM is not present, then software keys can be used for authentication.
If authentication is successful a block of data will be read from the card.



Note:
=====
The Example is applicable for PN7462
