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
- gcc 13.12 with ```-Ofast``` flag (https://godbolt.org/)
- clang 17.0.1 with ```-Ofast``` flag (https://godbolt.org/)

Typical timings are shown below.

### Visual Studio
#### Different parameter types
| Parameter types         | Time measured |
|-------------------------|---------------|
| ```const std::string&```      | 0.0006515   |
| ```std::string_view```        | 0.0006534   |
| ```const std::string_view&``` | 0.0006523    |
| ```boost::string_view```      | 0.0006517   |
#### Different return types
| Return types            | Time measured |
|-------------------------|---------------|
| ```const std::string&``` | 0.0006359 |
| ```std::string_view``` | 0.0006358 |
| ```boost::string_view``` | 0.0006358 |


### gcc
#### Different parameter types
| Parameter types         | Time measured |
|-------------------------|---------------|
| ```const std::string&```      | 0.000666876   |
| ```std::string_view```        | 0.000666885   |
| ```const std::string_view&``` | 0.00066694    |
| ```boost::string_view```      | 0.000666857   |
#### Different return types
| Return types            | Time measured |
|-------------------------|---------------|
| ```const std::string&``` | 0.0006114 |
| ```std::string_view``` | 0.0006114 |
| ```boost::string_view``` | 0.0006114 |

### clang
#### Different parameter types
| Parameter types         | Time measured |
|-------------------------|---------------|
| ```const std::string&```      | 0.000837026   |
| ```std::string_view```        | 0.000840242   |
| ```const std::string_view&``` | 0.000836303    |
| ```boost::string_view```      | 0.000836431   |
#### Different return types
| Return types            | Time measured |
|-------------------------|---------------|
| ```const std::string&``` | 0.000611567 |
| ```std::string_view``` | 0.000611581 |
| ```boost::string_view``` | 0.000611652 |


## Conclusion
Relative differences in timings for different parameter and return value types are usually less then per mille.
