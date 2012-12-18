<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xdoc="http://www.xdoc.org/version.1.1.0/index.html"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:redirect="org.apache.xalan.xslt.extension.Redirect"
  extension-element-prefixes="redirect">

  <xsl:output method="xml" encoding="UTF-8"/>

    <xsl:template match="/">
    <!-- Put out DTD info -->
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="processing-instruction()">
    <xsl:processing-instruction name="{name()}"><xsl:value-of select="."/></xsl:processing-instruction>
  </xsl:template>

  <xsl:template match="comment()">
    <xsl:comment><xsl:value-of select="."/></xsl:comment>
  </xsl:template>

  <!-- Catch all template -->
  <xsl:template match="*">
    <xsl:element name="{name()}"><xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="."/></xsl:attribute></xsl:for-each><xsl:if test="."><xsl:apply-templates/></xsl:if></xsl:element>
  </xsl:template>
</xsl:stylesheet>

