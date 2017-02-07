# CS130 winter 2017

## Echo server, using boost.

+ *to build the webserver*
`$ make webserver`

+ *to run*
`$ sudo ./webserver example_config`

+ *clean up*
`make clean`

+ *run unit tests* (add sudo if not write permission)
```
$ make run_all_tests
```

+ *run test coverage (gcov)* (depends on success of `$ make run_all_tests`)
```
$ make run_test_coverage
```

+ *run integration test* 
```
$ make run_integration_test
```