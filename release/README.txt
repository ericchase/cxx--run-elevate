# Installation

Run `Install.exe` (double click)

It should run as Administrator automatically, but do so manually if needed by
right-clicking it and selecting "Run as administrator".

The installer will create a folder under `C:\Program Files` and copy these
files to it:
- Elevate.exe
- Run.exe
- vcruntime140.dll

It will also add system wide aliases for both Run.exe and Elevate.exe, and then
add the installation folder path to the system wide Path environment variable.


# Right-click menu items

Double click to run. Click `Yes` and `OK` when prompted.

## CMD.reg
This file adds right click menu items to folders for opening them in Command
Prompt.

## CMD.[-].reg
This file removes the "Command Prompt" right click menu items.


# Uninstallation

Run `Uninstall.exe` (double click)

It should run as Administrator automatically, but do so manually if needed by
right-clicking it and selecting "Run as administrator".

The uninstaller will delete these files from the installation folder:
- Elevate.exe
- Run.exe
- vcruntime140.dll

It will also delete the installation folder itself is able, remove the system
wide aliases for Run.exe and Elevate.exe, and finally remove the installation
folder path from the system wide Path environment variable.

Make sure to run the `CMD.[-].reg` file as well when uninstalling.
