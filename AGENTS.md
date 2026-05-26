# CST-9074

This workspace contains a floor truss component data file (`FT1.json`) and a Qt 6 C++ viewer application.

## Project Structure

- `FT1.json` — floor truss data exported from an engineering/truss-design app (compatibility version `2026.3.0.66`)
- `TrussViewer` — Qt 6 Widgets app that loads FT1.json and renders the truss graphically
- `TrussGen` — CLI tool that generates a PNG image from a truss JSON file

## Build

Uses Qt 6.11.0 MSVC 2022 64-bit at `D:\Qt\6.11.0\msvc2022_64`.

```
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="D:\Qt\6.11.0\msvc2022_64"
cmake --build build --config Debug
```

Run: `.\build\Debug\TrussViewer.exe`

### TrussGen (CLI image generator)

```
.\build\Release\TrussGen.exe input.json [output.png]

```
.\build\Release\TrussGen.exe input.json output.svg  # SVG output
```

If `output.png` is omitted, it derives the name from the input file. Output format (PNG or SVG) is detected from the file extension.

Also available as a standalone zero-dependency EXE (`dist-trussgen\TrussGen.exe`, 218 KB). No DLLs needed.

## Distribute

Build Release and deploy:

```
cmake --build build --config Release
copy build\Release\TrussViewer.exe dist\
copy build\Release\TrussGen.exe dist\
D:\Qt\6.11.0\msvc2022_64\bin\windeployqt.exe dist\TrussViewer.exe --no-translations
robocopy "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Redist\MSVC\14.44.35112\x64\Microsoft.VC143.CRT" dist\ *.dll
```

Zip `dist\` — it's fully self-contained with both tools.

For a tiny TrussGen-only deploy, ship just `dist-trussgen\TrussGen.exe` (218 KB, no dependencies).

## Truss Data Format

Each piece in `PieceData[]` has:
- `Type`: TopChord / BottomChord / Web
- `EndCap.Positive[]` and `EndCap.Negative[]` — arrays of `{x, y}` defining the two ends of the piece
- `EngineeringLabel`: piece identifier
- Coordinates: X 0–120 (span), Y roughly -7.25 to 16.75 (height); Z always 0 (flat truss)
