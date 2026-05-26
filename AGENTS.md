# CST-9074

TrussGen — standalone CLI that generates 2D (PNG/SVG) and 3D (X3D/HTML with X3DOM) output from floor/roof truss JSON data. No external runtime dependencies beyond GDI+ (PNG).

## Quick Start

```
TrussGen.exe FT1.json              # → FT1.png
TrussGen.exe FT1.json out.svg      # SVG
TrussGen.exe FT1.json out.x3d      # X3D (standalone)
TrussGen.exe FT1.json out.html     # HTML with embedded X3DOM
```

Output format detected from file extension.

## Build (standalone EXE)

```
cl /EHsc /O2 /std:c++17 /DUNICODE /D_UNICODE /utf-8 /Fe:TrussGen.exe TrussGen.cpp /link gdiplus.lib
```

From a VS 2022 x64 developer prompt. Also distributable as `dist-trussgen\TrussGen.exe` (218 KB, zero-deps).

## 3D Output Validation

Use [Castle Model Viewer](https://castle-engine.io/castle-model-viewer) (v5.2.0 installed at `C:\Users\jissi\.local\bin\castle-model-viewer.exe`):

```
castle-model-viewer.exe scene.x3d --hide-extras --screenshot 0 out.png
```

Screenshot sizes: ~5 KB = blank/empty; 15 KB+ = rendered scene with content. The `--hide-extras` flag disables the default bounding‑box overlay (green wireframe that appears around every model).

## Known Quirks

- **X3D MFString quoting**: XML `family='"SANS"'` → MFString `["SANS"]`. Use double-quotes as MFString delimiters inside the XML attribute value.
- **X3D inch‑mark escaping**: Literal `"` in text content → `""` (per X3D MFString spec).
- **No self-closing tags in HTML**: X3DOM traverses the DOM tree; `<Tag/>` is parsed as an unclosed start tag in HTML5. Always use `<Tag></Tag>`.
- **Viewpoint orientation axis**: Computed as `(ly, -lx, 0)` (cross product of default -Z direction and camera-to-center vector), normalized. Negating this axis causes the camera to look away from the scene.
- **x3dom.js** must be in working directory at HTML generation time; inlined into the output for offline use.
- **Castle Model Viewer `--screenshot`** works reliably for headless rendering; the before/after screenshot size difference confirmed the orientation fix.
- **Castle Model Viewer default bounding box**: Shown as a green wireframe box around every model. Disable with `--hide-extras` or View → Show Bounding Box in the GUI.

## Truss Data Format

Each piece in `PieceData[]`:
- `Type`: TopChord / BottomChord / Web
- `EndCap.Positive[]` / `EndCap.Negative[]` — `{x,y}` defining the two ends
- `UserThickness`: piece thickness in inches (defaults from root `Thickness`)
- `EngineeringLabel`: piece identifier
- Coordinates in inches (×100 to integers internally); Z always 0

## Files

| File | Purpose |
|------|---------|
| `TrussGen.cpp` | All logic: JSON parse, Canvas (GDI+), savePNG/SVG/X3D/HTML, main |
| `FT1.json` | Floor truss sample (span 120", height 24", thick 3.5") |
| `T04.json` | Roof truss sample (span 432", height 94", thick 1.5") |
| `x3dom.js` | X3DOM 1.8.2 library, read at HTML generation time |
| `AGENTS.md` | This file |
| `dist-trussgen/` | Standalone EXE deployment (no DLLs) |
