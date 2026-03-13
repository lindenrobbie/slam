# SLAM - Sample Loop Analysis Module

A command-line tool for validating audio sample files against technical specifications.

## Features

- Checks WAV file technical specifications (sample rate, bit depth)
- Interactive CLI with colored output
- Configurable via slam.conf file

## Building

### Linux/WSL:
```
bash
mkdir build
cd build
cmake ..
make
```

### Windows (MinGW):
```
bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

## Usage

### Linux/WSL:
```
bash
./build/slam
```
### Windows:
Run from project root:
```
bash
.\build\slam.exe
```

Commands:
- `analyze` - Validate all WAV files in a folder
- `help` - Show available commands
- `info` - Display current configuration
- `exit` - Quit program

## Configuration

Default config listed below:

Edit `slam.conf` to customize validation rules:
```
SAMPLE_RATE=44100
BIT_DEPTH=24
SAMPLES_FOLDER=samples
```

## Requirements

- GCC compiler (Linux/WSL or MinGW on Windows)
- Standard C library
- Works on both Linux and Windows
- Uses dirent.h compatibility layer for Windows (https://github.com/tronkko/dirent)
¨
## Test samples

To test different bit depths and sample rate there is 5 test samples provided:

```
sample1.wav = 44100Hz / 32bit
sample2.wav = 44100Hz / 16bit
sample3.wav = 48000Hz / 24bit
sample4.wav = 48000Hz / 24bit
sample5.wav = 44100Hz / 24bit
```

## Possible extensions

- Have file name validation according to the splice naming standard (Tokenize names between "_")
- Be able to normalize audio to a configed level in dBFS (Truepeak?)
- Detect loop length if it bleeds over 1-2<sup>n</sup> bars
- Trim detected bleeding loops
- Import custom folder path
- Have command arguments (?) e.g ```normalize (folder)```

## Possible improvements

- Compile for MacOS using github actions so Aaro can use it for work
- Have better code structure:
- - No hardcoded strings for commands (if/else)
- Make error handling more pro (actual return values)
- - Error codes into enum
- - No hardcoded error messages (Make in separate file)
- Unit tests
- Have magic strings & constants as enum (?)
- Add debug level, make print processes in real time & logging
- Improve config validation
- Check memory allocation
- Learn CMake (?)
- Make CI/CD (?)

## What I learned !

- Header file creation and a make file creation
- Structs and standards for a lot of things, e.g wav files have 3 chunks
- Binary file reading and parsing (Not just CSV)
- Not to overcomplicate and overmodulorize code (Like I kinda did in this project)
- Comments as much as this isn't normal (But it's to help the teacher and future me)
- Compiling for windows sucks
