# Godot Support

It is possible to export maps created using Tactile as Godot scenes.
However, certain Tactile features, such as ellipse objects, cannot be directly translated.
In these cases, Tactile will do its best to approximate the map as it is described in the Tactile editor.

## Limitations

You do not have to design your maps any differently to export them as Godot scenes, but there are certain limitations due to implementation differences between Tactile and Godot.

- In Godot, a `TileMap` (which is really a tile layer) may only feature one associated `TileSet`, so Tactile will merge all tilesets associated with the map into a single `TileSet` definition.
- Ellipse objects are approximated as polygons.
- Godot tile animations cannot have frame-specific durations, so Tactile always uses the duration of the first frame.
- Properties and components are only provided as metadata.
- 4D vector properties are saved as arrays.

## Usage

Once you are happy with your map, you can export it as a Godot scene using the action `Map -> Export As -> Godot Scene...`.
This will open a dialog in which you may fill in the necessary information about your Godot project.

## Translation

When you export a map as a Godot scene, Tactile will do its best to translate the contents of the map to corresponding nodes available in Godot.
This section covers the overall translation strategy.

### Tilesets

No matter how many tilesets your map uses, Tactile will only generate a single Godot `TileSet` definition.
This information is written to a separate `tileset.tres` file.
The `TileSet` resource will contain the merged data from all of your tilesets.
Images used by your tilesets will also be automatically copied to your Godot project.
Consider disabling the `Import Defaults -> Texture -> Filter` option in your Godot project, to prevent blurry tiles by default.

### Tile Layers

Tile layers are fully supported, and are exported as `TileMap` nodes. All `TileMap` nodes will use the same `TileSet` resource.

### Object Layers & Objects

Each object layer is exported as a `Node2D` node, which have their objects stored as immediate children nodes.

Rectangle objects are exported using `Area2D` and `CollisionShape2D` nodes, using `RectangleShape2D` as the collision shape.
Ellipse objects are not directly supported in Godot, so they are approximated as polygons using `Area2D` and `CollisionPolygon2D` nodes.
Lastly, point objects are simply converted to plain `Node2D` nodes.

### Group Layers

Group layers are naturally mapped to simple `Node2D` nodes, with all child layers attached as child nodes.
