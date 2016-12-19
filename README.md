# UtilsClass

+ Utilities class, including operations of string, datetime, file I/O etc.
+ Author: [Liangjun Zhu](http://zhulj.net)

## 单独测试UtilsClass模块
### 1 Windows
+ 打开 “开始” -> Microsoft Visual Studio 2010 -> Visual Studio Tools -> Visual Studio 命令提示(2010)，以**管理员方式**运行，依次输入以下命令：

```shell
cd <path-to-UtilsClass>
mkdir build
cd build
### 仅编译安装 ###
cmake -G "NMake Makefiles" <path-to-UtilsClass> -DCMAKE_BUILD_TYPE=Release
nmake
nmake install
### 编译Microsoft Visual Studio工程 ###
cmake <path-to-UtilsClass>
nmake
```

+ 对于“仅编译安装”操作，`UtilsClass.exe`会自动安装在`<path-to-UtilsClass>\bin`目录下。
+ 对于“编译Microsoft Visual Studio工程”，`UtilsClass.sln`将保存在`<path-to-UtilsClass>\build`目录下。

### 2 Unix
对于Linux和macOS系统而言，操作与Windows类似，这里不再赘述。
