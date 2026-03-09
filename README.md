# SLAM - Sample Loop Analysis Module

A command-line tool for validating audio sample files against naming conventions and technical specifications.

## Features

- Validates sample filenames against configurable naming rules
- Checks WAV file technical specifications (sample rate, bit depth)
- Detects loops vs one-shots based on BPM presence
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
- `analyze <folder>` - Validate all WAV files in a folder
- `help` - Show available commands
- `info` - Display current configuration
- `exit` - Quit program

## Configuration

Edit `slam.conf` to customize validation rules:
```
LABEL=DS
ARTIST=HAYVE
BPM_MIN=40
BPM_MAX=200
SAMPLE_RATE=44100
BIT_DEPTH=24
```

## Requirements

- GCC compiler (Linux/WSL or MinGW on Windows)
- Standard C library
- Works on both Linux and Windows
