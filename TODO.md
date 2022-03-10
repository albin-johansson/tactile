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

* Provide option for user to manually locate tileset images for maps with invalid tileset image paths
* (Optionally) warn when closing maps with unsaved changes
* Reset affected tiles in maps when a tileset is removed (and restore them if the tileset is restored)

## Objects

* Implement object reference properties

## Components 

* Decide on whether component data is stored embedded in maps or externally (maybe both?)
