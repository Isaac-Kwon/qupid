# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

**qupid** — **Qu**asi-Signal Generation Model for **Pi**xelized **D**etector. A C++ library (ROOT-free core, optional ROOT visualization layer) that models the charge signal a particle deposits on a pixelized silicon sensor as an analytic 2D distribution (instead of a full Geant4/TCAD simulation), integrates it over a pixel matrix, applies a firing threshold, and studies the resulting cluster size via Monte Carlo. Geometry and units target ALPIDE-like sensors: lengths in µm (pixel pitch 29.24 × 26.88), signal amplitude in electrons (3.6 eV per e-h pair in analysis aliases), typical threshold 250 e⁻. Ported from the Python version [pyqupid](https://github.com/Isaac-Kwon/pyqupid).

## Build

Requires CMake ≥ 3.16 and a C++17 compiler. ROOT (https://root.cern) is optional: the `QUPID_WITH_ROOT` option (default ON) gates the visualization module `libqupidVis` and the ROOT-dependent tests. With `-DQUPID_WITH_ROOT=OFF` — or when ROOT is simply not found (configure prints which case applies) — only the ROOT-free core is built and no ROOT installation is needed. The standalone build uses C++17; when ROOT is found, the C++ standard follows how ROOT was built.

```bash
mkdir build && cd build
cmake ..            # add -DQUPID_WITH_ROOT=OFF for a core-only build
make
```

This produces `libqupid` (shared), `libqupidVis` when ROOT is enabled, plus one executable per listed test in `build/`. `include/` and `rootlogon.C` are copied into `build/`; with ROOT enabled, `macro/QPStyle.cpp` and the `vis/include` headers are copied too, so ROOT interactive sessions work from there.

Build gotchas:
- Sources and tests are explicit lists, not `file(GLOB)`: a new core `.cpp` goes into `qupid_sources` in the top-level `CMakeLists.txt`; a new test goes into `CORE_TESTS` (ROOT-free) or `ROOT_TESTS` (ROOT-gated) in `test/CMakeLists.txt`. A `.cxx` in `test/` that is not listed is simply not built — an incomplete WIP file no longer breaks the build.

## Tests

There is no test framework. `test/test0_*.cxx` … `test7_*.cxx` are standalone programs that double as usage examples, ordered from primitives to full pipeline (vector/matrix → Gaussian span → signal → digitizer → graph factory → pure Monte Carlo). `test0_VectorMatrix`, `test3_digitizer`, and `test7_montecarlo_csv` (ROOT-free Monte Carlo with `QPRandom`, CSV output) are in `CORE_TESTS` and build/run in the `QUPID_WITH_ROOT=OFF` configuration; the rest need ROOT. Run a single one from `build/`:

```bash
cd build && ./test0_VectorMatrix
```

They write outputs (`.root` files, `.pdf` canvases, `test7_output.csv`) into the current working directory. `macro/analysis_peak.cpp` is a ROOT macro that consumes `test6_output.root`.

Regression baseline: `test6` uses `TRandom3` with the default seed and a fixed per-event draw order, so its output is deterministic. `macro/dump_test6.C` dumps `test6_output.root` to a tab-separated `test6_dump.tsv` (amp/x/y/npix, `%.17g`) for bitwise comparison; `test/baseline/test6_npix_ref.txt` is the npix-distribution reference used by CI.

Interactive use: start `root` from `build/` (or the repo root); `rootlogon.C` loads `libqupid` and `libqupidVis`, adds the include path, and applies `QPStyle`.

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
4. **Visualization** — `QPGraphFactory` (in `vis/include`/`vis/src`, built as `libqupidVis` only when `QUPID_WITH_ROOT` is ON and ROOT is found) renders a digitizer's analog/digital/thresholded views as ROOT `TH2D`s; it is a `friend` of the digitizer classes and links `libqupid` + `ROOT::Core/Hist/Graf`.
5. **Monte Carlo & RNG** — `QPRandomBase` is the abstract RNG interface and the only RNG type core code may reference; `QPRandom` implements it on `std::mt19937_64`. `QPMonteCarlo` drives the per-event loop: draw x, then y, then amplitude when randomized — this draw order is fixed and the regression baselines depend on it — then inject one Gaussian signal, count fired pixels, emit a plain record (vector accumulation, CSV via `WriteCSV`). `vis/include/QPRandomTRandom.hpp` is a header-only adapter wrapping a caller-owned `TRandom*`; neither `libqupid` nor `libqupidVis` includes it, so ROOT is linked only by the executables that actually use it (`test5`/`test6`).

Supporting layer: `QPMath` (namespaced free functions: Gaussian variants incl. rotated/asymmetric, Woods-Saxon, rotation matrix, ellipse geometry) with hand-rolled linear algebra in `QPVector`/`QPMatrix` — no Eigen, no ROOT math types in the core.

`QPCluster` and `QPClustering` are empty stubs — cluster reconstruction was the planned next step and is not implemented.

Conventions: classes are prefixed `QP`, member variables `f` (ROOT style), header guards `__QPNAME__`; core headers in `include/` with implementations in `src/`, visualization in `vis/include`/`vis/src`.
