#UVSocket

基于libuv的服务端和客户端Socket.

Server and Client socket based on libuv.

#Compile

这个工程使用[premake5](http://premake.github.io/download.html)编译。从命令行运行：

```
premake5 vs2015
``` 

也可以选择生成其他你需要的visual studio版本(用premake5 --help命令查看更多选项)。打开VSbuild/UVSocket.sln文件，分别编译debug和release版本，然后运行根目录下的"Extract.bat"，将所有头文件和库文件提取到项目根目录下的"publish"文件夹下.

You need [premake5](http://premake.github.io/download.html) to compile this project. Run this from command line:


```
premake5 vs2015
``` 


or other visual studio version(use premake5 --help for more details). Open VSBuild/UVSocket.sln and compile both Debug and Release version.
Then run the "Extract.bat" to extract all libraries and header files to "publish" directory under project root directory.

#Supported platform

暂时只支持Windows. Linux应该也可以正常运行，但是未经测试。

Only Windows right now. But it should run well on Linux, but not be tested.

#Usage

编写一个你自己的类，继承"UVSocketCallBack.h"头文件中的IUVServerCallback 或者 IUVClientCallback（取决于你需要哪个），实现所有接口。你可以参考"TestClientSocket"和"TestServerSocket"中的用法。

Write your own class derived from IUVServerCallback or IUVClientCallback(decided by which socket do you need) in "UVSocketCallBack.h", and implement all interfaces.

You can take "TestClientSocket" and "TestServerSocket" for example.

#About Run Mode

UVSocket提供了两种运行模式：SOCKET_RUN_ASYNC(异步模式)和 SOCKET_RUN_SYNC（同步模式）。

**SOCKET_RUN_ASYNC**:异步模式，这是默认运行模式，整个Sockdet事件循环将在一个独立线程运行，你提供的类中实现的回调函数，也将在这个独立线程运行，这种模式适合单一的业务模式使用。在这种模式下，你只需要调用一次Run()函数，剩下的都交给UVSocket即可。如果你需要在这个类中处理其他业务，你需要考虑多线程编程的安全问题。

**SOCKET_RUN_SYNC**:同步模式，在这种模式下，不会创建独立的线程，因此你必须在自己的线程循环中反复调用Run()。回调函数的触发也是和你调用Run()是在同一个线程。

UVSocket provide two run modes: SOCKET_RUN_ASYNC and SOCKET_RUN_SYNC.


**SOCKET_RUN_ASYNC**: This is default run mode. Socket event loop will run on a standalone thread, and your own class derived from IUVServerCallback or IUVClientCallback will run on the same thread with socket's, too. In this mode, you should only call Run() once. So if you want to do something other in your derived class, you should care about multithread safety.

**SOCKET_RUN_SYNC**: Socket event loop will run on thread you call Run() function. You should keep calling Run() on your thread's run process.