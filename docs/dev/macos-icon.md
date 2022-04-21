# macOS icons

In order to generate a new `.icns` file for icon updates, use the following steps.

Create the following versions of the icon.

|                  Name | Size      |
|----------------------:|:----------|
|      `icon_16x16.png` | 16x16     |
|   `icon_16x16@x2.png` | 32x32     |
|      `icon_32x32.png` | 32x32     |
|   `icon_32x32@x2.png` | 64x64     |
|    `icon_128x128.png` | 128x128   |
| `icon_128x128@x2.png` | 256x256   |
| `icon_512x512@x2.png` | 512x512   |
| `icon_512x512@x2.png` | 1024x1024 |

Then, move these icons into a folder called `icons.iconset`.

Finally, create a `.icns` file by using the following command.

```bash
iconutil -c icns icon.iconset
```
