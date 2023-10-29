### 1. 如何存储参数值？

使用`ParamValue`包装`std::variant<int, double, char, std::string>`，内置`to_string`方法

用`std::unordered_map<std::string, std::vector<ParamValue>>;`存储

详见`include/common.h`

### 2.如何实现FunctionTarget?

由于自定义函数参数类型和个数的不确定性，一种方法是使用`std::tuple`和`std::apply`

需要解决的问题

* 如何根据函数参数类型创建tuple?
* 如何从variant中取出值然后修改tuple？

感谢万能的群友给出的解答

萃取函数参数类型

```cpp
template <typename R, typename... Args>
struct func_traits<R(Args...)> {
  using return_type = R;
  using arg_types = std::tuple<Args...>;
};
```

模板参数包直接传参，不需要改tuple

```cpp
[&]<std::size_t... Is>(std::index_sequence<Is...>) {
  f(std::get<std::tuple_element_t<Is, func_args>>(args[Is])...);
}
(std::make_index_sequence<func_args_num>{});
```

**局限性**

由于包装的类型是variant，尽管可以根据需要随时添加，但是类型实际是受限的，一种可能的解决方法是使用`std::any`，但是无法用`std::get`和`std::visit`访问值

另外，设置参数时需要确保类型的完全一致（包括const/引用），同时要注意潜在的类型转换

详见`include/target/function_target.h`

