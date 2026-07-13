# qupid

[English](README.md) | [한국어](README.ko.md)

[![DOI](https://zenodo.org/badge/408895120.svg)](https://zenodo.org/badge/latestdoi/408895120)

**Qu**asi-Signal Generation Model for **Pi**xelized **D**etector.

qupid models the charge a traversing particle deposits on a pixelized silicon
sensor as an analytic 2D distribution, rather than running a full Geant4/TCAD
simulation. It integrates that distribution over a pixel matrix, applies a
firing threshold, and studies the resulting cluster size by Monte Carlo.
Geometry and units follow ALPIDE-like sensors: lengths in µm (pixel pitch
29.24 × 26.88), signal amplitude in electrons (3.6 eV per electron-hole pair),
a typical firing threshold of 250 e⁻.

## The model

The signal is an amplitude times a 2D probability density. `QPSignalGaussian`
takes the density from a mean vector and a covariance matrix, so σx, σy, and a
rotation are all free. A digitizer sums the injected signals and integrates the
analog field over each cell of the pixel matrix; `QPDigitizerSquare` is a
uniform rectangular grid. Pixels whose integrated signal exceeds the threshold
are the fired pixels, and their count is the cluster size. `QPMonteCarlo` runs
this chain over randomized events and emits one record per event. Random
numbers come through the `QPRandomBase` interface; the default implementation,
`QPRandom`, uses `std::mt19937_64`.

The supporting linear algebra and math (`QPVector`, `QPMatrix`, `QPMath` —
Gaussian variants, Woods-Saxon, rotation matrices, ellipse geometry) are
hand-written and use nothing beyond the standard library.

## Dependencies

- **Core (`libqupid`)**: a standard C++17 toolchain, nothing else. The signal
  model, digitizer, pixel integration, RNG, and Monte Carlo driver all live
  here.
- **Visualization (`libqupidVis`, from `vis/`)**: [ROOT](https://root.cern).
  `QPGraphFactory` renders a digitizer's analog, digital, and thresholded views
  as ROOT `TH2D` histograms. This module is optional and built only when ROOT
  is present.

ROOT's `TRandom` can drive the Monte Carlo in place of `QPRandom` through the
header-only adapter `vis/include/QPRandomTRandom.hpp`; only an executable that
includes it links ROOT.

## Build

CMake ≥ 3.16.

```bash
cmake -B build
cmake --build build -j
```

By default (`QUPID_WITH_ROOT=ON`) CMake looks for ROOT and builds `libqupidVis`
and the ROOT-dependent tests when it is found; if ROOT is missing it builds the
core alone and reports that it did. To leave ROOT out entirely:

```bash
cmake -B build -DQUPID_WITH_ROOT=OFF
cmake --build build -j
```

Either way `libqupid` and the core tests are built. Executables land in the
build directory and write their output (`.csv`, `.root`, `.pdf`) to the current
directory, so run them from there.

## Tests and examples

The programs in `test/` double as usage examples, ordered from primitives to
the full pipeline. `test0_VectorMatrix`, `test3_digitizer`, and
`test7_montecarlo_csv` build and run without ROOT (`test7` writes the Monte
Carlo output as CSV). `test1`, `test2`, `test4`, `test5`, and `test6` use ROOT
for plotting or tree output and are built only in the ROOT configuration.

## Tutorials

`tutorials/` collects short, commented programs, built through the same core /
ROOT split as the tests:

1. defining a Gaussian signal and integrating its density (core only);
2. a digitizer and the fired-pixel cluster size at a threshold (core only);
3. a Monte Carlo run to CSV (core only);
4. visualization with `QPGraphFactory` and the `TRandom` adapter (ROOT).

## Documentation

The API documentation is generated with Doxygen (`doc/Doxyfile`) and published
to GitHub Pages on each push to `master`:
<https://isaac-kwon.github.io/qupid/>. For a ROOT interactive session,
`rootlogon.C` loads `libqupid` and `libqupidVis` and applies the plotting
style.

## Provenance

Ported from the Python version,
[pyqupid](https://github.com/Isaac-Kwon/pyqupid).
