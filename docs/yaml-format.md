# YAML format

Other than the Tiled JSON and XML formats, Tactile supports its own custom YAML map format. If
you're familiar with the Tiled JSON format, you'll notice many similarities. However, some key
differences include that the Tactile YAML format doesn't support embedded tilesets.

## Root

|        Attribute |   Type   | Required | Description                           |
| ---------------: | :------: | :------: | :------------------------------------ |
|           `name` | `string` |   Yes    | The name associated with the tilemap. |
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

## `layer`

|    Attribute |   Type   | Required | Description                                          |
| -----------: | :------: | :------: | :--------------------------------------------------- |
|       `name` | `string` |   Yes    | The name associated with the layer.                  |
|         `id` |  `int`   |   Yes    | The unique identifier associated with the layer.     |
|       `type` | `string` |   Yes    | One of `tile-layer`, `object-layer` or `group-layer` |
|    `opacity` | `float`  |    No    | The opacity of the layer, in the range [0, 1].       |
|    `visible` |  `bool`  |    No    | Whether or not the layer is rendered.                |
| `properties` | Sequence |    No    | A sequence of `property` nodes.                      |

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

## `object`

|    Attribute |   Type   | Required | Description                     |
| -----------: | :------: | :------: | :------------------------------ |
|       `name` | `string` |   Yes    |                                 |
|         `id` |  `int`   |   Yes    |                                 |
|       `type` | `string` |    No    |                                 |
|    `visible` |  `bool`  |    No    |                                 |
|          `x` | `float`  |    No    |                                 |
|          `y` | `float`  |    No    |                                 |
|      `width` | `float`  |    No    |                                 |
|     `height` | `float`  |    No    |                                 |
| `properties` | Sequence |    No    | A sequence of `property` nodes. |

## `tileset-ref`

|         Attribute |   Type   | Required | Description                                           |
| ----------------: | :------: | :------: | :---------------------------------------------------- |
| `first-global-id` |  `int`   |   Yes    | The first global tile ID associated with the tileset. |
|            `path` | `string` |   Yes    | Relative path to the tileset definition file.         |

## `tileset`

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

### `tile`

|    Attribute |   Type   | Required | Description                     |
| -----------: | :------: | :------: | :------------------------------ |
|         `id` |  `int`   |   Yes    |                                 |
|  `animation` | Sequence |    No    | A sequence of `frame` nodes.    |
| `properties` | Sequence |    No    | A sequence of `property` nodes. |

### `frame`

|  Attribute | Type  | Required | Description |
| ---------: | :---: | :------: | :---------- |
|     `tile` | `int` |   Yes    |             |
| `duration` | `int` |   Yes    |             |

## `property`

| Attribute |   Type   | Required | Description |
| --------: | :------: | :------: | :---------- |
|    `name` | `string` |   Yes    |             |
|    `type` | `string` |   Yes    |             |
|   `value` | `string` |   Yes    |             |
