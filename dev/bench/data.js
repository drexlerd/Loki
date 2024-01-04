window.BENCHMARK_DATA = {
  "lastUpdate": 1704382561248,
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
      }
    ]
  }
}