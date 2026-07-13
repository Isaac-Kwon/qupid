# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

**qupid** — **Qu**asi-Signal Generation Model for **Pi**xelized **D**etector. A C++/ROOT library that models the charge signal a particle deposits on a pixelized silicon sensor as an analytic 2D distribution (instead of a full Geant4/TCAD simulation), integrates it over a pixel matrix, applies a firing threshold, and studies the resulting cluster size via Monte Carlo. Geometry and units target ALPIDE-like sensors: lengths in µm (pixel pitch 29.24 × 26.88), signal amplitude in electrons (3.6 eV per e-h pair in analysis aliases), typical threshold 250 e⁻. Ported from the Python version [pyqupid](https://github.com/Isaac-Kwon/pyqupid).

## Build

Requires ROOT (https://root.cern) and CMake ≥ 3.2. ROOT is currently a hard link dependency of the library itself (the README's plan to make it optional outside `QPGraphFactory` was never implemented). C++ standard (11/14/17) is auto-selected to match how ROOT was built.

```bash
mkdir build && cd build
cmake ..
make
```

This produces `libqupid` (shared) plus one executable per test in `build/`. `include/`, `QPStyle.cpp`, and `rootlogon.C` are copied into `build/` so ROOT interactive sessions work from there.

Build gotchas:
- Sources and tests are collected with `file(GLOB)` — after adding/removing a `.cpp`/`.cxx`, re-run `cmake ..`.
- **Every** `test/*.cxx` is compiled as an executable; an incomplete/broken `.cxx` in `test/` fails the whole build (remove it to suppress the error).

## Tests

There is no test framework. `test/test0_*.cxx` … `test6_*.cxx` are standalone programs that double as usage examples, ordered from primitives to full pipeline (vector/matrix → Gaussian span → signal → digitizer → graph factory → pure Monte Carlo). Run a single one from `build/`:

```bash
cd build && ./test0_VectorMatrix
```

They write outputs (`.root` files, `.pdf` canvases) into the current working directory. `macro/analysis_peak.cpp` is a ROOT macro that consumes `test6_output.root`.

Interactive use: start `root` from `build/` (or the repo root); `rootlogon.C` loads `libqupid`, adds the include path, and applies `QPStyle`.

## Documentation

Doxygen, configured in `doc/Doxyfile`; relative paths resolve against the repo root:

```bash
doxygen doc/Doxyfile   # output: doc/doxygen/html, warnings: doc/doxygen/warnings.log
```

`.github/workflows/docs.yml` builds this on push to `master` and deploys to GitHub Pages (LaTeX is installed in CI because formulas render as PNG, `USE_MATHJAX = NO`).

## Architecture

Signal-processing chain, each stage an abstract base + concrete implementation:

1. **Signal** — `QPSignalBase` (amplitude × PDF; `GetSignal`, numeric `Integral` over a rectangle using the `qupid::NIntegrateDivide` grid from `QPGeneral.hpp`). `QPSignalGaussian` implements the PDF as a 2D Gaussian defined by a mean `QPVector` and covariance `QPMatrix` (supports σx, σy, rotation).
2. **Digitizer** — `QPDigitizerBase` owns the injected signals (`InjectSignal`/`ClearSignal`) and the pixel-matrix geometry; `GetAnalogSignal(x,y)` sums all signals; `GetFiredPixel(threshold)` returns pixels above threshold (→ cluster size). `QPDigitizerSquare` is a uniform rectangular grid.
3. **Pixel** — `QPPixelBase`/`QPPixel` integrate the analog signal over their own cell to produce the per-pixel digital signal.
4. **Visualization** — `QPGraphFactory` renders a digitizer's analog/digital/thresholded views as ROOT `TH2D`s; it is a `friend` of the digitizer classes.

Supporting layer: `QPMath` (namespaced free functions: Gaussian variants incl. rotated/asymmetric, Woods-Saxon, rotation matrix, ellipse geometry) with hand-rolled linear algebra in `QPVector`/`QPMatrix` — no Eigen, no ROOT math types in the core.

`QPCluster`, `QPClustering`, and `QPMonteCarlo` are empty stubs — cluster reconstruction was the planned next step and is not implemented.

Conventions: classes are prefixed `QP`, member variables `f` (ROOT style), header guards `__QPNAME__`; headers in `include/`, implementations in `src/`.
