# TODO

This document provides a brief outline of planned features for the Tactile editor.

## General

* Projects to allow common tilesets, components and general templates for maps
* Support viewing and editing external tilesets
* Minimap widget
* Custom themes?

## Tiled compatibility

* Tiled allows color properties to be unset, in which case the colors are exported as empty strings, make sure that the parsers can handle this.

## UX

* Fix viewport zoom on desktops
* Fix tileset viewport scrolling on desktops
* Provide option for user to manually locate tileset images for maps with invalid tileset image paths
* (Optionally) warn when closing maps with unsaved changes
* If stamp tool is disabled during a stamp sequence, it is not possible to undo the sequence
* Reset affected tiles in maps when a tileset is removed (and restore them if the tileset is restored)

## Objects

* Ellipse object tool
* Implement object reference properties

## Components 

* Decide on whether component data is stored embedded in maps or externally (maybe both?)
* Parsing
* Emitting
