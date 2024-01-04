window.BENCHMARK_DATA = {
  "lastUpdate": 1704383165404,
  "repoUrl": "https://github.com/drexlerd/Loki",
  "entries": {
    "Benchmark": [
      {
        "commit": {
          "author": {
            "email": "dominik.drexler@gmx.net",
            "name": "Dominik Drexler",
            "username": "drexlerd"
          },
          "committer": {
            "email": "dominik.drexler@gmx.net",
            "name": "Dominik Drexler",
            "username": "drexlerd"
          },
          "distinct": true,
          "id": "64dd8be1a651c6ac6054f737da9422552d9235b0",
          "message": "reset benchmark data",
          "timestamp": "2024-01-04T16:29:50+01:00",
          "tree_id": "d424b87dedf2a58a8a3a84b4fd4ef306eb3cea58",
          "url": "https://github.com/drexlerd/Loki/commit/64dd8be1a651c6ac6054f737da9422552d9235b0"
        },
        "date": 1704382560890,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24478240.172413737,
            "unit": "ns/iter",
            "extra": "iterations: 29\ncpu: 24477900.000000004 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1275139.1150943018,
            "unit": "ns/iter",
            "extra": "iterations: 530\ncpu: 1275103.0188679246 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2810627.636000163,
            "unit": "ns/iter",
            "extra": "iterations: 250\ncpu: 2810584.3999999994 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "dominik.drexler@gmx.net",
            "name": "Dominik Drexler",
            "username": "drexlerd"
          },
          "committer": {
            "email": "dominik.drexler@gmx.net",
            "name": "Dominik Drexler",
            "username": "drexlerd"
          },
          "distinct": true,
          "id": "7bc902af77293b2380c7d7e50b5dddb7e2a6025e",
          "message": "switch to vector from small_vector",
          "timestamp": "2024-01-04T16:39:54+01:00",
          "tree_id": "e7120ab5126c0bd06545dea8925688475a71c53f",
          "url": "https://github.com/drexlerd/Loki/commit/7bc902af77293b2380c7d7e50b5dddb7e2a6025e"
        },
        "date": 1704383164975,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 27785389.48000005,
            "unit": "ns/iter",
            "extra": "iterations: 25\ncpu: 27784252 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1245308.3101604532,
            "unit": "ns/iter",
            "extra": "iterations: 561\ncpu: 1245299.2869875224 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 4105239.261111314,
            "unit": "ns/iter",
            "extra": "iterations: 180\ncpu: 4105116.666666666 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}