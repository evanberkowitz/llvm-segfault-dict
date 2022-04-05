# llvm-segfault-dict

Compiling `dict.cpp` via `clang -std=c++20 dict.cpp` fails with a segfault.

When following [the instructions to try to report a back-end bug](https://www.llvm.org/docs/HowToSubmitABug.html#backend-code-generator-bugs) I tried passing the `-emit-llvm -c -o foo.bc` argument and was greeted with

```
> clang -std=c++20 dict.cpp -emit-llvm -c -o foo.bc
dict.cpp:128:28: error: implicit instantiation of undefined template 'std::array<std::string, 3>'
    for(auto fName: config.fieldNames()){
                           ^
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__tuple:219:64: note: template is declared here
template <class _Tp, size_t _Size> struct _LIBCPP_TEMPLATE_VIS array;
                                                               ^
dict.cpp:83:46: error: implicit instantiation of undefined template 'std::array<std::string, 3>'
    std::array<std::string,sizeof...(Types)> fieldNames(){
                                             ^
dict.cpp:128:28: note: in instantiation of member function 'Configuration<int, double, std::complex<double>>::fieldNames' requested here
    for(auto fName: config.fieldNames()){
                           ^
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__tuple:219:64: note: template is declared here
template <class _Tp, size_t _Size> struct _LIBCPP_TEMPLATE_VIS array;
                                                               ^
dict.cpp:84:50: error: implicit instantiation of undefined template 'std::array<std::string, 3>'
        std::array<std::string,sizeof...(Types)> names;
                                                 ^
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__tuple:219:64: note: template is declared here
template <class _Tp, size_t _Size> struct _LIBCPP_TEMPLATE_VIS array;
                                                               ^
clang: error: unable to execute command: Segmentation fault: 11
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Apple clang version 13.1.6 (clang-1316.0.21.2)
Target: x86_64-apple-darwin21.4.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /var/folders/xs/n80hfp_j4zx4z6gfqjf05wwr0000gn/T/dict-45ef40.cpp
clang: note: diagnostic msg: /var/folders/xs/n80hfp_j4zx4z6gfqjf05wwr0000gn/T/dict-45ef40.sh
clang: note: diagnostic msg: Crash backtrace is located in
clang: note: diagnostic msg: /Users/evanberkowitz/Library/Logs/DiagnosticReports/clang_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang: note: diagnostic msg:

********************
```

Also included in this repo are the indicated `dict-45ef40.cpp`, `dict-45ef40.sh` and `clang-2022-04-05-114851.ips` which (I think?) is the analog of the indicated `.crash` file (no `.crash` file was generated).

