<?xml version="1.0" encoding="UTF-8"?>
<!--


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

-->

<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xi="http://www.w3.org/2001/XInclude">

    <xsl:strip-space elements="*" />
    <xsl:output  indent="no" omit-xml-declaration="yes"/>
    <xsl:preserve-space elements="description"/>

    <xsl:template match="/">
        <html>
            <head>
                <link rel="stylesheet" type="text/css" href="user_ee.css" ></link>
                <title>
                    User EEPROM -
                    <xsl:value-of select="/UserEE/@product_name"/> -
                    <xsl:value-of select="/UserEE/@product_version" />
                </title>
            </head>
            <body>
                <xsl:apply-templates/>
            </body>
        </html>
    </xsl:template>

    <xsl:template match="/UserEE">
        <h2>
            <xsl:value-of select="@product_name"/> - <xsl:value-of select="@product_version" />
        </h2>
        <p>
            <b>EEPROM Range:</b> <xsl:value-of select="@startAddress" /> - <xsl:value-of select="@endAddress" />
        </p>
        <p>
            <b>Total Structures:</b>
            <xsl:value-of select="count(//structure)" />
        </p>
        <xsl:apply-templates  />
    </xsl:template>

    <xsl:template match="pages">
        <xsl:for-each select="page">
            <p />
            <xsl:call-template name="page" />
        </xsl:for-each>
    </xsl:template>
    <xsl:template match="page">
        <xsl:call-template name="page" />
    </xsl:template>

    <xsl:template name="structures">
        <xsl:for-each select="structure">
            <xsl:apply-templates />
        </xsl:for-each>
    </xsl:template>
    <xsl:template name="page">
        <h3 class="page">Page <xsl:number value="position()" format="I "/>
            [<xsl:value-of select="@name" />]
        </h3>
            <xsl:value-of select="description" disable-output-escaping="yes"/>
            <p class="pagedescription">Starting Address : <xsl:value-of select="@startAddress" /></p>
            <xsl:for-each select="structure">
                <p>
                    <xsl:call-template name="structure" />
                </p>
            </xsl:for-each>
    </xsl:template>

    <xsl:template name="structure">
        <br />
        <table class="structure">
            <thead>
                <tr>
                    <td>

                    </td>
                    <td>
                        Address
                    </td>
                    <td>
                        Type
                    </td>
                    <td>
                        Field Name
                    </td>
                    <td>
                        Default Value
                    </td>
                    <td>
                        Description
                    </td>
                </tr>
            </thead>
            <tr>
                <xsl:variable name="rspan_struct" select="1 + count(field) + count(array)" />
                <td class="structurename" rowspan="{$rspan_struct}" >
                    [Struct:<xsl:number value="position()" format="1] "/>
                    <br />
                    <h3>
                        <!-- <xsl:value-of select="../@name" /> _  -->
                        <xsl:value-of select="@name"/>
                    </h3>
                    <br />
                    <xsl:value-of select="@startAddress"/>
                </td>
                <td colspan="5" class="structuredescription" ><xsl:value-of select="description"  disable-output-escaping="yes" /></td>
            </tr>
            <xsl:for-each select="field|array"><xsl:call-template name="field_or_array" /></xsl:for-each>
        </table>
        <br />
    </xsl:template>
    <xsl:template name="field_or_array">
        <tr>
            <td class="fieldtype">
                <xsl:value-of select="@address"/>
            </td>
            <td class="fieldtype">
                <xsl:value-of select="@type"/><xsl:if test="@size">[<xsl:value-of select="@size"/>]</xsl:if>
            </td>
            <td class="fieldname">
                <xsl:value-of select="@name"/>
            </td>
            <td class="fieldvalue"><xsl:value-of select="@value"/><span class="filedvaluetype filedvaluetype_@valuetype" > (<xsl:value-of select="@valuetype"/>)</span></td>
            <td class="fielddescription"><xsl:value-of select="description"/><xsl:call-template name="options" /></td>
        </tr>
    </xsl:template>

    <xsl:template name="options">
        <xsl:if test="option">
            <ul class="fieldoption">
                <xsl:for-each select="option">
                    <li>
                        <xsl:call-template name="option" />
                    </li>
                </xsl:for-each>
            </ul>
        </xsl:if>
    </xsl:template>
    <xsl:template name="option">
        <tt><xsl:value-of select="@value"/></tt>
        <xsl:if test="@description">
            :
            <xsl:value-of select="@description"/>
        </xsl:if>
    </xsl:template>
</xsl:stylesheet>
