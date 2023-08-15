# AVPacket 

__AVPacket保存的是解码前的数据，也就是压缩后的数据。__  该结构本身不直接包含数据，其有一个指向数据域的指针，FFmpeg中很多的数据结构都使用这种方法来管理数据。

AVPacket的使用通常主要是下面4个函数:
* `av_packet_alloc`: 创建一个AVPacket的实例，但该函数并不会为数据分配空间，其指向数据域的指针为NULL。通常调用`av_read_frame`将流中的数据读取到AVPacket中
* `av_packet_free`
    ```c++ 

    void av_packet_free(AVPacket **pkt)
        {
            if (!pkt || !*pkt)
                return;
        
            av_packet_unref(*pkt);
            av_freep(pkt);
    
    }
    ```
    > 首先将AVPacket指向的数据域的引用技术减1（数据域的引用技术减为0时会自动释放）接着，释放为AVPacket分配的空间。 

* `av_packet_ref` 
    ```c++
    int av_packet_ref(AVPacket *dst, const AVPacket *src)
    {
        int ret;
    
        ret = av_packet_copy_props(dst, src);
        if (ret < 0)
            return ret;
    
        if (!src->buf) {
            ret = packet_alloc(&dst->buf, src->size);
            if (ret < 0)
                goto fail;
            if (src->size)
                memcpy(dst->buf->data, src->data, src->size);
    
            dst->data = dst->buf->data;
        } else {
            dst->buf = av_buffer_ref(src->buf);
            if (!dst->buf) {
                ret = AVERROR(ENOMEM);
                goto fail;
            }
            dst->data = src->data;
        }
    
        dst->size = src->size;
    
        return 0;
    fail:
        av_packet_free_side_data(dst);
        return ret;
    }

    ```
    > 将src的字段值复制给dst，如果src的数据域是基于引用计数的，则dst也引用该数据域，并将引用计数+1;如果不是，则为dst新分配一个数据域，并将src数据域的值复制过去。dst的数据域是基于引用计数的。  

* `av_packet_free`  
    替代 已被废弃的`av_free_packet`，其功能是将packet指向的数据域的引用技术减1，并将packe的值设为默认值。该函数并不会释放packet的空间，释放不使用的packet需要调用`av_packet_free`。  
    > 解码流程 
    ```c++

        AVPacket *packet = av_packet_alloc(); // 创建一个packet
        while(av_read_frame(pFormatCtx,packet))
        {
            if(packet->stream_index == audio_index)
            {
                ...
            }
            else if(packet->stream_index == video_index)
            {
                ...
            }

            av_packet_unref(packet); // 不要忘记减少引用技术
        }

        av_packet_free(packet);
    ```

    在使用`av_packet_alloc`创建packet的时候，并没有给数据域分配空间，数据域的空间实在`av_read_frame`内分配的，所以在每次循环的结束不能忘记调用`av_packet_unref`减少数据域的引用技术，当引用技术减为0时，会自动释放数据域所占用的空间。在循环结束后，调用`av_packet_free`来释放AVPacket本身所占用的空间。

    将数据从流中读取到packet的时候，通常需要缓存AVPacket，也就意味着要转移AVPacket本身的字段值以及其指向的数据域的引用。  

    ```c++
    AVPacket *pkt = av_packet_alloc();
    if (av_packet_ref(pkt, packet) < 0)
        return false;

    queue.push(*pkt);

    ```
缓存AVPacket时，可以新创建一个AVPacket实例，然后调用av_packet_ref复制目标packet的值。

