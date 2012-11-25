<?xml version="1.0"?>

<!-- bigger.xsl $Id$
     Author: Terry Brown
     Created: Thu Sep 25 2008
-->

<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:svg="http://www.w3.org/2000/svg"
  xmlns:sodipodi="http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd"
  >
  
  <xsl:output method="xml"/>
  
  <xsl:template match="svg:svg">

    <xsl:copy>  <!-- copying the svg element -->
 
      <xsl:copy-of select="svg:metadata|svg:defs|sodipodi:namedview"/>

      <xsl:element name="svg:g">

	<xsl:attribute name="transform">
	  <xsl:value-of select="concat(concat('scale(',$scale),')')"/>
	</xsl:attribute>

	<xsl:copy-of
	    select="*[not(svg:metadata|svg:defs|sodipodi:namedview)]"/>

      </xsl:element>

    </xsl:copy>

  </xsl:template>

</xsl:stylesheet>
