'''
Created on 07-Jan-2015

@author: ing05193
'''
import sys
import csv
from pprint import pprint
from xml.etree import ElementTree as ET
from xml.dom import minidom
import time

__VERSION__ = '2016.01.21_00'

#
#
# 2016.01.21_00:    PGh : Add regname and rfprotocol fields to assist GUI
# 2015.12.18_00:    PGh : More doxygen friendly header file
# 2015.11.18_01:    PGh : Create two separete header files for Load Protocol
# 2015.11.18_00:    PGh : Need post and pre copy.
# 2015.11.17_00:    PGh : RfAntennae, RfAPC, RfPcdShaping, RfTxOCP kept and used from antennae 
#              :        : specific folders.
# 2015.11.06_00:    PGh : Processs I_BOOT_TX,I_BOOT_RX registers
# 2015.10.14_00:    PGh : [PT_SC3779] ClifEE : Generate _output_xml_sizes Per Antennae
# 2015.09.23_00:    PGh : Doxygen documentation
# 2015.09.11_00:    PGh : QA C Fixes for length of error.
# 2015.09.10_00:    PGh : Create _ClifEE_debug.xml to more easily diff the changes
# 2015.09.08_00:    PGh : Single Mask
# 2015.09.07_00:    PGh : Masks for CLIF_ANA_TX_AMPLITUDE_REG, CLIF_TX_CONTROL_REG,
#              :        : and PMU_TXLDO_CONTROL_REG added.
# 2015.09.01_01:    PGh : For active mode, CLIF_ANA_TX_AMPLITUDE_REG should not be masked
# 2015.09.01_00:    PGh : Revert changes of PHHALRF_LP_I_TX_15693_53_10
# 2015.08.25_00:    PGh : Fixed verison number
# 2015.08.24_01:    PGh : PHHALRF_LP_I_TX_15693_53_10 is after Unknown
# 2015.08.24_00:    PGh : Special handling for I_TX_15693_53_10
# 2015.07.31_00:    PGh : Add version information of the excel sheet.
# 2015.04.14_01:    AKs : Updated comments for phhalRf_LP_Int.h file.
# 2015.04.14_00:    PGh : Instead of u32_v%02d, use TECH and Reg Name
# 2015.02.03_00:    AKs : output .c file is in binary mode
# 2015.01.21_00:    PGh : Added Keep_AsIs, Unknown, Max in Enum Files
#                       : Changed the way ENUMs are Generated
# 2015.01.13_00:    PGh : Keeping only values in EEPROM.
#                       : Generate corresponding .h file to be consumed by Flash Code.
# 2015.01.09_01:    PGh : Using description for the register name,
#                       : and description for the register values.
#                       : Keep all RF Values in a single Page
# 2015.01.09_00:    PGh : Updated code
#



DEBUG_ADD_REG_NAME = False
WITH_CFGOFFSETS_AND_REG_ADDRESS_IN_EE = False

ALLOW_PARTIAL_EXCEL_SHEETS = True

class ClifCSVToXml(object):
    '''
    classdocs
    '''

    def __init__(self, inIMCsvFile, inTMCsvFile, inVERSIONCsvFile, outROOT_Name):
        '''
        Constructor
        '''
        print("Input Files: %s, %s" % (inTMCsvFile, inTMCsvFile))
        print("Ouput Files: %s.*" % (outROOT_Name))

        self.o_im_csv = open(inIMCsvFile, 'r')
        self.o_tm_csv = open(inTMCsvFile, 'r')
        self.o_ver_csv = open(inVERSIONCsvFile, 'r')

        self.o_xml = open(outROOT_Name + ".xml", "w")
        self.o_debug_xml = open(outROOT_Name + "_debug.xml", "w")
        self.o_c_c = open(outROOT_Name + ".c.txt", "wb")
        self.o_c_v = open(outROOT_Name + ".v.txt", "wb")
        self.str_version_string_workbook_name = None

        self.d_im_values = {}
        self.d_tm_values = {}
        self.o_c_c.write("""/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014,2015
 *
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *    NXP reserves the right to make changes without notice at any time.
 *   NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *   copyright or trademark. NXP must not be liable for any loss or damage
 *                            arising from its use.
 */

/** @file
 *
 * Helper for Load Protocol settings for CLIF.  This file works in
 * close co-ordination with values stored in EEPROM.
 *
 * This file contains constants that shall never change.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-01-21 11:58:36 +0530 (Thu, 21 Jan 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17460 $
 */

#ifndef PHHALRF_LP_INT_H
#define PHHALRF_LP_INT_H

""")
        self.o_c_v.write("""/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014,2015
 *
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *    NXP reserves the right to make changes without notice at any time.
 *   NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *   copyright or trademark. NXP must not be liable for any loss or damage
 *                            arising from its use.
 */

/** @file
 *
 * Helper for Load Protocol settings for CLIF.  This file works in
 * close co-ordination with values stored in EEPROM.
 *
 * This file contains values that are variable and depend on 
 * Contents of the Excel Sheets containing CLIF Settings.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-01-21 11:58:36 +0530 (Thu, 21 Jan 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17460 $
 */

#ifndef PHHALRF_LP_V_H
#define PHHALRF_LP_V_H

""")
        local_time = time.localtime()
        datetime_str = time.strftime("%Y-%m-%d", local_time)
        self.o_c_c.write("/* Generated on %s by clifcsv_to_xml.py script version %s */\n\n" % (datetime_str,__VERSION__))
        self.o_c_v.write("/* Generated on %s by clifcsv_to_xml.py script version %s */\n\n" % (datetime_str,__VERSION__))



    def _CloseFiles(self):
        self.o_c_c.write("\n#endif /* PHHALRF_LP_INT_H */\n")
        self.o_c_v.write("\n#endif /* PHHALRF_LP_V_H */\n")
        self.o_im_csv.close()
        self.o_tm_csv.close()
        self.o_ver_csv.close()
        self.o_xml.close()
        self.o_debug_xml.close()
        self.o_c_c.close()
        self.o_c_v.close()
        
        self.CmdToCopyOutputXmlSizesHtml()
        
    def CmdToCopyOutputXmlSizesHtml(self):
        if self.str_version_string_workbook_name != None:
            with open("_copy_output_xml_size_html.bat","w") as copy_xml_sizes_html:
                copy_xml_sizes_html.write("cd /d %~dp0\n")
                copy_xml_sizes_html.write("copy /y %s\\*.xml .\n"%(self.str_version_string_workbook_name))
                copy_xml_sizes_html.write("copy /y _output_xml_sizes.html _output_xml_sizes_%s.html\n"%(self.str_version_string_workbook_name))
                

    def _parse_version_info_csv(self, o_csv):
        a_version_info = []
        a_version_info.append("")
        odCsv = csv.reader(o_csv)
        for csv_entry in odCsv:
            if len(csv_entry) >= 2 and len(csv_entry[1]) != 0:
            	key_string = csv_entry[0]
            	value_string = "  ".join(csv_entry[1:],).replace("\n","\r\n                    : ")
            	write_string = "%15s : %s"%(key_string,value_string)
                a_version_info.append(write_string)
                if csv_entry[0] == 'Name':
                    self.str_version_string_workbook_name = csv_entry[1]
        return a_version_info


    def _parse_the_csv(self, o_csv, tx_keys, rx_keys, d_values):
        odCsv = csv.DictReader(o_csv)
        xd_root = ET.Element("ROOT")
        register_node = None
        bit_field_node = None
        for csv_entry in odCsv:
            reg_name = csv_entry['RegisterName']
            bit_field = csv_entry['RegisterFiled']
            
            if len(bit_field) > 0 and register_node is not None:
                bit_field_node = ET.SubElement(register_node,bit_field)
                bit_field_node.text = ""
            if bit_field_node is not None:
                for key, val in csv_entry.items():
                    if len(val) > 0:
                        child = ET.SubElement(bit_field_node, key)
                        child.text = str(val)
            elif register_node is not None:
                for key, val in csv_entry.items():
                    if len(val) > 0:
                        child = ET.SubElement(register_node, key)
                        child.text = str(val)
            if len(reg_name) > 0:
                register_node = ET.SubElement(xd_root,reg_name)
                register_node.text = ""
                bit_field_node = None
                # if this is a register and not a bit field
                reg_add = CLIF_REGS[reg_name]
                reg_offset = (reg_add - CLIF_REG_START_ADDRESS) / 4
                for cfg_keys in tx_keys + rx_keys:
                    if ALLOW_PARTIAL_EXCEL_SHEETS and cfg_keys not in csv_entry:
                        csv_entry[cfg_keys] = []
                    reg_value_str = csv_entry[cfg_keys]
                    if len(reg_value_str) > 0:
                        reg_value_int = int(reg_value_str, 16)
                        d_values[cfg_keys].append(
                                (reg_name,  # hex(reg_add),
                                 reg_add,
                                # hex(reg_offset),
                                reg_offset,
                                reg_value_int))
        self._indent_xml(xd_root)
        self.o_debug_xml.write(ET.tostring(xd_root))
        
    def _indent_xml(self,elem, level=0):
        i = "\n" + level*"  "
        if len(elem):
            if not elem.text or not elem.text.strip():
                elem.text = i + "  "
            if not elem.tail or not elem.tail.strip():
                elem.tail = i
            for elem in elem:
                self._indent_xml(elem, level+1)
            if not elem.tail or not elem.tail.strip():
                elem.tail = i
        else:
            if level and (not elem.tail or not elem.tail.strip()):
                elem.tail = i
    def _dump_values(self, o_xml, o_c_c, o_c_v, keys, d_values, the_name, the_description):
        ee_cfg_offsets = []
        ee_clif_offsets = []
        ee_clif_values = []

        last_length = 0

        if WITH_CFGOFFSETS_AND_REG_ADDRESS_IN_EE:
            xml_structure_cfg_offsets = ET.SubElement(self.xml_page,
                "structure",
                name = "%s_cfg" % (the_name,))
            ET.SubElement(xml_structure_cfg_offsets, "description").text = \
                "Configuration offset and number of configurations for %s" % (the_description)
        if WITH_CFGOFFSETS_AND_REG_ADDRESS_IN_EE:
            xml_structure_clif_regs = ET.SubElement(self.xml_page,
                "structure",
                name = "%s_regs" % (the_name,))
            ET.SubElement(xml_structure_clif_regs, "description").text = \
                "Registers to be set for %s.\n\nNote: The values are relative offsets in CLIF Namespace" % (the_description)
        xml_structure_clif_vals = ET.SubElement(self.xml_page,
            "structure",
            name = "%s_val" % (the_name,))
        ET.SubElement(xml_structure_clif_vals, "description").text = \
            "Value to be applied to the corresponding CLIF register for %s." % (the_description)

        o_c_c.write("/* %s */\n" % (75 * '='))
        o_c_c.write("/* %-75s */\n" % (the_description))
        o_c_c.write("/* %s */\n" % (75 * '='))

        o_c_c.write("/* PRQA S 0779 ++ */\n/** Configuration offsets for %s.\n * @{ */\n" % (the_description))
        o_c_c.write("typedef enum phhalRf_LP_%s {\n" % (the_name,))

        keys_and_trailerenums = keys + [
                the_name.upper() +'_Keep_Asis',
                the_name.upper() +'_Unknown',
                ]

        keys_and_trailerenums = keys_and_trailerenums + [
            the_name.upper() +'_Max' ]

        for (count, cfg_key) in enumerate(keys_and_trailerenums):
           val = "(%d)" % (count)
           enum_name = "E_PHHAL_RF_LP_" \
            + cfg_key.upper()
           o_c_c.write("    %-30s = %6s%s\n" % (
                enum_name,
                val,
                "" if count + 1 == (len(keys_and_trailerenums)) else ",",
                ))
        o_c_c.write("} phhalRf_LP_%s_t;\n" % (the_name,))
        o_c_c.write("/** @} */\n/* PRQA S 0779 -- */\n\n")

        for (count, cfg_key) in enumerate(keys):
            cfg_values = d_values[cfg_key]
            # cur_len_plus_offset = len(cfg_values) + last_length
            if DEBUG_ADD_REG_NAME:
                ee_cfg_offsets.append(cfg_key)
                ee_clif_offsets.append(cfg_key)
                ee_clif_values.append(cfg_key)

            last_cfg_key = cfg_key
            ee_clif_offsets.append((cfg_key))

            for (vCount, (reg_name, reg_add, reg_offset, reg_val)) in enumerate(cfg_values):
                ee_clif_offsets.append((
                        reg_offset,
                        reg_name,
                        reg_add,
                        ))
                ee_clif_values.append(reg_val)
                if last_cfg_key:
                    new_cfg_key_description = "\n\nNote: Configuration: %s starts from here." % (last_cfg_key)
                    last_cfg_key = None
                else:
                    new_cfg_key_description = ""
                if WITH_CFGOFFSETS_AND_REG_ADDRESS_IN_EE:
                    xml_reg_offset = ET.SubElement(
                        xml_structure_clif_regs,
                        "field",
                        name = "u8_r%02d" % (vCount + last_length),
                        regname = reg_name,
                        rfprotocol = cfg_key,
                        type = "u8",
                        value = hex(reg_offset).replace("0x", "").replace("L", ""),
                        valuetype = 'hex')
                    ET.SubElement(xml_reg_offset, "description").text = \
                        "Register %s: Address: 0x%08X%s" % (reg_name, reg_add, new_cfg_key_description)
                xml_reg_val = ET.SubElement(
                    xml_structure_clif_vals,
                    "field",
                    name = "u32_%s_%02d_%s" % (cfg_key,vCount,reg_name[5:]),
                    regname = reg_name,
                    rfprotocol = cfg_key,
                    type = "u32",
                    value = hex(reg_val).replace("0x", "").replace("L", ""),
                    valuetype = 'hex')
                ET.SubElement(xml_reg_val, "description").text = \
                    "Value for Register %s.%s" % (reg_name, new_cfg_key_description)
            last_length = len(cfg_values) + last_length
            if WITH_CFGOFFSETS_AND_REG_ADDRESS_IN_EE:
                xml_structure_cfg_offsets_filed = \
                    ET.SubElement(xml_structure_cfg_offsets,
                        "field",
                        name = cfg_key,
                        type = "u8",
                        value = str(last_length),
                        valuetype = 'int')
            ee_cfg_offsets.append(
                (last_length,
                 '' if  1 + count == len(keys) else ',',
                 cfg_key))

        last_length = 0

        o_c_v.write("/**\n * Number of registers to be loaded for %s.\n */\n"%(the_name,))
        o_c_v.write("#define PHHAL_RF_LP_INT_%s_CFG_OFFSETS { \\\n" % (the_name.upper(),))
        for e in ee_cfg_offsets:
            o_c_v.write("    %2d%-2s/* %-20s*/ \\\n" % e)
        o_c_v.write("}\n\n")

        # o_c_v.write("uint8_t %s_clif_regadd_offsets[] = {\n" % (the_name))
        o_c_v.write("/**\n * Register address offset to be loaded for %s.\n */\n"%(the_name,))
        o_c_v.write("#define PHHAL_RF_LP_INT_%s_CLIF_REGADD_OFFSETS { \\\n" % (the_name.upper(),))
        for (count, e) in enumerate(ee_clif_offsets):
            if type(e) is tuple:
                o_c_v.write("    0x%02X%-2s/* %-36s (@0x%08X) */ \\\n" % (
                                e[0],
                                "" if count + 1 == len(ee_clif_offsets) else ",",
                                e[1],
                                e[2]
                                    ))
            else:
                o_c_v.write("    /* Mode: %-50s */ \\\n" % (str(e)))
        o_c_v.write("}\n\n")
        self._create_mask(o_c_v, the_name, ee_clif_offsets, {
                    "CLIF_ANA_TX_AMPLITUDE_REG" : "1",
                    "CLIF_TX_CONTROL_REG" : "2",
                    #"PMU_TXLDO_CONTROL_REG" : "3",
                })
        
        # o_c_v.write("uint32_t %s_reg_clif_values[] = {\n" % (the_name))
        # for e in ee_clif_values:
        #    o_c_v.write(" 0x%08X," % e)
        # o_c_v.write("\n};\n")

    def _create_mask(self, o_c_v, the_name, ee_clif_offsets, masks):
        if "_TX" in the_name.upper():
            o_c_v.write("/** Internal logic for masking */\n")
            o_c_v.write("#define PHHAL_RF_LP_INT_%s_MASK { " % (the_name.upper(),))
            active_mode = True
            for (count, e) in enumerate(ee_clif_offsets):
                if type(e) is tuple:
                    if e[1] in masks and not active_mode:
                        o_c_v.write(masks[e[1]])
                    else:
                        o_c_v.write("0")
                    o_c_v.write(
                        " " if count + 1 == len(ee_clif_offsets) else ", "
                    )
                else:
                    if "_ACT" in str(e):
                        # For active mode, CLIF_ANA_TX_AMPLITUDE_REG should not be masked.
                        active_mode = True
                    else:
                        active_mode = False
                    o_c_v.write("    \\\n    /* Mode: %s */ \\\n    " % (str(e)))
            o_c_v.write("\\\n}\n\n")

    def _pretty_xml_etree(self, elem):
        """Return a pretty-printed XML string for the Element."""
        rough_string = ET.tostring(elem, 'utf-8')
        reparsed = minidom.parseString(rough_string)
        return reparsed.toprettyxml(indent = "    ").replace('<?xml version="1.0" ?>', "")


    def run(self):
        (TM_TX_KEYS, TM_RX_KEYS, IM_TX_KEYS, IM_RX_KEYS) = getKeys()

        for k in IM_TX_KEYS + IM_RX_KEYS:
            self.d_im_values[k] = []

        for k in TM_TX_KEYS + TM_RX_KEYS:
            self.d_tm_values[k] = []

        self._parse_the_csv(self.o_im_csv, IM_TX_KEYS, IM_RX_KEYS, self.d_im_values)
        self._parse_the_csv(self.o_tm_csv, TM_TX_KEYS, TM_RX_KEYS, self.d_tm_values)
        a_version_info = self._parse_version_info_csv(self.o_ver_csv)

        self.xml_page = ET.Element("page", name = "phRFLP")
        xml_page_desc = ET.SubElement(self.xml_page, "description")
        local_time = time.localtime()
        datetime_str = time.strftime("%Y-%m-%d", local_time)
        xml_page_desc.text = "Values for RF Load Protocol based on:\n\n<pre>\n%s\n</pre>\n\n    Generated from clifcsv_to_xml.py version %s on %s"%\
            ("\n    ".join(a_version_info),__VERSION__,datetime_str)

        self._dump_values(self.o_xml, self.o_c_c, self.o_c_v, TM_TX_KEYS, self.d_tm_values, "T_Tx", "CLIF Target Mode - Transmit")
        self._dump_values(self.o_xml, self.o_c_c, self.o_c_v, TM_RX_KEYS, self.d_tm_values, "T_Rx", "CLIF Target Mode - Receive")
        self._dump_values(self.o_xml, self.o_c_c, self.o_c_v, IM_TX_KEYS, self.d_im_values, "I_Tx", "CLIF Initiator Mode - Transmit")
        self._dump_values(self.o_xml, self.o_c_c, self.o_c_v, IM_RX_KEYS, self.d_im_values, "I_Rx", "CLIF Initiator Mode - Receive")

        self.o_xml.write(self._pretty_xml_etree(self.xml_page))

        self._CloseFiles()

def getKeys():
    IM_KEYS = "I_TX,I_RX_PASS,I_TX_A,I_RX_A_P,I_TX_A_106,I_RX_A_106_P,I_TX_A_212,I_RX_A_212,I_TX_A_424,I_RX_A_424,I_TX_A_848,I_RX_A_848,I_TX_B,I_RX_B,I_TX_B_106,I_RX_B_106,I_TX_B_212,I_RX_B_212,I_TX_B_424,I_RX_B_424,I_TX_B_848,I_RX_B_848,I_TX_F,I_RX_F_P,I_TX_F_212,I_RX_F_212_P,I_TX_F_424,I_RX_F_424_P,I_TX_15693,I_RX_15693,I_TX_15693_100,I_RX_15693_26,I_TX_15693_10,I_RX_15693_53,I_TX_EPCV2,I_RX_EPCV2,I_TX_EPCV2_TARI_9_44,I_TX_EPCV2_TARI_18_88,I_RX_EPCV2_SC424_2MP,I_RX_EPCV2_SC424_4MP,I_RX_EPCV2_SC848_2MP,I_RX_EPCV2_SC848_4MP,I_TX_ACT,I_RX_ACT,I_TX_ACT_106,I_RX_ACT_106,I_TX_ACT_212,I_RX_ACT_212,I_TX_ACT_424,I_RX_ACT_424,I_TX_BOOT,I_RX_BOOT".split(",")
    TM_KEYS = "T_TX_GTM,T_RX_GTM,T_TX_A_P,T_RX_A,T_TX_A_106_P,T_RX_A_106,T_TX_A_212,T_RX_A_212,T_TX_A_424,T_RX_A_424,T_TX_A_848,T_RX_A_848,T_TX_F_P,T_RX_F,T_TX_F_212_P,T_RX_F_212,T_TX_F_424_P,T_RX_F_424,T_TX_ACT,T_RX_ACT,T_TX_A_106_ACT,T_RX_A_106_ACT,T_TX_F_212_ACT,T_RX_F_212_ACT,T_TX_F_424_ACT,T_RX_F_424_ACT,T_TX_B,T_RX_B,T_TX_B_106,T_RX_B_106,T_TX_B_212,T_RX_B_212,T_TX_B_424,T_RX_B_424,T_TX_B_848,T_RX_B_848".split(",")

    IM_TX_KEYS = []
    IM_RX_KEYS = []
    TM_TX_KEYS = []
    TM_RX_KEYS = []

    for k in IM_KEYS:
        if k.startswith("I_TX"):
            IM_TX_KEYS.append(k)
        else:
            IM_RX_KEYS.append(k)

    for k in TM_KEYS:
        if k.startswith("T_TX_"):
            TM_TX_KEYS.append(k)
        else:
            TM_RX_KEYS.append(k)

    return (TM_TX_KEYS, TM_RX_KEYS, IM_TX_KEYS, IM_RX_KEYS)

CLIF_REG_START_ADDRESS = 0x40004000

CLIF_REGS = {
    "CLIF_CONTROL_REG":(0x40004000),
    "CLIF_CLOCK_CONTROL_REG":(0x40004004),
    "CLIF_STATUS_REG":(0x40004008),
    "CLIF_TRANSCEIVE_CONTROL_REG":(0x4000400C),
    "CLIF_TX_WAIT_REG":(0x40004010),
    "CLIF_TX_TIMESLOT_SEL_REG":(0x40004014),
    "CLIF_RX_WAIT_REG":(0x40004018),
    "CLIF_TX_WATERLEVEL_REG":(0x40004024),
    "CLIF_RX_WATERLEVEL_REG":(0x40004028),
    "CLIF_RF_CONTROL_REG":(0x4000402C),
    "CLIF_TX_DATA_CONFIG_REG":(0x40004030),
    "CLIF_TX_DATA_MOD_REG":(0x40004034),
    "CLIF_TX_FRAME_CONFIG_REG":(0x40004038),
    "CLIF_TX_SYMBOL_CONFIG_REG":(0x4000403C),
    "CLIF_TX_SYMBOL0_DEF_REG":(0x40004040),
    "CLIF_TX_SYMBOL1_DEF_REG":(0x40004044),
    "CLIF_TX_SYMBOL23_DEF_REG":(0x40004048),
    "CLIF_TX_SYMBOL01_MOD_REG":(0x4000404C),
    "CLIF_TX_SYMBOL23_MOD_REG":(0x40004050),
    "CLIF_TX_OVERSHOOT_CONFIG_REG":(0x40004054),
    "CLIF_TX_UNDERSHOOT_CONFIG_REG":(0x40004058),
    "CLIF_RX_CONFIG_REG":(0x4000405C),
    "CLIF_RX_STATUS_REG":(0x40004060),
    "CLIF_NFCWI_REG":(0x40004068),
    "CLIF_CRC_RX_CONFIG_REG":(0x4000406C),
    "CLIF_CRC_TX_CONFIG_REG":(0x40004070),
    "CLIF_CRC_DATA_REG":(0x40004074),
    "CLIF_TIMER0_CONFIG_REG":(0x40004078),
    "CLIF_TIMER1_CONFIG_REG":(0x4000407C),
    "CLIF_TIMER2_CONFIG_REG":(0x40004080),
    "CLIF_TIMER0_RELOAD_REG":(0x40004084),
    "CLIF_TIMER1_RELOAD_REG":(0x40004088),
    "CLIF_TIMER2_RELOAD_REG":(0x4000408C),
    "CLIF_TIMER0_OUTPUT_REG":(0x40004090),
    "CLIF_TIMER1_OUTPUT_REG":(0x40004094),
    "CLIF_TIMER2_OUTPUT_REG":(0x40004098),
    "CLIF_TIMER3_CONFIG_REG":(0x4000409C),
    "CLIF_SIGPRO_CONFIG_REG":(0x400040AC),
    "CLIF_SIGPRO_RM_CONFIG0_REG":(0x400040B0),
    "CLIF_SIGPRO_RM_CONFIG1_REG":(0x400040B4),
    "CLIF_SIGPRO_CM_CONFIG_REG":(0x400040B8),
    "CLIF_SIGPRO_ADCBCM_CONFIG_REG":(0x400040BC),
    "CLIF_SIGPRO_ADCBCM_THRESHOLD_REG":(0x400040C0),
    "CLIF_SIGPRO_ADCBCM_BITENERGY0_REG":(0x400040C4),
    "CLIF_SIGPRO_ADCBCM_BITENERGY1_REG":(0x400040C8),
    "CLIF_AGC_CONFIG0_REG":(0x400040CC),
    "CLIF_AGC_CONFIG1_REG":(0x400040D0),
    "CLIF_AGC_INPUT_REG":(0x400040D4),
    "CLIF_AGC_VALUE_REG":(0x400040D8),
    "CLIF_TX_CONTROL_REG":(0x400040DC),
    "CLIF_TBCONTROL_REG":(0x400040E0),
    "CLIF_DACCONTROL_REG":(0x400040E4),
    "CLIF_TBVALUE_REG":(0x400040E8),
    "CLIF_RX_DATA_BUFFER_REG":(0x400040EC),
    "CLIF_TEST_CONTROL_REG":(0x400040FC),
    "CLIF_ANA_NFCLD_REG":(0x40004100),
    "CLIF_ANA_TX_CLK_CONTROL_REG":(0x40004104),

    "CLIF_ANA_TX_AMPLITUDE_REG":(0x40004108),

    "CLIF_ANA_PBF_CONTROL_REG":(0x4000410C),
    "CLIF_ANA_RX_REG":(0x40004110),
    "CLIF_ANA_CM_CONFIG_REG":(0x40004114),
    "CLIF_ANA_CMPLUS_B_REG":(0x40004118),
    "CLIF_ANA_AGC_REG":(0x4000411C),
    "CLIF_ANA_CLK_MAN_REG":(0x40004120),
    "CLIF_ANA_TESTBUS_SELECT_REG":(0x40004124),
    "CLIF_ANA_TX_SHAPE_CONTROL_REG":(0x40004128),
    "CLIF_ANA_TESTBUS_SELECT2_REG":(0x4000412C),
    "CLIF_ANA_TEST_REG":(0x400041FC),
    "CLIF_SPARE_REG":(0x40004200),
    "CLIF_DPLL_CONTROL_REG":(0x40004204),
    "CLIF_DPLL_INIT_REG":(0x40004208),
    "CLIF_DPLL_GEAR_REG":(0x4000420C),
    "CLIF_DPLL_INIT_FREQ_REG":(0x40004210),
    "CLIF_BBA_CONTROL_REG":(0x40004214),
    "CLIF_INT_CLR_ENABLE_REG":(0x40007FD8),
    "CLIF_INT_SET_ENABLE_REG":(0x40007FDC),
    "CLIF_INT_STATUS_REG":(0x40007FE0),
    "CLIF_INT_ENABLE_REG":(0x40007FE4),
    "CLIF_INT_CLR_STATUS_REG":(0x40007FE8),
    "CLIF_INT_SET_STATUS_REG":(0x40007FEC),

}


def main():
    o = ClifCSVToXml(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
    o.run()

if __name__ == "__main__":
    main()
