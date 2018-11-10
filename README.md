# DFGame UI Module
DFGame is a collection of libraries designed to remove much of the boilerplate
involved in "from scratch" game development. This module provides additional
UI-related features for projects using DFGame

## Features
Currently, this module is in an early WIP state. All that is supported is loading and rendering custom boxes/frames.

## Building This Library
To build this library, you'll need the following tools:
- meson
- ninja
- pkg-config (optional)
as well as all DFGame dependencies.

You can build and install this library using the following steps.
```bash
# Create build directory and generate project files
mkdir build && cd build
meson ..

# Compile and install
ninja
ninja install
```
