# Installer

[Qt installer framework]: https://doc.qt.io/qtinstallerframework/index.html

The Tactile installers are built using the Qt installer framework.

Note, the [Qt installer framework] is not by default included when installing Qt. There are numerous options for how to install Qt, which are not discussed here. See [this](https://download.qt.io) page for an overview of available Qt downloads. Beware of the licensing requirements.

## Generate the installer

After a build of the editor has been generated, move the built files to the `installer/packages/com.albinjohansson.tactile/data` directory. This is the content that will be deployed.

Also, remember to update the configuration files `config.xml` and `packages.xml`.

Use the following command to build the installer.

```bash
binarycreator -p packages -c config/config.xml TactileInstaller.app  # macOS
binarycreator -p packages -c config/config.xml TactileInstaller.exe  # Windows
```
