#include <stdio.h>
#include <stdlib.h>

#if 0
在C语言中，枚举类型（Enum）是一种用户自定义的数据类型，用于定义一些常量。枚举类型的每个常量都是一个整数值，通常用于代表一些特定的状态或者选项。

枚举类型的定义方式如下：

enum enum_name {
    const1,
    const2,
    const3,
    ...
};
其中，enum_name是枚举类型的名称，const1、const2、const3等是枚举类型的常量。
每个常量都会被赋予一个默认的整数值，从0开始逐个递增，也可以手动指定整数值，如下所示：

enum enum_name {
    const1 = 10,
    const2,
    const3 = 20,
    ...
};
在上面的例子中，const1的值被指定为10，const2的值将被赋值为11（因为前面已经有一个常量了），const3的值被指定为20。

枚举类型定义后，就可以使用它来声明变量，例如：

enum weekday {
    MON,
    TUE,
    WED,
    THU,
    FRI,
    SAT,
    SUN
};
enum weekday today = TUE;
上面的代码定义了一个枚举类型weekday，包含7个常量。然后使用weekday类型声明了一个名为today的变量，并将其初始化为TUE常量。

#endif
enum day{
    MON,
    TUE,
    WED,
    THU,
    FRI,
    SAT,
    SUN
};

int main()
{
    

    exit(0);
}