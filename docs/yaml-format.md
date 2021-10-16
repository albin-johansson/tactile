# YAML format

Other than the Tiled JSON and XML formats, Tactile supports its own custom YAML map format. If
you're familiar with the Tiled JSON format, you'll notice many similarities. However, one of the key
differences is that the Tactile YAML format doesn't support embedded tilesets. All predefined names are in `kebab-case`. All attributes that aren't specified as required may be omitted from save files.

## Root

The root node in the Tactile YAML format provides general information about the tilemap.

|        Attribute |   Type   | Required | Description                           |
| ---------------: | :------: | :------: | :------------------------------------ |
|        `version` |  `int`   |   Yes    | The version of the YAML map format.   |
|      `row-count` |  `int`   |   Yes    | The number of rows in the tilemap.    |
|   `column-count` |  `int`   |   Yes    | The number of columns in the tilemap. |
|     `tile-width` |  `int`   |   Yes    | The width of tiles in the tilemap.    |
|    `tile-height` |  `int`   |   Yes    | The height of tiles in the tilemap.   |
|  `next-layer-id` |  `int`   |   Yes    | The next available layer ID.          |
| `next-object-id` |  `int`   |   Yes    | The next available object ID.         |
|         `layers` | Sequence |    No    | A sequence of `layer` nodes.          |
|       `tilesets` | Sequence |    No    | A sequence of `tileset-ref` nodes.    |
|     `properties` | Sequence |    No    | A sequence of `property` nodes.       |

---

## `layer`

|    Attribute |   Type   | Required | Default | Description                                          |
| -----------: | :------: | :------: | :-----: | :--------------------------------------------------- |
|       `name` | `string` |   Yes    |   N/A   | The name associated with the layer.                  |
|         `id` |  `int`   |   Yes    |   N/A   | The unique identifier associated with the layer.     |
|       `type` | `string` |   Yes    |   N/A   | One of `tile-layer`, `object-layer` or `group-layer` |
|    `opacity` | `float`  |    No    |  `1.0`  | The opacity of the layer, in the range [0, 1].       |
|    `visible` |  `bool`  |    No    | `true`  | Whether or not the layer is rendered.                |
| `properties` | Sequence |    No    |   N/A   | A sequence of `property` nodes.                      |

Example:

```YAML
name: foo
id: 7
type: tile-layer
data: >
  1 2 3 4
  5 6 7 8
```

In addition to the layer attributes listed above, each of the three different kinds of layers have
additional attributes.

### `tile-layer`

| Attribute |   Type   | Required | Description                                               |
| --------: | :------: | :------: | :-------------------------------------------------------- |
|    `data` | `string` |   Yes    | A string of global tile identifiers, separated by spaces. |

### `object-layer`

| Attribute |   Type   | Required | Description                   |
| --------: | :------: | :------: | :---------------------------- |
| `objects` | Sequence |   Yes    | A sequence of `object` nodes. |

### `group-layer`

| Attribute |   Type   | Required | Description                  |
| --------: | :------: | :------: | :--------------------------- |
|  `layers` | Sequence |   Yes    | A sequence of `layer` nodes. |

---

## `object`

|    Attribute |   Type   | Required | Default | Description                          |
| -----------: | :------: | :------: | :-----: | :----------------------------------- |
|         `id` |  `int`   |   Yes    |   N/A   |                                      |
|       `type` | `string` |   Yes    |   N/A   | One of `point`, `rect` or `ellipse`. |
|       `name` | `string` |    No    |  `""`   |                                      |
|        `tag` | `string` |    No    |  `""`   | A user-defined type tag.             |
|    `visible` |  `bool`  |    No    | `true`  |                                      |
|          `x` | `float`  |    No    |   `0`   |                                      |
|          `y` | `float`  |    No    |   `0`   |                                      |
|      `width` | `float`  |    No    |   `0`   |                                      |
|     `height` | `float`  |    No    |   `0`   |                                      |
| `properties` | Sequence |    No    |   N/A   | A sequence of `property` nodes.      |

Example:

```YAML
name: foo
id: 42
type: point
tag: spawn-point
x: 123
y: 456
```

---

## `tileset-ref`

The `tileset-ref` node provides information about an external tileset definition, and are always stored in the main tilemap file.

|         Attribute |   Type   | Required | Description                                           |
| ----------------: | :------: | :------: | :---------------------------------------------------- |
| `first-global-id` |  `int`   |   Yes    | The first global tile ID associated with the tileset. |
|            `path` | `string` |   Yes    | Relative path to the tileset definition file.         |

Example:

```YAML
first-global-id: 42
path: foo/bar/tileset.yaml
```

---

## `tileset`

The `tileset` node is always defined in an external YAML file, since embedded tilesets are not supported
by the Tactile YAML format. By default, a tileset that is added to a map in the Tactile editor will be stored
next to the main map file.

|      Attribute |   Type   | Required | Description                                |
| -------------: | :------: | :------: | :----------------------------------------- |
|         `name` | `string` |   Yes    | The name of the tileset.                   |
|   `tile-count` |  `int`   |   Yes    | The total amount of tiles in the tileset.  |
|   `tile-width` |  `int`   |   Yes    | The width of each tile in the tileset.     |
|  `tile-height` |  `int`   |   Yes    | The height of each tile in the tileset.    |
| `column-count` |  `int`   |   Yes    | The amount of tile columns in the tileset. |
|   `image-path` | `string` |   Yes    | Relative path to the tileset image.        |
|  `image-width` |  `int`   |   Yes    | The width of the tileset image.            |
| `image-height` |  `int`   |   Yes    | The height of the tileset image.           |
|      `version` |  `int`   |   Yes    |                                            |
|        `tiles` | Sequence |    No    | A sequence of `tile` nodes.                |
|   `properties` | Sequence |    No    | A sequence of `property` nodes.            |

Example:

```YAML
name: foo
tile-count: 1024
tile-width: 32
tile-height: 32
column-count: 32
image-path: ../foo.png
image-width: 1024
image-height: 1024
version: 1
```

### `tile`

The `tile` node provides additional information about tiles in a tileset.

|    Attribute |   Type   | Required | Description                      |
| -----------: | :------: | :------: | :------------------------------- |
|         `id` |  `int`   |   Yes    | Local ID of the associated tile. |
|  `animation` | Sequence |    No    | A sequence of `frame` nodes.     |
| `properties` | Sequence |    No    | A sequence of `property` nodes.  |

Example:

```YAML
id: 42
animation:
  - tile: 123
    duration: 150
  - tile: 456
    duration: 120
```

### `frame`

The `frame` node represents a frame in an animation.

|  Attribute | Type  | Required | Description                                                    |
| ---------: | :---: | :------: | :------------------------------------------------------------- |
|     `tile` | `int` |   Yes    | Local ID of the tile that should be rendered during the frame. |
| `duration` | `int` |   Yes    | Duration of the frame, in milliseconds.                        |

---

## `property`

| Attribute |   Type    | Required | Description                                                          |
| --------: | :-------: | :------: | :------------------------------------------------------------------- |
|    `name` | `string`  |   Yes    | Name that is unique in the scope of "sibling" properties.            |
|    `type` | `string`  |   Yes    | One of `string`, `int`, `float`, `bool`, `color`, `file` or `object` |
|   `value` | `variant` |   Yes    | The value of the property.                                           |

Note, color properties are always stored using RGBA encoding, with a `#` prefix, e.g. `"#11223344"`.

Examples:

```YAML
name: Scale
type: float
value: 0.7
```

```YAML
name: Tint
type: color
value: "#4FEB19FF"
```

```YAML
name: Associated object
type: object
value: 42
```

```YAML
name: Raw data
type: file
value: foo/bar/data.bin
```
