# SLAM - Sample Loop Analysis Module

A command-line tool for validating audio sample files against technical specifications.

## Features

- Checks WAV file technical specifications (sample rate, bit depth)
- Interactive CLI with colored output
- Configurable via slam.conf file

## Building

```bash
make
```

## Usage

```bash
./slam
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
- Have better code structure (?)
