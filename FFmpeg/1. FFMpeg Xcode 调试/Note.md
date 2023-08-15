# FFmpeg Xcode 调试 


## 编译 ffmepg 

```c
./configure  --disable-optimizations --disable-stripping --enable-debug=3 --disable-doc
make -j `nproc`

./configure --enable-debug --enable-gpl --enable-libx264 \
--enable-nonfree --enable-libmp3lame --disable-optimizations

```

#### 可能依赖的库
* `brew install x264` 
* `brew install lame`

__以_g结尾的就是可以调试的程序ffmpeg_g, ffplay_g, ffprobe_g__ 


## Xcode 项目 
1. 创建 macos 项目 

![图 6](images/113eb7961aff9bc3f8b5d7798e57ef03b43b30237a23120a776a73358c780d06.png)  


2. 项目配置 
![图 7](images/b0c2f206b8f1db5a43d6c3906a1e5ad2da105777554bbb7c01cc7872aadd3f56.png)  


3. 添加 ffmepg 目录到工程中 

![图 8](images/b3faf252acb9cf916e677872a10e5c3c63f0b257b54cb30422d80e02570ce2c3.png)  

![图 9](images/c27d2cb6859573855f48349c96716a9957c6cdbd4bbffbb2de5cdc654f426e04.png)  

4. 添加 target 调试 
![图 10](images/14cf98f4b1fd08778e82a09334f65df24335166d2f3064619851cc67012793ce.png)  
![图 11](images/33d3e1336f48301aa47bda519895e1bcf7ac5f7c989f301db7410328d043ff4d.png)  
![图 12](images/a933bc956eca3bfe05bab6e50df5d3845593fbca6ea501172af098b48c63129a.png)  

![图 13](images/8ad180a11fe4a5cde9bd1689f8023186fefdfa0757af5600592959481afed71a.png)  


5. 更改代码后，重新调试 
    * make  重新编译出对应的可执行文件 




### 调试 example 

1. 编译 example 

```
// -o 代表生成中间编译代码，并且生成可执行程序
cd ffmpeg源码/examples
clang encode_video.c -o ./test/encode_video -I/usr/local/include -L/usr/local/lib/ -lavformat -lavcodec -lavutil -lavfilter -lswscale

./test/encode_video ./test/test.mp4 libx264

```
__此方式编译可能无法断点__ 

```
cd ffmpeg源码/
make examples

```



## 参考
[Xcode调试ffmpeg源码(十五)](https://link.juejin.cn/?target=https%3A%2F%2Fwww.jianshu.com%2Fp%2F27a90b113413) 




