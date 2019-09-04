'''

                     Copyright (c), NXP Semiconductors

                        (C) NXP Semiconductors 2014,2015

          All rights are reserved. Reproduction in whole or in part is
         prohibited without the written consent of the copyright owner.
     NXP reserves the right to make changes without notice at any time.
    NXP makes no warranty, expressed, implied or statutory, including but
    not limited to any implied warranty of merchantability or fitness for any
   particular purpose, or that the use will not infringe any third party patent,
    copyright or trademark. NXP must not be liable for any loss or damage
                             arising from its use.

'''

import sys
import xml.dom.minidom
import math
import binascii
import time
import re
import os
from pprint import pprint

__VERSION__ = '2016.06.29_00'

#
#
#   2016.07.04_00  : PGh : Using macro name NXPBUILD__CFG_ENABLE_EE_DOWNLOAD
#   2016.06.29_00  : PGh : Create EEPROM Layout at build time
#   2015.09.11_01  : PGh : QAC Fixes for more than 127 Enums
#   2015.09.11_00  : PGh : In the generated XML, show the Address
#   2015.09.10_00  : PGh : Add version number
#   2015.07.16_01  : PGh : Fixed 0x0x in _PF.c file
#   2015.06.23_01  : PGh : Disabled Logging
#   2015.06.23_00  : KRa : Updated with a fix to read array from xml by ignoring '0x' 
#   2015.04.14_00  : PGh : Updating logic for generation of PF.c file
#   2015.04.07_00  : PGh : Remove Debug printing. Show version number/name in the generated file
#   2015.04.02_00  : KRa : Fixing alignment
#   2015.01.31_00  : PGh : Stricter about 120 bytes end of line
#   2015.01.20_00  : PGh : Create more doxygen compliant CFG File.
#   2015.01.01_00  : PGh : [PT_SC2440] Merge 2014.11.03_00 to trunk.
#   2014.11.03_00  : PGh : [PT_SC2063] Do not use magic numbers for phCfg_EE_.c
#                        : Use the #defines already calculated in the XML Folder.
#   Before         : KRa : Initial revision
DEBUG = False

PREFIX = "phCfg_EE_"
PREFIX_PF = "phCfg_PF_"
PREFIX_BUILD = "phCfg_EE_Build_"
PREFIX_UC = "PH_CFG_EE_"

class Page:
    NAME = 'name'
    START_ADD = 'StartAddress'
    FREE_SIZE = 'FreeSize'
    TOTAL_SIZE = 'TotalSize'
    END_ADD = 'EndAddress'
    STRUCTURES = 'Structures'
    DESCRIPTION = 'Description'

class Structure:
    NAME = 'name'
    FIELDS = 'Fields'
    TOTAL_SIZE = 'Total_size'
    START_ADD = 'StartAddress'
    END_ADD = 'EndAddress'
    DESCRIPTION = 'Description'

class Field:
    NAME = 'name'
    SIZE = 'size'
    TYPE = 'type'
    VALUE = 'value'
    ADDRESS = 'address'
    DESCRIPTION = 'Description'
    OPTIONS = 'Options'

class Option:
    DESCRIPTION = 'Description'
    VALUE = 'Value'

class UserEEToBinary(object):

    def __init__(self, in_path, out_dir):
        self.SOURCE_PATH = in_path
        self.HEADER_FILE_PATH = os.path.join(out_dir, PREFIX[:-1] + ".h")
        self.SOURCE_EE_FILE_PATH = os.path.join(out_dir, PREFIX[:-1] + ".c")
        self.SOURCE_PF_FILE_PATH = os.path.join(out_dir, PREFIX_PF[:-1] + ".c")
        self.SOURCE_BUILD_FILE_PATH = os.path.join(out_dir, PREFIX_BUILD[:-1] + ".c")
        self.BIN_TEXT_PATH = os.path.join(out_dir, "output_xml.bin.txt")
        self.BIN_FILE_PATH = os.path.join(out_dir, "output_xml.bin")
        self.XML_WITH_SIZE_PATH = "_output_xml_sizes.xml"
        self._full_ee_startAddress = None

    def run(self):
        pages = self.parse_xmlfile(self.SOURCE_PATH)
        write_string = self.bin_val_calc(pages, self.BIN_TEXT_PATH)
        self.dump_header_file(pages, self.HEADER_FILE_PATH, self.SOURCE_PF_FILE_PATH, self.SOURCE_EE_FILE_PATH)
        self.dump_binary_file(write_string, self.BIN_FILE_PATH)
        self.dump_build_file(write_string, self.SOURCE_BUILD_FILE_PATH)

    def parse_xmlfile(self, SOURCE_PATH):
        pages = []
        print "Processing XML File from %s" % (SOURCE_PATH)
        o_input = open(SOURCE_PATH, "r")
        dom = xml.dom.minidom.parse(o_input)
        dom_useree = dom.getElementsByTagName("UserEE")

        dom_pages = dom.getElementsByTagName("page")

        startAddress = dom_useree[0].getAttribute('startAddress')
        self._full_ee_startAddress = startAddress
        endAddress = dom_useree[0].getAttribute('endAddress')
        product_name = dom_useree[0].getAttribute('product_name')
        product_version = dom_useree[0].getAttribute('product_version')

        sect_start_addr = struct_start_addr = this_field_addr = startAddress
        u8 = 1
        u16 = 2
        u32 = 4

        for dom_page in dom_pages:
            sectPad = 0
            this_page = {}
            pages.append(this_page)
            this_page[Page.NAME] = dom_page.getAttribute("name")
            this_page[Page.FREE_SIZE] = 0
            this_page[Page.TOTAL_SIZE] = 0
            this_page[Page.START_ADD] = sect_start_addr
            dom_page.setAttribute("startAddress", sect_start_addr)

            this_page[Page.STRUCTURES] = []
            this_page[Page.DESCRIPTION] = ''

            for child in dom_page.childNodes:
                if child.nodeType == child.ELEMENT_NODE and child.tagName == 'description' and child.parentNode == dom_page:
                    this_page[Page.DESCRIPTION] = str(child.firstChild.data)

            dom_structures = dom_page.getElementsByTagName("structure")
            for dom_structure in dom_structures:
                this_structure = {}
                this_page[Page.STRUCTURES].append(this_structure)
                structPad = 0
                this_structure[Structure.NAME] = dom_structure.getAttribute("name")
                this_structure[Structure.FIELDS] = []
                this_structure[Structure.DESCRIPTION] = ''
                this_structure[Structure.START_ADD] = struct_start_addr
                this_structure[Structure.TOTAL_SIZE] = 0
                dom_structure.setAttribute("startAddress", struct_start_addr)

                for child in dom_structure.childNodes:
                    if child.nodeType == child.ELEMENT_NODE and child.tagName == 'description':
                        this_structure[Structure.DESCRIPTION] = str(child.firstChild.data)

                dom_fields = dom_structure.childNodes
                for dom_field in dom_fields:
                    if dom_field.nodeType == dom_field.ELEMENT_NODE and dom_field.nodeName == 'field' or dom_field.nodeName == 'array':
                        this_field = {}
                        dom_field.setAttribute(Field.ADDRESS,this_field_addr)
                        this_field_addr, this_field, this_structure, this_page = self.field_addr_handle(this_page, this_structure, this_field, dom_field, this_field_addr, u8, u16, u32)

                    dom_options = dom_field.childNodes
                    for dom_option in dom_options:
                        if dom_option.nodeType == dom_option.ELEMENT_NODE and dom_option.nodeName == 'option':
                            this_option = {}
                            this_field[Field.OPTIONS].append(this_option)
                            this_option[Option.DESCRIPTION] = dom_option.getAttribute("description")
                            this_option[Option.VALUE] = dom_option.getAttribute("value")

                quot_value = math.fabs(this_structure[Structure.TOTAL_SIZE]) / 4
                quot_value = math.ceil(quot_value)
                struct_mod = this_structure[Structure.TOTAL_SIZE] % 4
                if(0 != struct_mod):
                    structPad = structPad + (4 - (this_structure[Structure.TOTAL_SIZE] % 4))

                struct_size = this_structure[Structure.TOTAL_SIZE] + structPad
                this_structure[Structure.END_ADD] = hex(int(this_structure[Structure.START_ADD], 16) + 4 * int(quot_value))
                dom_structure.setAttribute("totalSize", str(struct_size))

                struct_start_addr = this_structure[Structure.END_ADD]
                this_field_addr = struct_start_addr
                sectPad = sectPad + structPad

            sect_mod = ((sectPad + this_page[Page.TOTAL_SIZE]) % 64)
            if(0 != sect_mod):
                sectPad = 64 - ((sectPad + this_page[Page.TOTAL_SIZE]) % 64)

            sect_size = this_page[Page.TOTAL_SIZE] + sectPad
            quo_value = math.fabs(sect_size) / 64
            quo_value = math.ceil(quo_value)

            this_page[Page.END_ADD] = hex(int(sect_start_addr, 16) + 64 * int(quo_value))
            debug_end_addr = this_page[Page.END_ADD]

            dom_page.setAttribute("totalSize", str(sect_size))
            sect_start_addr = this_page[Page.END_ADD]

            this_field_addr = sect_start_addr
            struct_start_addr = sect_start_addr

        o_input.close()
        with open(self.XML_WITH_SIZE_PATH, "w") as oXml:
            oXml.write(dom.toxml("utf-8"))
        return pages


    def field_addr_handle(self, this_page, this_structure, this_field, dom_field, this_field_addr, u8, u16, u32):
        this_structure[Structure.FIELDS].append(this_field)
        field_name = dom_field.getAttribute("name")
        field_val_type = dom_field.getAttribute("valuetype")
        field_val = dom_field.getAttribute("value")
        if '0x' in field_val:
            field_val = field_val.replace("0x"," ").strip()
        field_type = dom_field.getAttribute("type")
        this_field[Field.NAME] = field_name
        this_field[Field.TYPE] = field_type
        this_field['ValueType'] = field_val_type
        this_field[Field.VALUE] = field_val
        this_field[Field.SIZE] = 0
        this_field[Field.DESCRIPTION] = ''
        this_field[Field.OPTIONS] = []
        if dom_field.getAttribute("size"):
            this_field[Field.SIZE] = dom_field.getAttribute("size")
        if this_field[Field.SIZE] == 0:
            if field_val_type == 'hex':
                this_field['ParsedValue'] = int('0x0' + field_val.replace(" ", "").replace("0x", ""), 16)
            elif field_val_type == 'int':
                this_field['ParsedValue'] = int(field_val)
        for child in dom_field.childNodes:
            if child.nodeType == child.ELEMENT_NODE and child.tagName == 'description':
                this_field[Field.DESCRIPTION] = str(child.firstChild.data).strip()

        this_field[Field.ADDRESS] = this_field_addr
        i_start_address = int(this_field_addr, 16)
        if this_field[Field.SIZE]:
            this_page, this_structure, this_field, this_field_addr, i_start_address = self.handle_field_size(this_page, this_structure, this_field, this_field_addr, i_start_address, u8, u16, u32)
        if this_field[Field.SIZE] == 0:
            this_page, this_structure, this_field, this_field_addr, i_start_address = self.handle_no_field_size(this_page, this_structure, this_field, this_field_addr, i_start_address, u8, u16, u32)
        return this_field_addr, this_field, this_structure, this_page


    def handle_field_size(self, this_page, this_structure, this_field, this_field_addr, i_start_address, u8, u16, u32):
        if this_field[Field.TYPE] == 'u8':
            this_page[Page.TOTAL_SIZE] = this_page[Page.TOTAL_SIZE] + u8 * int(this_field[Field.SIZE])
            this_structure[Structure.TOTAL_SIZE] = this_structure[Structure.TOTAL_SIZE] + u8 * int(this_field[Field.SIZE])
            nxtfield_addr = hex(i_start_address + u8 * int(this_field[Field.SIZE]))

        elif this_field[Field.TYPE] == 'u16':
            this_page[Page.TOTAL_SIZE] = this_page[Page.TOTAL_SIZE] + u16 * int(this_field[Field.SIZE])
            this_structure[Structure.TOTAL_SIZE] = this_structure[Structure.TOTAL_SIZE] + u16 * int(this_field[Field.SIZE])
            nxtfield_addr = hex(i_start_address + u16 * int(this_field[Field.SIZE]))
            if i_start_address & 0x1:
                print("ERROR: Can not start %s at %s" % (str(this_field[Field.NAME]), nxtfield_addr))
                raise Exception("Memory alignment issue. ")

        elif this_field[Field.TYPE] == 'u32':
            this_page[Page.TOTAL_SIZE] = this_page[Page.TOTAL_SIZE] + u32 * int(this_field[Field.SIZE])
            this_structure[Structure.TOTAL_SIZE] = this_structure[Structure.TOTAL_SIZE] + u32 * int(this_field[Field.SIZE])
            nxtfield_addr = hex(i_start_address + u32 * int(this_field[Field.SIZE]))
            if i_start_address & 0x3:
                print("ERROR: Can not start %s at %s" % (str(this_field[Field.NAME]), nxtfield_addr))
                raise Exception("Memory alignment issue. ")

        this_field_addr = nxtfield_addr
        return this_page, this_structure, this_field, this_field_addr, i_start_address


    def handle_no_field_size(self, this_page, this_structure, this_field, this_field_addr, i_start_address, u8, u16, u32):
        if this_field[Field.TYPE] == 'u8':
            this_page[Page.TOTAL_SIZE] = this_page[Page.TOTAL_SIZE] + u8
            this_structure[Structure.TOTAL_SIZE] = this_structure[Structure.TOTAL_SIZE] + u8
            nxtfield_addr = hex(i_start_address + u8)

        elif this_field[Field.TYPE] == 'u16':
            this_page[Page.TOTAL_SIZE] = this_page[Page.TOTAL_SIZE] + u16
            this_structure[Structure.TOTAL_SIZE] = this_structure[Structure.TOTAL_SIZE] + u16
            nxtfield_addr = hex(i_start_address + u16)
            if i_start_address & 0x1:
                print("ERROR: Can not start %s at %s" % (str(this_field[Field.NAME]), nxtfield_addr))
                raise Exception("Memory alignment issue. ")

        elif this_field[Field.TYPE] == 'u32':
            this_page[Page.TOTAL_SIZE] = this_page[Page.TOTAL_SIZE] + u32
            this_structure[Structure.TOTAL_SIZE] = this_structure[Structure.TOTAL_SIZE] + u32
            nxtfield_addr = hex(i_start_address + u32)
            if i_start_address & 0x3:
                print("ERROR: Can not start %s at %s" % (str(this_field[Field.NAME]), nxtfield_addr))
                raise Exception("Memory alignment issue. ")
        this_field_addr = nxtfield_addr
        return this_page, this_structure, this_field, this_field_addr, i_start_address


    def bin_val_calc(self, pages, BIN_TEXT_PATH):
        dump_values = []
        for page in pages:
            for structure in page['Structures']:
                for field in structure['Fields']:
                    if field['type'] == 'u8':
                        if field['ValueType'] == 'hex':
                            hex_value = field['value'].replace(" ", "")
                            hex_value = self.u8_handle(hex_value, field['size'])
                            dump_values.append(hex_value)

                        if field['ValueType'] == 'int':
                            hex_value = hex(int(field['value'])).replace(" ", "")
                            hex_value = self.u8_handle(hex_value, field['size'])
                            dump_values.append(hex_value)

                        if field['ValueType'] == 'cstring':
                            hex_value = field['value'].replace(" ", "")
                            hex_value = self.u8_cstring_handle(hex_value, field['size'])
                            dump_values.append(hex_value)

                    if field['type'] == 'u16':
                        if field['ValueType'] == 'hex':
                            hex_value = field['value'].replace(" ", "")
                            dump_values = self.u16_handle(hex_value, dump_values, field['size'])

                        if field['ValueType'] == 'int':
                            hex_value = hex(int(field['value'])).replace(" ", "")
                            dump_values = self.u16_handle(hex_value, dump_values, field['size'])

                        if field['ValueType'] == 'cstring':
                            hex_value = field['value'].replace(" ", "")
                            dump_values = self.u16_ctring_handle()

                    if field['type'] == 'u32':
                        if field['ValueType'] == 'hex':
                            hex_value = field['value'].replace(" ", "")
                            dump_values = self.u32_handle(hex_value, dump_values, field['size'])

                        if field['ValueType'] == 'int':
                            hex_value = hex(int(field['value'])).replace(" ", "")
                            dump_values = self.u32_handle(hex_value, dump_values, field['size'])

                        if field['ValueType'] == 'cstring':
                            hex_value = field['value'].replace(" ", "")
                            dump_values = self.u32_ctring_handle()

                struct_values = str(dump_values).replace("'", "").replace("[", "").replace("]", "").replace(",", "").replace("u", "").replace('"', "").replace(' ', "")
                struct_pad_value = '00'
                no_of_x = 4 - ((len(struct_values) / 2) % 4)
                if (0 != (no_of_x % 4)):
                    for count in range(no_of_x):
                        dump_values.append(struct_pad_value)

            sector_values = str(dump_values).replace("'", "").replace("[", "").replace("]", "").replace(",", "").replace("u", "").replace('"', "").replace(' ', "")
            sect_pad_value = '00'
            no_of_y = 64 - ((len(sector_values) / 2) % 64)

            if (0 != (no_of_y % 64)):
                for count in range(no_of_y):
                    dump_values.append(sect_pad_value)

        dump_str = str(dump_values)
        output_str = ''
        for each_char in dump_str:
            output_str = output_str + each_char.replace("'", "").replace("[", "").replace("]", "").replace(",", "").replace("u", "").replace('"', "").replace(' ', "")

        self.format_write(output_str, BIN_TEXT_PATH)
        return output_str


    def u8_handle(self, hex_value, field_size):
        hex_value = hex_value.replace(" ", "").replace("0x","")
        if '0x' in hex_value:
            hex_value = hex_value[2:]

        if len(hex_value) > 2 and len(hex_value) > (int(field_size) * 2):
            print("Error processing u8: hex_value=%s field_size=%s"%(str(hex_value), hex(field_size)))
            raise Exception("FIELD SIZE INCOMPATIBLE")

        if len(hex_value) < 2:
            pad_val = 2
        else:
            pad_val = (len(hex_value) % 2) * 2
        hex_value = hex_value.zfill(pad_val)

        if field_size:
            no_of_zeroes = (int(field_size) - len(hex_value)) * 2
            zeroes = '0'
            for count in range(no_of_zeroes):
                hex_value = str(hex_value) + zeroes
        return hex_value


    def u8_cstring_handle(self, hex_value, field_size):
        hex_value = hex_value.replace(" ", "")
        cstring_list = []
        if len(hex_value) > 2 and len(hex_value) > (int(field_size)):
            print("Error processing cstring: hex_value=%s field_size=%s"%(str(hex_value), hex(field_size)))
            raise Exception("FIELD SIZE INCOMPATIBLE")
        for ch in hex_value:
            ascii_val = ord(ch)
            hex_value = hex(ascii_val)
            hex_value = hex_value[2:]
            hex_value = hex_value.zfill(2)
            cstring_list.append(hex_value)
        hex_value = cstring_list
        if field_size:
            no_of_zeroes = (int(field_size) - len(hex_value)) * 2
            zeroes = '0'
            for count in range(no_of_zeroes):
                hex_value = str(hex_value) + zeroes
        return hex_value


    def u16_handle(self, hex_value, dump_values, field_size):
        hex_value = hex_value.replace(" ", "").replace("0x","")
        if '0x' in hex_value:
            hex_value = hex_value[2:]
        if len(hex_value) > 4 and len(hex_value) > (int(field_size) * 4):
            print("Error processing u16: hex_value=%s field_size=%s"%(str(hex_value), hex(field_size)))
            raise Exception("FIELD SIZE INCOMPATIBLE")
        if len(hex_value) < 4:
            pad_val = 4
        else:
            pad_val = (len(hex_value) % 4) * 4
        hex_value = hex_value.zfill(pad_val)

        hex_array4 = re.findall('....', hex_value)

        for val in hex_array4:
            half = len(val) / 2
            first_half = val[:half]
            second_half = val[half:]
            hex_value_rev = second_half + first_half
            dump_values.append(hex_value_rev)

        if field_size:
            no_of_zeroes = (int(field_size) - len(hex_value)) * 2
            zeroes = '0'
            for count in range(no_of_zeroes):
                dump_values.append(zeroes)

        return dump_values


    def u16_ctring_handle(self, hex_value, dump_values, field_size):
        raise Exception("CString not allowed with 16 bit datatype")

    def u32_handle(self, hex_value, dump_values, field_size):
        hex_value = hex_value.replace(" ", "").replace("0x","")
        if '0x' in hex_value:
            hex_value = hex_value[2:]
        if len(hex_value) > 8 and len(hex_value) > (int(field_size) * 8):
            print("Error processing u32: hex_value=%s field_size=%s"%(str(hex_value), hex(field_size)))
            raise Exception("FIELD SIZE INCOMPATIBLE")
        if len(hex_value) < 8:
            pad_val = 8
        else:
            pad_val = (len(hex_value) % 8) * 8
        hex_value = hex_value.zfill(pad_val)

        hex_array8 = re.findall('........', hex_value)

        for val in hex_array8:
            half = len(val) / 2
            first_half = val[:half]
            second_half = val[half:]
            half_inner = len(first_half) / 2
            first_half_inner = first_half[:half_inner]
            second_half_inner = first_half[half_inner:]
            half_outer = len(second_half) / 2
            first_half_outer = second_half[:half_outer]
            second_half_outer = second_half[half_outer:]

            hex_value_rev = second_half_outer + first_half_outer + second_half_inner + first_half_inner
            dump_values.append(hex_value_rev)

        if field_size:
            no_of_zeroes = (int(field_size) - len(hex_value)) * 2
            zeroes = '0'
            for count in range(no_of_zeroes):
                dump_values.append(zeroes)

        return dump_values


    def u32_ctring_handle(self, hex_value, dump_values, field_size):
        raise Exception("CString not allowed with 32 bit datatype")


    def dump_binary_file(self, write_string, BIN_FILE_PATH):
        data = binascii.a2b_hex(write_string)
        print "Writing Binary File to %s" % (BIN_FILE_PATH)
        o_write = open(BIN_FILE_PATH, "wb")
        o_write.write(data)
        o_write.close()


    def dump_build_file(self, write_string, BUILD_FILE_PATH):
        """ this file would be part of build system to write EE Directly """
        print "Writing Build File to %s" % (BUILD_FILE_PATH)
        o_write = open(BUILD_FILE_PATH, "wb")
        o_write.write(NXP_COPYRIGHT_HEADER)
        o_write.write(BUILD_FILE_PATH_HEADER)
        ee_add = int(self._full_ee_startAddress,16)
        o_write.write("/* %s */ "%(hex(ee_add),))
        for i in range(len(write_string)/2):
            o_write.write("0x")
            o_write.write(write_string[2*i])
            o_write.write(write_string[2*i+1])
            if i+1 != len(write_string)/2:
                o_write.write(", ")
            if ( 0 == (i+1) % 16):
                o_write.write("\n")
                o_write.write("/* %s */ "%(hex(ee_add + i + 1),))
            elif ( 0 == (i+1) % 8):
                o_write.write("    ")
            elif ( 0 == (i+1) % 4):
                o_write.write("  ")
        o_write.write(BUILD_FILE_PATH_FOOTER)
        o_write.close()

    def format_write(self, output_str, BIN_TEXT_PATH):
        o_bin_output = open(BIN_TEXT_PATH, "w")
        format_count = 1
        formatted_str = ''

        for char in output_str:
            if format_count % 2 == 0:
                formatted_str = formatted_str + char + ' '
            else:
                formatted_str = formatted_str + char
            if format_count % 32 == 0:
                formatted_str = ("%s\n" % (formatted_str))
            format_count = format_count + 1

        o_bin_output.write(formatted_str)

    def write_description(self, o_output, input_string, prefix = ' '):
        """
        Write description.
        """
        s_desc = input_string
        if len(s_desc.strip()) > 0:
            if "\n" in s_desc:
                for l in s_desc.split("\n"):
                    if len(l.strip()) > 0:
                        o_output.write("%s* %s\n" % (prefix, l.rstrip()))
                    else:
                        o_output.write("%s*\n" % (prefix,))
            else:
                o_output.write("%s* %s\n" % (prefix, s_desc.strip()))

    def write_version_information(self,o_h_output,ver_info_structure):
        for field in ver_info_structure['Fields']:
            if 'ParsedValue' in field:
                o_h_output.write("\n/** %s */"%field['Description'])
                o_h_output.write("\n#define %sVER_%s   (%d)"%(PREFIX_UC, field['name'].upper(), field['ParsedValue'] ))
        o_h_output.write("\n\n")

    def dump_header_file(self, pages, HEADER_FILE_PATH, SOURCE_PF_FILE_PATH, SOURCE_EE_FILE_PATH):
        o_h_output = open(HEADER_FILE_PATH, "wb")
        o_ee_output = open(SOURCE_EE_FILE_PATH, "wb")
        o_pf_output = open(SOURCE_PF_FILE_PATH, "wb")
        local_time = time.localtime()
        datetime_str = time.strftime("%Y-%m-%d", local_time)
        for f in (o_h_output, o_ee_output, o_pf_output):
            f.write(NXP_COPYRIGHT_HEADER)
            f.write("\n/* %s */\n" % ('*' * 114))
            f.write("/* Generated by user_ee.py version %s on %s\n */\n\n" % (__VERSION__,datetime_str))
        o_h_output.write("#ifndef " + PREFIX.upper() + "H\n")
        o_h_output.write("#define " + PREFIX.upper() + "H\n\n")
        o_h_output.write("/* PRQA S 0639 ++ */\n")
        for f in (o_h_output, o_ee_output, o_pf_output):
            f.write('#include "ph_Datatypes.h"\n')
        for f in (o_ee_output, o_pf_output):
            f.write("#include \"ph_NxpBuild.h\"\n")
            f.write("#include \"%s.h\"\n\n" % (PREFIX[:-1]))
        o_ee_output.write("\n#if defined(NXPBUILD__CFG_FROM_EE) || defined (NXPBUILD__CFG_ENABLE_EE_DOWNLOAD)\n\n")
        o_pf_output.write("\n#ifdef NXPBUILD__CFG_FROM_PF\n\n")
        
        o_h_output.write("\n#ifdef NXPBUILD__CFG_ENABLE_EE_DOWNLOAD")
        o_h_output.write("\n    extern const uint8_t %sValues[];"%(PREFIX_BUILD,))
        o_h_output.write("\n#endif /* NXPBUILD__CFG_ENABLE_EE_DOWNLOAD*/\n\n")

        if DEBUG:
            print(pages)
            
        for page in pages:
            already_written_structures = set()
            if page['Description']:
                for f in (o_h_output, o_pf_output):
                    f.write("\n/* %s */\n" % ('*' * 114))
                    f.write("/** \\name Page: %s\n * %s\n" % (page[Page.NAME], page[Page.DESCRIPTION].rstrip()))
                    f.write(" */\n\n")

            for structure in page['Structures']:
                o_h_output.write("\n")
                structure_sub_name = None
                structure_name = structure[Structure.NAME]
                if structure_name == 'VerInfo':
                    self.write_version_information(o_h_output,structure)

                struct_type_name = "%s%s_%s" % (PREFIX, page['name'], structure_name)

                if '::' in structure['name']:
                    for f in (o_h_output, o_pf_output):
                        f.write("/* Type of %s */\n" % struct_type_name)
                    start_address_define_name = "%s%s_%s_START_ADDRESS" % (PREFIX_UC, structure_name.upper(), structure_sub_name.upper())
                    o_h_output.write("#define %s (%s)\n" % (start_address_define_name, structure[Structure.START_ADD]))
                else:
                    start_address_define_name = "%s%s_START_ADDRESS" % (PREFIX_UC, structure_name.upper())
                    o_h_output.write("#define %s (%s)\n" % (start_address_define_name, structure[Structure.START_ADD]))

                if '::' in structure_name:
                    structure_name, structure_sub_name = structure['name'].split("::")
                    for f in (o_h_output, o_pf_output):
                        f.write("/** %s */\n" % (structure_sub_name))
                else:
                    for f in (o_h_output, o_pf_output):
                        f.write("/** Structure: %s\n" % (structure_name))
                        self.write_description(f, structure[Structure.DESCRIPTION])
                        f.write(" */\n")

                if structure_name in already_written_structures:
                    if '::' in structure['name']:
                        pass
                    else:
                        # o_h_output.write("#define %s%s_START_ADDRESS (%s)\n" % (PREFIX_UC, structure_name.upper(), structure['StartAddress']))
                        raise Exception("Structure Already Present")
                    o_h_output.write("/* Same as %s */\n\n" % structure_name)
                else:
                    print("Info: Writing " + structure_name)

                    o_h_output.write("typedef PH_PACK_STRUCT_BEGIN struct %s\n{\n" % (struct_type_name))
                    o_pf_output.write("static const %s_t gk%s_PF = {\n" % (struct_type_name, struct_type_name))
                    for field in structure['Fields']:
                        val_type = field['ValueType']
                        field_description = field['Description']
                        for f in (o_h_output, o_pf_output):
                            f.write("    /**\n")
                            if field_description:
                                self.write_description(f, field_description, '     ')
                            for option in field['Options']:
                                f.write("     * - %s => %s\n" % (option[Option.VALUE], option[Option.DESCRIPTION].rstrip()))
                            f.write("     */\n")

                        if field['type'] == 'u8': data_type = "uint8_t"
                        elif field['type'] == 'u16': data_type = "uint16_t"
                        elif field['type'] == 'u32': data_type = "uint32_t"
                        if val_type == 'hex' or val_type == 'int' or val_type == 'cstring':
                            if field['size']:
                                print_text = "    %s %s[%s];" % (data_type, field['name'], field['size'])
                                str_spacing = 76 - len(print_text)
                                if str_spacing < 1 or str_spacing > 60:
                                    str_spacing = 1
                                str_spacing = ' ' * str_spacing
                                default_value_cmnt = "/* default value = %s(%s) */" % (field['value'], val_type)
                                if len(default_value_cmnt) + len(print_text) > 80:
                                    str_spacing = "\n    "
                                o_h_output.write(print_text + "%s%s\n" % (str_spacing, default_value_cmnt))
                                if val_type == 'cstring':
                                    o_pf_output.write("    .%s = \"%s\",\n" % (field['name'], field['value']))
                                else:
                                    o_pf_output.write("    .%s = {\n" % (field['name']))
                                    for value in field['value'].split():
                                        val_type_prefx = '0x' if val_type == 'hex' else ''
                                        o_pf_output.write("        %s%s,\n" % (val_type_prefx, value))
                                    o_pf_output.write("    },\n")
                            else :
                                print_text = "    %s %s;" % (data_type, field['name'])
                                str_spacing = 75 - len(print_text)
                                if str_spacing < 1 or str_spacing > 60:
                                    str_spacing = 1
                                str_spacing = ' ' * str_spacing
                                o_h_output.write(print_text + " %s/* %s(%s) */\n" % (str_spacing, field['value'], val_type))
                                str_value = '0x%s' % (field['value']) if val_type == 'hex' else int(field['ParsedValue'])
                                o_pf_output.write('    .%s = %s,\n' % (field['name'], str_value))


                    o_h_output.write("} PH_PACK_STRUCT_END %s_t, * p%s_t;\n\n" % (\
                                   struct_type_name, struct_type_name))
                    o_pf_output.write("\n};\n")
                if structure_sub_name:
                    o_h_output.write("extern const %s_t * const gpk%s_%s;\n" % (
                                   struct_type_name, struct_type_name, structure_sub_name))
                    o_ee_output.write("const %s_t * const gpk%s_%s =\n    ((%s_t *) %s );\n" % (
                               struct_type_name, struct_type_name, structure_sub_name, struct_type_name, start_address_define_name))
                    raise Exception("Not Handled")
                else:
                    o_ee_output.write("const %s_t * const gpk%s =\n    ((%s_t *) %s );\n" % (\
                               struct_type_name, struct_type_name, struct_type_name, start_address_define_name))
                    o_pf_output.write("const %s_t * const gpk%s = &gk%s_PF;\n\n" % (\
                               struct_type_name, struct_type_name, struct_type_name))
                    o_h_output.write("extern const %s_t * const gpk%s;\n" % (struct_type_name, struct_type_name))
                already_written_structures.add(structure_name)
        o_h_output.write("/* PRQA S 0639 -- */\n")
        o_h_output.write("#endif /* " + PREFIX.upper() + "H */\n")
        o_ee_output.write("\n#endif /* NXPBUILD__CFG_FROM_EE */\n")
        o_pf_output.write("\n#endif /* NXPBUILD__CFG_FROM_PF */\n")
        for f in (o_h_output, o_ee_output, o_pf_output):
            f.close()

def usage():
    print ("Error: Not Enough Arguements:")
    print ("    %s <InputFile> <OutputDirectory>" % (sys.argv[0],))

NXP_COPYRIGHT_HEADER = r"""/*
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
 * Run Time configuration settings extracted from EEPROM/FLASH.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-07-04 23:48:03 +0530 (Mon, 04 Jul 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18541 $
 */

"""

BUILD_FILE_PATH_HEADER = """

#include <ph_Datatypes.h>
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__CFG_ENABLE_EE_DOWNLOAD

#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR
    #pragma location = ".eeprom"
#endif
#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC
#   error Not available with GCC.
#endif
#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5
    __attribute__((at(0x00201200)))
#endif

const uint8_t %sValues[3584]  = {

"""%(PREFIX_BUILD,)

BUILD_FILE_PATH_FOOTER = """

}; /* %sValues */

#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR
    #pragma keep_definition=%sValues
#endif

#endif /* NXPBUILD__CFG_ENABLE_EE_DOWNLOAD */
"""%(PREFIX_BUILD,PREFIX_BUILD)

if __name__ == '__main__':
    if len(sys.argv) == 3:
        obj = UserEEToBinary(sys.argv[1], sys.argv[2])
        obj.run()
    else:
        usage()

