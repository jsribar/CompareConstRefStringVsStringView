# Speed comparison of passing and returning strings in C++

A simple benchmark that compares the speed of passing string to a method:
- as ```const std::string&```
- as ```std::string_view```
- as ```const std::string_view&```
- as ```boost::string_view```

Also, different return types are bechmarked:
- return ```const std::string&```
- return ```std::string_view```
- return ```boost::string_view```

## Results
Code was run on:
- Visual Studio 2022 (Version 17.8.3), x64 Release mode (my private machine)
- gcc 13.12 with ```-Ofast``` flag (https://godbolt.org/z/3nzsfo59M)
- clang 17.0.1 with ```-Ofast``` flag (https://godbolt.org/z/vnabvjT4Y)

Typical timings are shown below.

### Visual Studio
#### Different parameter types
| Parameter types         | Time measured |
|-------------------------|---------------|
| ```const std::string&```      | 0.0014311   |
| ```std::string_view```        | 0.0014298   |
| ```const std::string_view&``` | 0.0014300   |
| ```boost::string_view```      | 0.0014304   |
#### Different return types
| Return types             | Time measured |
|--------------------------|---------------|
| ```const std::string&``` | 0.0009508 |
| ```std::string_view```   | 0.0009513 |
| ```boost::string_view``` | 0.0009499 |
| ```boost::string_view2``` | 0.0009617 |
| ```const boost::string_view&``` | 0.0009675 |


### gcc
#### Different parameter types
| Parameter types         | Time measured |
|-------------------------|---------------|
| ```const std::string&```      | 0.00166879   |
| ```std::string_view```        | 0.00162398   |
| ```const std::string_view&``` | 0.00164239    |
| ```boost::string_view```      | 0.00164594   |
#### Different return types
| Return types            | Time measured |
|-------------------------|---------------|
| ```const std::string&``` | 0.000917671 |
| ```std::string_view``` | 0.000917651 |
| ```boost::string_view``` | 0.000917702 |
| ```boost::string_view2``` | 0.00091768 |
| ```const boost::string_view&``` | 0.00091767 |

### clang
#### Different parameter types
| Parameter types         | Time measured |
|-------------------------|---------------|
| ```const std::string&```      | 0.00147693   |
| ```std::string_view```        | 0.00138438   |
| ```const std::string_view&``` | 0.00138049   |
| ```boost::string_view```      | 0.00138044   |
#### Different return types
| Return types            | Time measured |
|-------------------------|---------------|
| ```const std::string&``` | 0.000918258 |
| ```std::string_view``` | 0.000918149 |
| ```boost::string_view``` | 0.000917973 |
| ```boost::string_view2``` | 0.000918066 |
| ```const boost::string_view&``` | 0.000918127 |


## Conclusion
Relative differences in timings for different parameter and return value types are usually less then per mille.
