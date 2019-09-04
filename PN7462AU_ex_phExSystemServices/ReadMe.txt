/*********************phExSystemServices- Example for customer Demo******************************/


/*Features*/
1. This example application demonstrates system services invocation.

2. The PN7462AU provides ROM services, also described in
   PN7462AU/phROMIntf/phhalSysSer/inc/phhalsysSer.h and with more detailed in
   API documentation.

3. SECROW lock:-

   The SecRow contains the SWD access bits, code write-protection bits and RSTN
   pin behaviour bits.  For blocking any further writes to SecRow, the
   phhalSysSer_OTP_setscrewlock() is used.

4. Code write protection:-

   It is required to lock flash memory from write at HW level. For such use
   cases, phhalSysSer_OTP_setscrewlock() is used to lock the flash memory from
   any further write.

5. Block SWD debugging:-

   This command disables PN7462AU SWD interface. when the PN7462AU IC is
   delivered from production to user, the default SWD access level enables the
   user to view and debug user flash memory, user EEPROM memory, user RAM memory
   and peripherals registers.

6. Disable primary download:-

   This command is used to irreversibly disable the ROM primary download
   feature. This feature is typically used after development and flashing of
   secondary downloader in the flash memory, for subsequent code/data upgradws.

7 Perform in application Programming:-

  Application asks for FLASH page number. Page is 128 bytes long, for 158kb of
  the flash memory, the page number is in range 0-1263.  The selected flash page
  is updated from user programmable values.
