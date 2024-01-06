window.BENCHMARK_DATA = {
  "lastUpdate": 1704538000632,
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
      }
    ]
  }
}