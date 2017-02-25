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

## Code Layout

+ Config format, see example_config
currently, all path must be of one word; "/" is not allowed. 

+ Create handlers
Important function: `get_server_parameters` in `server.cc`
handlers are created and stored in a member variable uri2handler,
which is a std::map from uri to handler pointer. e.g. "/static" -> Hander_Static

+ Each connection is separated into a new thread.
function `Server::run_server()` will call `Server::session(socket_ptr sock)`

+ calling handlers
The uri is parsered in to "head" and "tail".
e.g. "/static/foo/bar" => "/static" + "/foo/bar".
"head" is used to find the corresponding handler.
The logic is implemented in `Server::session(socket_ptr sock)`.