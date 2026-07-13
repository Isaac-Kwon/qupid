# qupid

[English](README.md) | [한국어](README.ko.md)

[![DOI](https://zenodo.org/badge/408895120.svg)](https://zenodo.org/badge/latestdoi/408895120)

**Qu**asi-Signal Generation Model for **Pi**xelized **D**etector.

qupid는 입자가 픽셀화된 실리콘 센서에 남기는 전하 신호를, Geant4/TCAD 전체
시뮬레이션 대신 해석적인 2차원 분포로 모델링한다. 이 분포를 픽셀 행렬 위에서
적분하고 발화 문턱값(firing threshold)을 적용한 뒤, 그 결과로 나오는 클러스터
크기를 몬테카를로로 조사한다. 기하와 단위는 ALPIDE 계열 센서를 따른다. 길이는
µm(픽셀 피치 29.24 × 26.88), 신호 진폭은 전자 수(전자-정공 쌍당 3.6 eV),
전형적인 발화 문턱값은 250 e⁻이다.

## 모델

신호는 진폭과 2차원 확률밀도의 곱이다. `QPSignalGaussian`은 이 밀도를 평균
벡터와 공분산 행렬로부터 정의하므로 σx, σy, 그리고 회전이 모두 자유롭다.
디지타이저는 주입된 신호들을 합한 뒤 픽셀 행렬의 각 셀 위에서 아날로그 장을
적분하며, `QPDigitizerSquare`는 균일한 직사각 격자다. 적분된 신호가 문턱값을
넘는 픽셀이 발화 픽셀이고, 그 개수가 곧 클러스터 크기다. `QPMonteCarlo`는 이
과정을 무작위 사건들에 대해 반복하며 사건마다 레코드 하나를 남긴다. 난수는
`QPRandomBase` 인터페이스를 거쳐 뽑고, 기본 구현인 `QPRandom`은
`std::mt19937_64`를 쓴다.

이를 떠받치는 선형대수와 수학(`QPVector`, `QPMatrix`, `QPMath` — 가우시안
변형들, Woods-Saxon, 회전 행렬, 타원 기하)은 직접 작성했으며 표준 라이브러리
바깥의 것은 쓰지 않는다.

## 의존성

- **코어 (`libqupid`)**: 표준 C++17 툴체인 외에는 아무것도 필요 없다. 신호
  모델, 디지타이저, 픽셀 적분, 난수 생성기, 몬테카를로 드라이버가 모두 여기에
  있다.
- **시각화 (`libqupidVis`, `vis/`)**: [ROOT](https://root.cern)가 필요하다.
  `QPGraphFactory`가 디지타이저의 아날로그·디지털·문턱 처리 결과를 ROOT
  `TH2D` 히스토그램으로 그린다. 이 모듈은 선택 사항이며 ROOT가 있을 때만
  빌드된다.

몬테카를로는 `QPRandom` 대신 ROOT의 `TRandom`으로도 돌릴 수 있다. 헤더 온리
어댑터 `vis/include/QPRandomTRandom.hpp`를 쓰면 되며, 이를 include한 실행
파일만 ROOT에 링크된다.

## 빌드

CMake 3.16 이상이 필요하다.

```bash
cmake -B build
cmake --build build -j
```

기본값(`QUPID_WITH_ROOT=ON`)에서는 CMake가 ROOT를 찾아 있으면 `libqupidVis`와
ROOT 의존 테스트까지 빌드하고, 없으면 코어만 빌드하며 그 사실을 알린다. ROOT를
아예 빼려면 다음과 같이 한다.

```bash
cmake -B build -DQUPID_WITH_ROOT=OFF
cmake --build build -j
```

어느 쪽이든 `libqupid`와 코어 테스트는 빌드된다. 실행 파일은 빌드 디렉터리에
생기고 출력(`.csv`, `.root`, `.pdf`)은 현재 디렉터리에 쓰이므로 그 안에서
실행한다.

## 테스트와 예제

`test/`의 프로그램들은 원시 요소부터 전체 파이프라인까지 순서대로 놓인 사용
예제를 겸한다. `test0_VectorMatrix`, `test3_digitizer`, `test7_montecarlo_csv`는
ROOT 없이 빌드·실행되며(`test7`은 몬테카를로 출력을 CSV로 쓴다), `test1`,
`test2`, `test4`, `test5`, `test6`은 그림이나 트리 출력에 ROOT를 써서 ROOT
설정에서만 빌드된다.

## 튜토리얼

`tutorials/`에는 짧고 주석이 달린 프로그램들이 테스트와 같은 코어 / ROOT
구분으로 빌드되어 들어 있다.

1. 가우시안 신호를 정의하고 밀도를 적분하기 (코어 전용);
2. 디지타이저와 문턱값에서의 발화 픽셀 클러스터 크기 (코어 전용);
3. 몬테카를로를 CSV로 출력하기 (코어 전용);
4. `QPGraphFactory`와 `TRandom` 어댑터로 시각화하기 (ROOT).

## 문서

API 문서는 Doxygen(`doc/Doxyfile`)으로 생성되며 `master`에 push할 때마다 GitHub
Pages에 게시된다: <https://isaac-kwon.github.io/qupid/>. ROOT 대화형 세션에서는
`rootlogon.C`가 `libqupid`와 `libqupidVis`를 불러오고 그림 스타일을 적용한다.

## 출처

Python 판인 [pyqupid](https://github.com/Isaac-Kwon/pyqupid)에서 포팅했다.
