window.BENCHMARK_DATA = {
  "lastUpdate": 1704646312445,
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
          "id": "fa2be8735648d2fdd18e1eb671dfa4bd7084db42",
          "message": ".",
          "timestamp": "2024-01-04T18:40:25+01:00",
          "tree_id": "3ae9f43ca95ef5e03a9688a0691fbeefb2adccb5",
          "url": "https://github.com/drexlerd/Loki/commit/fa2be8735648d2fdd18e1eb671dfa4bd7084db42"
        },
        "date": 1704390394330,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24624246.107141595,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 24623089.28571429 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1276956.3351548268,
            "unit": "ns/iter",
            "extra": "iterations: 549\ncpu: 1276883.2422586519 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2799514.0515875006,
            "unit": "ns/iter",
            "extra": "iterations: 252\ncpu: 2799290.873015872 ns\nthreads: 1"
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
          "id": "4a4700cfab4bdc87314f33a4503ef44e167e141d",
          "message": "tiny",
          "timestamp": "2024-01-04T19:37:18+01:00",
          "tree_id": "5e8f535622455d73c513de67a23761580ebb7553",
          "url": "https://github.com/drexlerd/Loki/commit/4a4700cfab4bdc87314f33a4503ef44e167e141d"
        },
        "date": 1704393828666,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 26242083.555555634,
            "unit": "ns/iter",
            "extra": "iterations: 27\ncpu: 26241800 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1273783.4470802876,
            "unit": "ns/iter",
            "extra": "iterations: 548\ncpu: 1273724.4525547447 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2908778.168067308,
            "unit": "ns/iter",
            "extra": "iterations: 238\ncpu: 2908657.9831932774 ns\nthreads: 1"
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
          "id": "5b6c5ede7df77debe20da635f2f4bd7913850518",
          "message": "added missing include_directories",
          "timestamp": "2024-01-04T22:50:08+01:00",
          "tree_id": "54b2d6f568585315a0a5e60896fef69e95b71146",
          "url": "https://github.com/drexlerd/Loki/commit/5b6c5ede7df77debe20da635f2f4bd7913850518"
        },
        "date": 1704405286929,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25909668.35714223,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 25909282.14285714 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1273056.0400728427,
            "unit": "ns/iter",
            "extra": "iterations: 549\ncpu: 1273033.8797814208 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2797044.1314741187,
            "unit": "ns/iter",
            "extra": "iterations: 251\ncpu: 2796882.0717131486 ns\nthreads: 1"
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
          "id": "416e25d40602dda1bb9a3d825d15577d4af8cf8a",
          "message": ".",
          "timestamp": "2024-01-05T14:29:54+01:00",
          "tree_id": "d39a1522e38b1371daf3f8eca3f3a51c60bc0244",
          "url": "https://github.com/drexlerd/Loki/commit/416e25d40602dda1bb9a3d825d15577d4af8cf8a"
        },
        "date": 1704461708582,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 26349235.07407326,
            "unit": "ns/iter",
            "extra": "iterations: 27\ncpu: 26347014.814814813 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1277427.3083941366,
            "unit": "ns/iter",
            "extra": "iterations: 548\ncpu: 1277377.1897810223 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2935419.5684647164,
            "unit": "ns/iter",
            "extra": "iterations: 241\ncpu: 2935259.7510373434 ns\nthreads: 1"
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
          "id": "cdc4684ba6c8a86398fe4a801ab1ec544963d446",
          "message": ".",
          "timestamp": "2024-01-05T14:45:59+01:00",
          "tree_id": "e09a7bcb100225a1986f01353902ce75719315be",
          "url": "https://github.com/drexlerd/Loki/commit/cdc4684ba6c8a86398fe4a801ab1ec544963d446"
        },
        "date": 1704462661231,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25528965.499999247,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 25528389.285714287 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1275707.2127272876,
            "unit": "ns/iter",
            "extra": "iterations: 550\ncpu: 1275691.2727272734 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2821389.7349398546,
            "unit": "ns/iter",
            "extra": "iterations: 249\ncpu: 2821355.4216867466 ns\nthreads: 1"
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
          "id": "6115b32cd602b94815a32ef7a2c90d097d4a45c3",
          "message": ".",
          "timestamp": "2024-01-05T14:56:58+01:00",
          "tree_id": "692665e3fe4cefa2d31ee18d2995a70c3cc8c445",
          "url": "https://github.com/drexlerd/Loki/commit/6115b32cd602b94815a32ef7a2c90d097d4a45c3"
        },
        "date": 1704463323096,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25355842.846153107,
            "unit": "ns/iter",
            "extra": "iterations: 26\ncpu: 25353538.46153846 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1294815.8339483852,
            "unit": "ns/iter",
            "extra": "iterations: 542\ncpu: 1294714.0221402214 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2835140.380566805,
            "unit": "ns/iter",
            "extra": "iterations: 247\ncpu: 2834995.141700406 ns\nthreads: 1"
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
          "id": "35e817887826f412fa86eb99715261c02d83267d",
          "message": ".",
          "timestamp": "2024-01-05T15:02:54+01:00",
          "tree_id": "dc66758074ec9418071d2b32ad5355e7c0ddb9b9",
          "url": "https://github.com/drexlerd/Loki/commit/35e817887826f412fa86eb99715261c02d83267d"
        },
        "date": 1704463688403,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24531533.866667133,
            "unit": "ns/iter",
            "extra": "iterations: 30\ncpu: 24530906.66666667 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1276188.3180986622,
            "unit": "ns/iter",
            "extra": "iterations: 547\ncpu: 1276163.2541133454 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2803347.5059759715,
            "unit": "ns/iter",
            "extra": "iterations: 251\ncpu: 2803323.1075697206 ns\nthreads: 1"
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
          "distinct": false,
          "id": "35e817887826f412fa86eb99715261c02d83267d",
          "message": ".",
          "timestamp": "2024-01-05T15:02:54+01:00",
          "tree_id": "dc66758074ec9418071d2b32ad5355e7c0ddb9b9",
          "url": "https://github.com/drexlerd/Loki/commit/35e817887826f412fa86eb99715261c02d83267d"
        },
        "date": 1704464044607,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24678759.464285757,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 24678096.428571433 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1274811.6218181788,
            "unit": "ns/iter",
            "extra": "iterations: 550\ncpu: 1274778.1818181823 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2863771.0691055614,
            "unit": "ns/iter",
            "extra": "iterations: 246\ncpu: 2863700.81300813 ns\nthreads: 1"
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
          "id": "b0590b585519b44f4723ce5d22eb934bd5a94d9f",
          "message": "update workflows",
          "timestamp": "2024-01-05T15:09:16+01:00",
          "tree_id": "8a82646b43acb8901742f383df8b2b2f03badb20",
          "url": "https://github.com/drexlerd/Loki/commit/b0590b585519b44f4723ce5d22eb934bd5a94d9f"
        },
        "date": 1704464077256,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25353753.111110784,
            "unit": "ns/iter",
            "extra": "iterations: 27\ncpu: 25351888.888888888 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1277455.6265937218,
            "unit": "ns/iter",
            "extra": "iterations: 549\ncpu: 1277409.6539162116 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2821829.274900522,
            "unit": "ns/iter",
            "extra": "iterations: 251\ncpu: 2821674.9003984053 ns\nthreads: 1"
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
          "id": "46f0706e2b0e8eca7f7dfb550f6993b9b4d804b5",
          "message": "changed \\n to eol in parser, added eoi to keyword parsers, fixed test name",
          "timestamp": "2024-01-06T10:59:00+01:00",
          "tree_id": "dc814f6adc9447b71ec969fb067a9e78c6faf5f3",
          "url": "https://github.com/drexlerd/Loki/commit/46f0706e2b0e8eca7f7dfb550f6993b9b4d804b5"
        },
        "date": 1704535520633,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25065956.96296346,
            "unit": "ns/iter",
            "extra": "iterations: 27\ncpu: 25065388.888888888 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 1275645.7850638006,
            "unit": "ns/iter",
            "extra": "iterations: 549\ncpu: 1275622.9508196723 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2834366.459016432,
            "unit": "ns/iter",
            "extra": "iterations: 244\ncpu: 2834327.868852459 ns\nthreads: 1"
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
          "id": "bba05befdf6f7902eca9f5bd05a400db85f22503",
          "message": "remove name visitor",
          "timestamp": "2024-01-06T11:06:57+01:00",
          "tree_id": "6289f6092e0e9c03b75b83168050486d4fba63f9",
          "url": "https://github.com/drexlerd/Loki/commit/bba05befdf6f7902eca9f5bd05a400db85f22503"
        },
        "date": 1704535920535,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24550118.857143436,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 24548660.714285713 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 900236.0437580604,
            "unit": "ns/iter",
            "extra": "iterations: 777\ncpu: 900214.6718146718 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2005581.8104956667,
            "unit": "ns/iter",
            "extra": "iterations: 343\ncpu: 2005505.5393586003 ns\nthreads: 1"
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
          "id": "2f30b736e395880a52a9c1434c661e4fc6fbf43e",
          "message": "factored out tracking and untracking of lists of pddl objects",
          "timestamp": "2024-01-06T11:41:18+01:00",
          "tree_id": "4254d7a16636178fd231d2a653ff5e85491c7dca",
          "url": "https://github.com/drexlerd/Loki/commit/2f30b736e395880a52a9c1434c661e4fc6fbf43e"
        },
        "date": 1704538000286,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 23654373.133333214,
            "unit": "ns/iter",
            "extra": "iterations: 30\ncpu: 23653110.000000004 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 909641.0670967851,
            "unit": "ns/iter",
            "extra": "iterations: 775\ncpu: 909597.9354838708 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2007175.787356189,
            "unit": "ns/iter",
            "extra": "iterations: 348\ncpu: 2007111.781609195 ns\nthreads: 1"
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
          "id": "7983b71912225e96d94d06dec3f2c46ab52ed60f",
          "message": "update readme with missing flags for testing and benchmarking",
          "timestamp": "2024-01-06T11:54:00+01:00",
          "tree_id": "fffef99b77201bc86b58bf43a27c59f0c9392842",
          "url": "https://github.com/drexlerd/Loki/commit/7983b71912225e96d94d06dec3f2c46ab52ed60f"
        },
        "date": 1704538749268,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24879743.17857119,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 24878192.85714286 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 899651.1838046612,
            "unit": "ns/iter",
            "extra": "iterations: 778\ncpu: 899637.6606683806 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2018254.0809248504,
            "unit": "ns/iter",
            "extra": "iterations: 346\ncpu: 2018203.1791907507 ns\nthreads: 1"
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
          "id": "bb46e3927ff6fb0fe88a62b3369d25fb439655ba",
          "message": "added comment to code block",
          "timestamp": "2024-01-06T11:55:51+01:00",
          "tree_id": "9bb220b72d55c0f4ebaad6f5755ad735e630f13f",
          "url": "https://github.com/drexlerd/Loki/commit/bb46e3927ff6fb0fe88a62b3369d25fb439655ba"
        },
        "date": 1704538875510,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25291773.999999885,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 25290757.14285714 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 916504.1003860799,
            "unit": "ns/iter",
            "extra": "iterations: 777\ncpu: 916468.5971685973 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2023867.456647447,
            "unit": "ns/iter",
            "extra": "iterations: 346\ncpu: 2023706.936416186 ns\nthreads: 1"
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
          "id": "2f319ef14fc3fc22c4c273ec4015019b47f227f9",
          "message": "update comment symbol",
          "timestamp": "2024-01-06T11:56:38+01:00",
          "tree_id": "1797ff5f0c6538463ec96a5e85aa1bd74d171de1",
          "url": "https://github.com/drexlerd/Loki/commit/2f319ef14fc3fc22c4c273ec4015019b47f227f9"
        },
        "date": 1704538902305,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24990854.962964106,
            "unit": "ns/iter",
            "extra": "iterations: 27\ncpu: 24990314.814814813 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 900876.3868894922,
            "unit": "ns/iter",
            "extra": "iterations: 778\ncpu: 900866.7095115681 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2048272.4186046182,
            "unit": "ns/iter",
            "extra": "iterations: 344\ncpu: 2048272.6744186047 ns\nthreads: 1"
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
          "id": "59d1fffbebf5c021b16e0dd5fa7bfcc6c64b13f8",
          "message": "added more tests",
          "timestamp": "2024-01-06T14:50:22+01:00",
          "tree_id": "633971bc3de81d463d9f0be96b777d2797aad98a",
          "url": "https://github.com/drexlerd/Loki/commit/59d1fffbebf5c021b16e0dd5fa7bfcc6c64b13f8"
        },
        "date": 1704549341835,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25013874.115385395,
            "unit": "ns/iter",
            "extra": "iterations: 26\ncpu: 25013761.53846154 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 911636.4780927902,
            "unit": "ns/iter",
            "extra": "iterations: 776\ncpu: 911615.7216494847 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2049686.3779762008,
            "unit": "ns/iter",
            "extra": "iterations: 336\ncpu: 2049640.4761904764 ns\nthreads: 1"
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
          "id": "fa595213d8f506b5b2cc00002df065f08bee1bb1",
          "message": "added explanation on how to integrate loki in cmake projects",
          "timestamp": "2024-01-06T15:44:21+01:00",
          "tree_id": "d1459aa8c6400f290a4f6fa1167f70fb867ac65a",
          "url": "https://github.com/drexlerd/Loki/commit/fa595213d8f506b5b2cc00002df065f08bee1bb1"
        },
        "date": 1704552576115,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24167955.689655438,
            "unit": "ns/iter",
            "extra": "iterations: 29\ncpu: 24167510.34482759 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 921338.9035989334,
            "unit": "ns/iter",
            "extra": "iterations: 778\ncpu: 921332.9048843189 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2032131.5100864798,
            "unit": "ns/iter",
            "extra": "iterations: 347\ncpu: 2032122.7665706063 ns\nthreads: 1"
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
          "id": "ca4dfebee2c2c6416a663dfa9b77314700bf4b49",
          "message": "made it clearer what the installation directory is",
          "timestamp": "2024-01-06T15:45:17+01:00",
          "tree_id": "e74bd7f78d6c833ca829c7541957cd9c00c0a744",
          "url": "https://github.com/drexlerd/Loki/commit/ca4dfebee2c2c6416a663dfa9b77314700bf4b49"
        },
        "date": 1704552650932,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 26784611.50000107,
            "unit": "ns/iter",
            "extra": "iterations: 26\ncpu: 26777073.076923076 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 901363.0115681335,
            "unit": "ns/iter",
            "extra": "iterations: 778\ncpu: 901335.3470437017 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2057980.9941003502,
            "unit": "ns/iter",
            "extra": "iterations: 339\ncpu: 2057878.4660766965 ns\nthreads: 1"
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
          "id": "0657a1fd5ee1bf8d64f620f883e058acaa41eda9",
          "message": "added section to readme",
          "timestamp": "2024-01-06T15:50:09+01:00",
          "tree_id": "c0055604f7f03548d5c6e0fc22a8d39bc8722de4",
          "url": "https://github.com/drexlerd/Loki/commit/0657a1fd5ee1bf8d64f620f883e058acaa41eda9"
        },
        "date": 1704552915843,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 23999579.714285348,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 23998460.71428572 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 899327.716859702,
            "unit": "ns/iter",
            "extra": "iterations: 777\ncpu: 899317.5032175034 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2008514.4697406604,
            "unit": "ns/iter",
            "extra": "iterations: 347\ncpu: 2008534.5821325649 ns\nthreads: 1"
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
          "id": "6b6e8abe7ce70db3ae3eee5144356f53ed637c69",
          "message": "added integrationtest",
          "timestamp": "2024-01-06T16:50:35+01:00",
          "tree_id": "366dffca7a706cfdf43050df5c4323d3b818e727",
          "url": "https://github.com/drexlerd/Loki/commit/6b6e8abe7ce70db3ae3eee5144356f53ed637c69"
        },
        "date": 1704556537434,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 23716507.580645394,
            "unit": "ns/iter",
            "extra": "iterations: 31\ncpu: 23714448.387096778 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 892282.4125964186,
            "unit": "ns/iter",
            "extra": "iterations: 778\ncpu: 892244.3444730078 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2005272.2241380042,
            "unit": "ns/iter",
            "extra": "iterations: 348\ncpu: 2005185.632183908 ns\nthreads: 1"
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
          "id": "b9ca200a6542821236cb5d9c858aea6175653aec",
          "message": "remove compile flag",
          "timestamp": "2024-01-06T16:53:41+01:00",
          "tree_id": "178a200567fd4760a1f664f0349154291f422f6a",
          "url": "https://github.com/drexlerd/Loki/commit/b9ca200a6542821236cb5d9c858aea6175653aec"
        },
        "date": 1704556727683,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24184547.9310349,
            "unit": "ns/iter",
            "extra": "iterations: 29\ncpu: 24183206.89655173 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 901633.2768434989,
            "unit": "ns/iter",
            "extra": "iterations: 773\ncpu: 901576.8434670117 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2051319.5714286782,
            "unit": "ns/iter",
            "extra": "iterations: 336\ncpu: 2051295.5357142857 ns\nthreads: 1"
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
          "id": "f5bebef443c6ff6742f096b605aa3102ae0c2142",
          "message": "remove prefixes",
          "timestamp": "2024-01-06T16:54:38+01:00",
          "tree_id": "ff656f309bd1dbca1e99d6dda614c9f2fd670869",
          "url": "https://github.com/drexlerd/Loki/commit/f5bebef443c6ff6742f096b605aa3102ae0c2142"
        },
        "date": 1704556789397,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24536538.32142777,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 24534714.285714287 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 902435.8365508261,
            "unit": "ns/iter",
            "extra": "iterations: 777\ncpu: 902369.7554697555 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2039201.0379009726,
            "unit": "ns/iter",
            "extra": "iterations: 343\ncpu: 2039050.728862974 ns\nthreads: 1"
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
          "id": "68b97925f98fd24a4d3b9cc6b14af4b3ccd66982",
          "message": "fixed paths",
          "timestamp": "2024-01-06T17:10:36+01:00",
          "tree_id": "9a8b8931099274f7d767ded77577f2ec61dbcc92",
          "url": "https://github.com/drexlerd/Loki/commit/68b97925f98fd24a4d3b9cc6b14af4b3ccd66982"
        },
        "date": 1704557744399,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25400357.214284748,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 25400117.857142862 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 899951.052699278,
            "unit": "ns/iter",
            "extra": "iterations: 778\ncpu: 899890.4884318764 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2056806.3323614837,
            "unit": "ns/iter",
            "extra": "iterations: 343\ncpu: 2056681.9241982503 ns\nthreads: 1"
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
          "id": "5222f5474a120cff3219b10abf2bf4386f3e9f95",
          "message": "fix include order",
          "timestamp": "2024-01-06T18:16:24+01:00",
          "tree_id": "75371d36169af71e8e329819bd21f74e5ffd4d41",
          "url": "https://github.com/drexlerd/Loki/commit/5222f5474a120cff3219b10abf2bf4386f3e9f95"
        },
        "date": 1704561697095,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25604582.464284573,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 25602957.14285714 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 904957.1703225881,
            "unit": "ns/iter",
            "extra": "iterations: 775\ncpu: 904947.2258064519 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2010127.1959653737,
            "unit": "ns/iter",
            "extra": "iterations: 347\ncpu: 2010045.821325649 ns\nthreads: 1"
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
          "id": "4959229a5a5cb9094e3b4396c57113294fe7e4b1",
          "message": "added missing include_directory in config.cmake.in",
          "timestamp": "2024-01-06T18:23:11+01:00",
          "tree_id": "d89da396e5ff088c6f387a48bbc249438aed0e43",
          "url": "https://github.com/drexlerd/Loki/commit/4959229a5a5cb9094e3b4396c57113294fe7e4b1"
        },
        "date": 1704562106902,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24478539.482758828,
            "unit": "ns/iter",
            "extra": "iterations: 29\ncpu: 24477196.55172414 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 910394.3783783648,
            "unit": "ns/iter",
            "extra": "iterations: 777\ncpu: 910340.2831402833 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2019331.5977011502,
            "unit": "ns/iter",
            "extra": "iterations: 348\ncpu: 2019292.24137931 ns\nthreads: 1"
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
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "e07fb993eb0624690afac3de87a297cb12d08eff",
          "message": "Update CMakeLists.txt",
          "timestamp": "2024-01-06T20:27:35+01:00",
          "tree_id": "d9e9cabb05146acf2c6f1c788b4633a52ff43b9f",
          "url": "https://github.com/drexlerd/Loki/commit/e07fb993eb0624690afac3de87a297cb12d08eff"
        },
        "date": 1704569554335,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25018407.428571466,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 25016250 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 899866.4612403231,
            "unit": "ns/iter",
            "extra": "iterations: 774\ncpu: 899851.6795865635 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2015859.6293102205,
            "unit": "ns/iter",
            "extra": "iterations: 348\ncpu: 2015753.160919541 ns\nthreads: 1"
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
          "id": "530672c24abcd22c4209e9908dd2a9f3f9a8f92f",
          "message": "added caching of gtest installation",
          "timestamp": "2024-01-06T22:48:58+01:00",
          "tree_id": "9daac14d031053cc5743f52a25db5aa0cb6c0a34",
          "url": "https://github.com/drexlerd/Loki/commit/530672c24abcd22c4209e9908dd2a9f3f9a8f92f"
        },
        "date": 1704578042141,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 24594348.413794205,
            "unit": "ns/iter",
            "extra": "iterations: 29\ncpu: 24592782.758620694 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 901624.0038659552,
            "unit": "ns/iter",
            "extra": "iterations: 776\ncpu: 901580.025773196 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2028107.7093022694,
            "unit": "ns/iter",
            "extra": "iterations: 344\ncpu: 2028005.5232558139 ns\nthreads: 1"
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
          "id": "c108c42789ab5cc4fac1c361d3fb48120dd2347f",
          "message": "checkout googletest first",
          "timestamp": "2024-01-06T22:52:27+01:00",
          "tree_id": "4f5c02d909278723c3bf36e1735b8e5cf9ef36f7",
          "url": "https://github.com/drexlerd/Loki/commit/c108c42789ab5cc4fac1c361d3fb48120dd2347f"
        },
        "date": 1704578252455,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 23964904.9655178,
            "unit": "ns/iter",
            "extra": "iterations: 29\ncpu: 23962693.103448275 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 900731.4293059105,
            "unit": "ns/iter",
            "extra": "iterations: 778\ncpu: 900730.5912596403 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2040715.0639534967,
            "unit": "ns/iter",
            "extra": "iterations: 344\ncpu: 2040609.5930232564 ns\nthreads: 1"
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
          "id": "aafa5f45b4745b94845fcdb504baeb694e87418a",
          "message": "added flag to use all available cores for building",
          "timestamp": "2024-01-06T22:57:57+01:00",
          "tree_id": "f0e618c6d8056bb6cf3eeb40fd2d72499f07c2cc",
          "url": "https://github.com/drexlerd/Loki/commit/aafa5f45b4745b94845fcdb504baeb694e87418a"
        },
        "date": 1704578436739,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 25545201.14285757,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 25543435.714285713 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 901100.9184993628,
            "unit": "ns/iter",
            "extra": "iterations: 773\ncpu: 901093.6610608024 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2008732.7463976594,
            "unit": "ns/iter",
            "extra": "iterations: 347\ncpu: 2008655.619596542 ns\nthreads: 1"
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
          "id": "08c0d71c90c57e11f55d4b7becf21271976acedc",
          "message": "test caching",
          "timestamp": "2024-01-06T23:06:56+01:00",
          "tree_id": "b64d4f2f00dfca4a17ba40a8787a1828f86fbb82",
          "url": "https://github.com/drexlerd/Loki/commit/08c0d71c90c57e11f55d4b7becf21271976acedc"
        },
        "date": 1704578953846,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_ConstructAtoms",
            "value": 26278428.85714265,
            "unit": "ns/iter",
            "extra": "iterations: 28\ncpu: 26276117.85714286 ns\nthreads: 1"
          },
          {
            "name": "BM_IterateAtoms",
            "value": 901932.7273901959,
            "unit": "ns/iter",
            "extra": "iterations: 774\ncpu: 901822.3514211888 ns\nthreads: 1"
          },
          {
            "name": "BM_RandomlyIterateAtoms",
            "value": 2068496.1589595668,
            "unit": "ns/iter",
            "extra": "iterations: 346\ncpu: 2068183.8150289024 ns\nthreads: 1"
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
          "id": "f80403c56c4af5a82e8422d91a912ce3fe88ce2a",
          "message": "added more fine grained benchmarking",
          "timestamp": "2024-01-07T17:49:27+01:00",
          "tree_id": "6e98c88a29b0c004890864eaf917afe0c634b3d6",
          "url": "https://github.com/drexlerd/Loki/commit/f80403c56c4af5a82e8422d91a912ce3fe88ce2a"
        },
        "date": 1704646301270,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "loki::benchmarks::BM_ConstructAtoms",
            "value": 24225926.66666598,
            "unit": "ns/iter",
            "extra": "iterations: 30\ncpu: 24224230.000000004 ns\nthreads: 1"
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
          "id": "f80403c56c4af5a82e8422d91a912ce3fe88ce2a",
          "message": "added more fine grained benchmarking",
          "timestamp": "2024-01-07T17:49:27+01:00",
          "tree_id": "6e98c88a29b0c004890864eaf917afe0c634b3d6",
          "url": "https://github.com/drexlerd/Loki/commit/f80403c56c4af5a82e8422d91a912ce3fe88ce2a"
        },
        "date": 1704646311765,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "loki::benchmarks::BM_IterateAtoms/1000",
            "value": 10351.302604128368,
            "unit": "ns/iter",
            "extra": "iterations: 68545\ncpu: 10350.728718360204 ns\nthreads: 1"
          },
          {
            "name": "loki::benchmarks::BM_IterateAtoms/10000",
            "value": 114190.18674698743,
            "unit": "ns/iter",
            "extra": "iterations: 6142\ncpu: 114186.16085965483 ns\nthreads: 1"
          },
          {
            "name": "loki::benchmarks::BM_IterateAtoms/100000",
            "value": 1144213.854232001,
            "unit": "ns/iter",
            "extra": "iterations: 638\ncpu: 1144155.3291536046 ns\nthreads: 1"
          },
          {
            "name": "loki::benchmarks::BM_IterateAtoms/1000000",
            "value": 11661076.916666957,
            "unit": "ns/iter",
            "extra": "iterations: 60\ncpu: 11660416.666666662 ns\nthreads: 1"
          },
          {
            "name": "loki::benchmarks::BM_RandomlyIterateAtoms/1000",
            "value": 15585.652943907262,
            "unit": "ns/iter",
            "extra": "iterations: 45229\ncpu: 15585.197550244322 ns\nthreads: 1"
          },
          {
            "name": "loki::benchmarks::BM_RandomlyIterateAtoms/10000",
            "value": 195359.3020190711,
            "unit": "ns/iter",
            "extra": "iterations: 3566\ncpu: 195346.1581604038 ns\nthreads: 1"
          },
          {
            "name": "loki::benchmarks::BM_RandomlyIterateAtoms/100000",
            "value": 2620576.4052044074,
            "unit": "ns/iter",
            "extra": "iterations: 269\ncpu: 2620554.6468401505 ns\nthreads: 1"
          },
          {
            "name": "loki::benchmarks::BM_RandomlyIterateAtoms/1000000",
            "value": 71616289.99999595,
            "unit": "ns/iter",
            "extra": "iterations: 9\ncpu: 71612344.44444454 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}