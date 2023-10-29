## AutoTuner

AutoTuner是一个由cpp编写的自动调参库，支持对命令行程序、cpp自定义函数的自动调参。

### 已实现功能

* 对命令行程序的调参 - `CmdlineTarget`
* 对自定义函数的调参 - `FunctionTarget`
* 网格搜索调参 - `Gridsearch`
* 决策树预测调参 - `DecisionTreeSearch`
* 设置不同的优化目标 - `MIN_RUNTIME, MIN_OUTPUT_DOUBLE, MAX_OUTPUT_DOUBLE`

### 测试环境

本项目使用`C23`标准，测试操作系统为`5.10.16.3-microsoft-standard-WSL2`

### 项目文件

```
.
├── CMakeLists.txt
├── README.md
├── bin - 二进制文件
├── doc - 文档
├── examples - 示例文件
├── include - 头文件
├── lib - 库文件
├── src - 源代码
├── test - 测试代码
└── third_party - mlpack库
```

### 编译

项目由`cmake`构建，依赖于机器学习库`mlpack`

如果需要构建库文件，需要先安装mlpack（对应仓库已给出安装方法）

[mlpack/mlpack: mlpack: a fast, header-only C++ machine learning library (github.com)](https://github.com/mlpack/mlpack)

测试部分使用`GoogleTest`

### 如何使用

由于仓库已上传动态库至`lib/shared`，所以直接链接即可

```cmake
add_library(auto_tuner_shared_release SHARED IMPORTED)
set_target_properties(auto_tuner_shared_release PROPERTIES IMPORTED_LOCATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libauto_tuner_shared.so)

add_executable(example matrix_multiplication/example.cpp)
target_link_libraries(example auto_tuner_shared_release)
```

当然也可以根据需要自行编译动态库/静态库（确保mlpack的安装）

添加头文件，编译时链接库文件即可使用

```cpp
#include <auto_tuner.h>

int main() {
  tune::AutoTuner<tune::CmdlineTarget, tune::DecisionTreeSearch> auto_tuner{};
  ...
```

详细用法参见`examples/`

本项目衍生自课堂实验作业

### License
MIT - feel free to use or contribute:)