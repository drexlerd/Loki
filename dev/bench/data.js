window.BENCHMARK_DATA = {
  "lastUpdate": 1704387161956,
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
          "id": "a24006bb6d57f2894ec45f65db525e0cd24dd4ac",
          "message": "readded small vector to see performance differences",
          "timestamp": "2024-01-04T17:04:08+01:00",
          "tree_id": "ba38058b36e48534eed7ec90e503083c83157b98",
          "url": "https://github.com/drexlerd/Loki/commit/a24006bb6d57f2894ec45f65db525e0cd24dd4ac"
        },
        "date": 1704384642943,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 26642674.045454193,
            "unit": "ns/iter",
            "extra": "iterations: 22\ncpu: 26642381.818181824 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1244662.845149289,
            "unit": "ns/iter",
            "extra": "iterations: 536\ncpu: 1244677.6119402985 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2818765.36842093,
            "unit": "ns/iter",
            "extra": "iterations: 247\ncpu: 2818796.3562753038 ns\nthreads: 1"
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
          "id": "4b3b2186b0f7681bf4dea113db6fa9d8e2440d01",
          "message": "added cmake config",
          "timestamp": "2024-01-04T17:45:58+01:00",
          "tree_id": "71cf7fb558ac1bbfcde5d7d3123526fe512f0998",
          "url": "https://github.com/drexlerd/Loki/commit/4b3b2186b0f7681bf4dea113db6fa9d8e2440d01"
        },
        "date": 1704387161051,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25495907.296296403,
            "unit": "ns/iter",
            "extra": "iterations: 27\ncpu: 25493807.40740741 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1245271.3772242186,
            "unit": "ns/iter",
            "extra": "iterations: 562\ncpu: 1245162.4555160145 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2825460.342741796,
            "unit": "ns/iter",
            "extra": "iterations: 248\ncpu: 2825358.8709677407 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}