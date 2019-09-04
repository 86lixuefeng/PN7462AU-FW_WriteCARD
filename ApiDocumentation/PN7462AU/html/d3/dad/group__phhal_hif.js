var group__phhal_hif =
[
    [ "Error Code: HIF HAL", "da/de4/group__grp_err___hal_hif.html", "da/de4/group__grp_err___hal_hif" ],
    [ "phhalHif_Hsu_t", "d0/d92/structphhal_hif___hsu__t.html", [
      [ "bIsHsuBoot", "d0/d92/structphhal_hif___hsu__t.html#a14c21d7f42011447c372bb5000bd14c7", null ],
      [ "bEOF", "d0/d92/structphhal_hif___hsu__t.html#ab42c948ceaa94563717739b2cb27b7a6", null ],
      [ "bStopBits", "d0/d92/structphhal_hif___hsu__t.html#a5789adb631af1f52abf498e72071023d", null ],
      [ "bDummyBytes", "d0/d92/structphhal_hif___hsu__t.html#ac965c4ad12c94f5fa683a7b301a2c09e", null ],
      [ "bBaudRate", "d0/d92/structphhal_hif___hsu__t.html#ab309a79f0a91d3350364b70ece8c3dbd", null ]
    ] ],
    [ "phhalHif_Config_t", "db/de6/structphhal_hif___config__t.html", [
      [ "phhalHif_IntfConfig", "dd/d5f/unionphhal_hif___config__t_1_1phhal_hif___intf_config.html", [
        [ "sI2cConfig", "dd/d5f/unionphhal_hif___config__t_1_1phhal_hif___intf_config.html#a699e2754c2a59d787111e05ff2c3ed5f", null ],
        [ "sSpiConfig", "dd/d5f/unionphhal_hif___config__t_1_1phhal_hif___intf_config.html#ade06934b2c7098559a82ceb0d70908cf", null ],
        [ "sHsuConfig", "dd/d5f/unionphhal_hif___config__t_1_1phhal_hif___intf_config.html#ac0ef6b50dece71f8be59f1bafae0a8c7", null ]
      ] ],
      [ "sConfig", "db/de6/structphhal_hif___config__t.html#a59aad639fd74bd4272754777cde848fb", null ],
      [ "bTimeout", "db/de6/structphhal_hif___config__t.html#a641883303b33d428b55592e5b1ab48d8", null ],
      [ "eInterface", "db/de6/structphhal_hif___config__t.html#a74986c998f9c4ca28e192ff3017e6581", null ],
      [ "eBufferType", "db/de6/structphhal_hif___config__t.html#a0d594a50330bc25e77ca5919f7718e63", null ],
      [ "bShortFrameLen", "db/de6/structphhal_hif___config__t.html#ac949b7e901ed2f764df9cf567d00692e", null ],
      [ "bStoreErrData", "db/de6/structphhal_hif___config__t.html#a7fdcd228b42362bf3ce3a59820d8ad3a", null ],
      [ "bHeaderSize", "db/de6/structphhal_hif___config__t.html#ae2436e93e146abd2a6f6cbb38bf746a2", null ]
    ] ],
    [ "phhalHif_RxBuffContext_t", "d6/df9/structphhal_hif___rx_buff_context__t.html", [
      [ "pdwBuffAddr", "d6/df9/structphhal_hif___rx_buff_context__t.html#a92294174160ba485a2808f109359a93a", null ],
      [ "IsDataAvailable", "d6/df9/structphhal_hif___rx_buff_context__t.html#aa6f677e8e9e3826e4fd365e89c13d420", null ],
      [ "dwDataLength", "d6/df9/structphhal_hif___rx_buff_context__t.html#ad860d29c8201305279fd42de59689b02", null ],
      [ "dwMaxSize", "d6/df9/structphhal_hif___rx_buff_context__t.html#a877f50947a19d6b7c5bdfe22a25142fc", null ]
    ] ],
    [ "phhalHif_AbortTransmission", "d3/dad/group__phhal_hif.html#ga3c5e623579c454621b1baef5f4c8520c", null ],
    [ "phhalHif_DeInit", "d3/dad/group__phhal_hif.html#ga09b911ced8f8a6cd7a2090512da039b8", null ],
    [ "pphhalHif_Callback_t", "d3/dad/group__phhal_hif.html#ga9cf37f635b2b9743bffb92ad8f8d6e44", null ],
    [ "phhalHif_DeInit_t", "d3/dad/group__phhal_hif.html#ga078153e94de476debc32fc5ee270dca8", null ],
    [ "phhalHif_AbortTransmission_t", "d3/dad/group__phhal_hif.html#ga62eb9561cc500aae1ce3ca4044466c41", null ],
    [ "phhalHif_BuffId_t", "d3/dad/group__phhal_hif.html#ga7a351ae38589351fa94e8211e563c371", [
      [ "E_RX_BUFFER_ID0", "d3/dad/group__phhal_hif.html#gga7a351ae38589351fa94e8211e563c371a1e4a756c28109a0b7f3e3de840f4d2fd", null ],
      [ "E_RX_BUFFER_ID1", "d3/dad/group__phhal_hif.html#gga7a351ae38589351fa94e8211e563c371a20ffb9e6d51e2ccf0576a8d3c29c0040", null ],
      [ "E_RX_BUFFER_ID2", "d3/dad/group__phhal_hif.html#gga7a351ae38589351fa94e8211e563c371a3ef5025d5b787f0164bd4d164496c820", null ],
      [ "E_RX_BUFFER_ID3", "d3/dad/group__phhal_hif.html#gga7a351ae38589351fa94e8211e563c371a099b8f18feefb4252441490d01c5c913", null ]
    ] ],
    [ "phhalHif_BuffType_t", "d3/dad/group__phhal_hif.html#gae5f9488f12a99c9d677e65f3853f7bae", [
      [ "E_RX_BUFFER_NORMAL", "d3/dad/group__phhal_hif.html#ggae5f9488f12a99c9d677e65f3853f7baeacec509a69580a3217f49cc47570057ba", null ],
      [ "E_RX_BUFFER_SHORT", "d3/dad/group__phhal_hif.html#ggae5f9488f12a99c9d677e65f3853f7baea691bc4eb87177ab4710edc7c398fc2c6", null ]
    ] ],
    [ "phhalHif_BuffFormat_t", "d3/dad/group__phhal_hif.html#gaa6db3827c1270c4a90b5e04381b0e064", [
      [ "E_BUFFER_FORMAT_FREE", "d3/dad/group__phhal_hif.html#ggaa6db3827c1270c4a90b5e04381b0e064a4786d3a263e15a1cb8297217dd6da2fc", null ],
      [ "E_BUFFER_FORMAT_FIXED", "d3/dad/group__phhal_hif.html#ggaa6db3827c1270c4a90b5e04381b0e064a535b997ec53ee3029582eba3349f14e8", null ],
      [ "E_BUFFER_FORMAT_NATIVE", "d3/dad/group__phhal_hif.html#ggaa6db3827c1270c4a90b5e04381b0e064ab48a9bb19350f04b5a708299bc0b8938", null ]
    ] ],
    [ "phhalHif_Hsu_BaudRate_t", "d3/dad/group__phhal_hif.html#ga64fab9104746f5a857c3ca26586019ce", [
      [ "E_HSU_BAUDRATE_9_6K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019ceae1b3061b71f575a7c3e933a4d9acec21", null ],
      [ "E_HSU_BAUDRATE_19_2K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cea9b2a1e2e731af289684feb95e6ffadb0", null ],
      [ "E_HSU_BAUDRATE_38_4K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cead45fe0b73f8c18b67b9d6618d344b52a", null ],
      [ "E_HSU_BAUDRATE_57_6K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cea06e271656a67b484a9b84e11c57265b7", null ],
      [ "E_HSU_BAUDRATE_115_2K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cea3abdc5c280f25cbe8d1e44daedff6bc6", null ],
      [ "E_HSU_BAUDRATE_230_4K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cea7f1d746f0c038ef070054d7044117918", null ],
      [ "E_HSU_BAUDRATE_460_8K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cead35d38dd99e6f4229c247528a614c4c1", null ],
      [ "E_HSU_BAUDRATE_921_6K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019ceaf67431f669e051ac4253189fd3f7fd07", null ],
      [ "E_HSU_BAUDRATE_1288K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cea67deb88343398f0427bfd1ddb4bb8c8a", null ],
      [ "E_HSU_BAUDRATE_2400K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cea4c8828d067adc6da9ec02abddf5a820b", null ],
      [ "E_HSU_BAUDRATE_3500K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cea0330fb00e6e4b8d5c7713d6904bd6201", null ],
      [ "E_HSU_BAUDRATE_3750K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019ceaac63c0fe50b573ff3c71cf95925cdf32", null ],
      [ "E_HSU_BAUDRATE_4000K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cea5d9336f1b032ca4dee23fa91f12ac941", null ],
      [ "E_HSU_BAUDRATE_5000K", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cea8add863b74dc0d6bb8a354f7d7a6535d", null ],
      [ "E_HSU_BAUDRATE_SEMIAUTO", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019cea3cfec12ae2c81cca428f17c4dd6381ce", null ],
      [ "E_HSU_BAUDRATE_AUTO", "d3/dad/group__phhal_hif.html#gga64fab9104746f5a857c3ca26586019ceaba05685d4e8779ce36503ba5979f2510", null ]
    ] ],
    [ "phhalHif_Sel_t", "d3/dad/group__phhal_hif.html#ga44a5a05a467fe8cdd1a2ef9e0ecdf006", [
      [ "E_HIF_DISABLE", "d3/dad/group__phhal_hif.html#gga44a5a05a467fe8cdd1a2ef9e0ecdf006acbceb12c573c5c17359cb167eaae0c5e", null ],
      [ "E_HIF_I2C", "d3/dad/group__phhal_hif.html#gga44a5a05a467fe8cdd1a2ef9e0ecdf006ae6214d4245d6afc4f8c7dcd9a5a06fcf", null ],
      [ "E_HIF_SPI", "d3/dad/group__phhal_hif.html#gga44a5a05a467fe8cdd1a2ef9e0ecdf006a17bf5580608c985b7d15d446c6885f77", null ],
      [ "E_HIF_HSU", "d3/dad/group__phhal_hif.html#gga44a5a05a467fe8cdd1a2ef9e0ecdf006ab82b53924ce87f95c8997777e466687c", null ],
      [ "E_HIF_USB", "d3/dad/group__phhal_hif.html#gga44a5a05a467fe8cdd1a2ef9e0ecdf006a26727f9e63cd1e3c7cfeaecf6b0fa47b", null ]
    ] ],
    [ "phhalHif_Init", "d3/dad/group__phhal_hif.html#gaa324f27ac2d93296b51a6863830febff", null ],
    [ "phhalHif_Transmit", "d3/dad/group__phhal_hif.html#ga614eeab0103d70bdc3f15c0297f42e18", null ],
    [ "phhalHif_ConfigureWaterLevel", "d3/dad/group__phhal_hif.html#gae5936edea34da4e268c7630557d18e6c", null ],
    [ "phhalHif_InitRxBuffer", "d3/dad/group__phhal_hif.html#ga2fe3879e49e8639b7de488ecac11e70e", null ],
    [ "phhalHif_DeInitRxBuffer", "d3/dad/group__phhal_hif.html#gaa4faa65e9496509473497aeffe733677", null ],
    [ "phhalHif_ReleaseRxBuffer", "d3/dad/group__phhal_hif.html#ga1d843a583aee2f8cda3f8dc307fdc3a7", null ],
    [ "phhalHif_Reset", "d3/dad/group__phhal_hif.html#gac0633850cb2af0aadc6d3ce5fc6fab24", null ]
];