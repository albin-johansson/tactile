# Godot Support

It is possible to export maps created using Tactile as Godot scenes. However, certain Tactile features, such as ellipse objects, cannot be directly translated. In these cases, Tactile will do its best to approximate the map as it is described in the Tactile editor.

## Limitations

- Tile layers must not reference tiles from more than *one* tileset
- Ellipse objects are not fully supported, see below for more details

## Usage

Once you are happy with your map, you can export it as a Godot scene using the action `Map -> Export -> Godot Scene...`. This will open a dialog in which you may fill in the necessary information about your Godot project.

## Translation

When you export a map as a Godot scene, Tactile will do its best to translate the contents of the map to corresponding nodes available in Godot. This section covers the overall translation strategy.

### Tile Layers

Tile layers are exported as `TileMap` nodes. 

### Object Layers & Objects

Each object layer is converted to a `Node2D`, all of which have their objects stored as immediate children nodes. Rectangle and ellipse objects are both converted to `Area2D` nodes, with attached `CollisionShape2D` nodes. Rectangle objects naturally use `RectangleShape2D` as the collision shape, whilst ellipses are not directly supported by Godot, so they are assigned `CircleShape2D` shapes instead. The strategy employed by Tactile when translating ellipse objects consists of comparing the width and height, and using the larger of the two as the diameter. Finally, point objects are simply converted to plain `Node2D` nodes.

### Group Layers

Group layers are naturally mapped to simple `Node2D` nodes, with all child layers attached as child nodes. 

### Properties & Components

TODO