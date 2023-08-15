# RTMP 源码分析  


## 示例工程 
1. 创建 cocoa demo 工程  
2. 下载 RTMP dump 将其中的librtmp 拖入示例工程   
3. 添加 openssl到工程  
    * 自己编译   
        
    * pod 
        `pod 'OpenSSL-Universal', '1.0.2.20'`

4. swift 工程(需要操作此步骤，OC 略过)  
    *  创建 module.modulemap 文件  
        ```
        module librtmp {
            header "librtmp/rtmp.h"
            export *
        }
        ```
    * 修改 swift inport search path 路径 





## 测试 
1. 安装 `Local RTMP server` 
2. ffmpeg 推流到地址 
    `ffmpeg -re -i /Users/caolongjian/Desktop/CCVideo.mp4  -vcodec copy -f flv rtmp://localhost:1935/abcs/room`