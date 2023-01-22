# Tiled Support

Tactile maintains general compatibility with the Tiled JSON and XML file formats.
This document outlines the general limitations of this compatibility.

## Unsupported Tiled features

The following Tiled features have no corresponding feature in Tactile and are thus unsupported.

* Image layers
* Wang tiles
* User-defined property types
* `class` properties
* Tileset transformations
* Tileset tile offsets
* Text objects
* Infinite maps (chunks)
* Template objects

## Import

The following are the limitations when importing Tiled maps.

* Most unsupported features are simply ignored during parsing
* Features introduced after version 1.7 of both Tiled JSON/XML formats are generally not supported

## Export

The following are the limitations when exporting Tactile maps in a Tiled format.

* Vector properties are exported as plain string properties
* Components are ignored when using Tiled formats
