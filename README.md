# Speed comparison of passing and returning strings in C++

A simple benchmark that compares the speed of passing string to a method:
- as ```const std::string&```
- as ```std::string```
- as ```std::string_view```
- as ```const std::string_view&```
- as ```boost::string_view```

Arguments are taken from:
- ```std::string```
- ```boost::string_view```
- ```const char*```

Also, different return types are bechmarked:
- return ```const std::string&```
- return ```std::string_view```
- return ```boost::string_view```

## Results
Code was run on:
- Visual Studio 2022 (Version 17.8.3), x64 Release mode (my private machine)
- gcc 13.12 with ```-Ofast``` flag (https://godbolt.org/z/P3h3PTWYj)
- clang 17.0.1 with ```-Ofast``` flag (https://godbolt.org/z/66ddddKcd)

Typical timings are shown below.

### Different parameter types
#### Visual Studio
| Parameter type                | Argument type            | Time measured |
|-------------------------------|--------------------------|---------------|
| ```const std::string&```      | ```std::string```        | 0.0005871     |
| ```std::string_view```        | ```std::string```        | 0.0005866     |
| ```const std::string_view&``` | ```std::string```        | 0.0005869     |
| ```boost::string_view```      | ```std::string```        | 0.0005876     |
| ```std::string```             | ```boost::string_view``` | **0.0020685** |
| ```const std::string&```      | ```const char*```        | **0.002035** |
| ```std::string```             | ```const char*```        | **0.0020342** |
| ```boost::string_view```      | ```const char*```        | 0.0005944     |
#### gcc
| Parameter type                | Argument type            | Time measured  |
|-------------------------------|--------------------------|----------------|
| ```const std::string&```      | ```std::string```        | 0.000601113    |
| ```std::string_view```        | ```std::string```        | 0.000601281    |
| ```const std::string_view&``` | ```std::string```        | 0.000601274    |
| ```boost::string_view```      | ```std::string```        | 0.000601232    |
| ```std::string```             | ```boost::string_view``` | **0.00151555** |
| ```const std::string&```      | ```const char*```        | **0.00209719** |
| ```std::string```             | ```const char*```        | **0.00206175** |
| ```boost::string_view```      | ```const char*```        | 0.000600982    |
#### clang
| Parameter type                | Argument type            | Time measured   |
|-------------------------------|--------------------------|-----------------|
| ```const std::string&```      | ```std::string```        | 0.000767679     |
| ```std::string_view```        | ```std::string```        | 0.00077255      |
| ```const std::string_view&``` | ```std::string```        | 0.000775012     |
| ```boost::string_view```      | ```std::string```        | 0.000765972     |
| ```std::string```             | ```boost::string_view``` | **0.000893587** |
| ```const std::string&```      | ```const char*```        | **0.00137083**  |
| ```std::string```             | ```const char*```        | **0.00137587**  |
| ```boost::string_view```      | ```const char*```        | 0.000767388     |

### Different return types
#### Visual Studio
| Return type                     | Local type               | Time measured |
|---------------------------------|--------------------------|---------------|
| ```const std::string&```        | ```std::string```        | 0.0005700     |
| ```std::string_view```          | ```std::string```        | 0.0005697     |
| ```boost::string_view```        | ```std::string```        | 0.0005697     |
| ```boost::string_view```        | ```boost::string_view``` | 0.0005752     |
| ```const boost::string_view&``` | ```boost::string_view``` | 0.0005750     |
#### gcc
| Return type                     | Local type               | Time measured |
|---------------------------------|--------------------------|---------------|
| ```const std::string&```        | ```std::string```        | 0.00055094    |
| ```std::string_view```          | ```std::string```        | 0.000550952   |
| ```boost::string_view```        | ```std::string```        | 0.00055096    |
| ```boost::string_view```        | ```boost::string_view``` | 0.00055093    |
| ```const boost::string_view&``` | ```boost::string_view``` | 0.00055094    |
#### clang
| Return type                     | Local type               | Time measured |
|---------------------------------|--------------------------|---------------|
| ```const std::string&```        | ```std::string```        | 0.000560791   |
| ```std::string_view```          | ```std::string```        | 0.000559871   |
| ```boost::string_view```        | ```std::string```        | 0.00056079    |
| ```boost::string_view```        | ```boost::string_view``` | 0.000560781   |
| ```const boost::string_view&``` | ```boost::string_view``` | 0.000561132   |

## Conclusion
Contrary to popular belief that for ```string_view``` parameter type argument is passed faster, relative differences in timings measured for different parameter and return value types are usually less then per mille **except** when constructor od std::string is invoked (corresponding values are bold in tables above).
