

```
NSMetadataQuery *metadataQuery = [[NSMetadataQuery alloc] init]; 


设置一个监听函数来接受检索进度和结果：
NSNotificationCenter *nf = [NSNotificationCenter defaultCenter];
[nf addObserver:self selector:@selector(queryNote:) name:nil object:metadataQuery];


```


```
 (void)queryNote:(NSNotification *)note {
    // 查看 [note name], 可以获取事件类型
    if ([[note name] isEqualToString:NSMetadataQueryDidStartGatheringNotification]) {
        // 检索开始的消息
    } else if ([[note name] isEqualToString:NSMetadataQueryDidFinishGatheringNotification]) {
        // 检索完成的消息
    } else if ([[note name] isEqualToString:NSMetadataQueryGatheringProgressNotification]) {
        // 检索中...，会时时更新检索到的数据。NSMetadataQuery不会等把所有的数据检索完成才发数据，检索到一批后就会把数据更新，这就会减少检索时的等待。
    } else if ([[note name] isEqualToString:NSMetadataQueryDidUpdateNotification]) {
        // 当检索的文件目录范围内有文件操作时，比如新建、删除等操作时，会更新检索结果
    }
}


```

检索结果会放倒NSMetadataQuery的实例属性里面，比如metadataQuery.resultCount存储文件数量，metadataQuery.results存储所有的文件，results里面存储的是NSMetadataItem对象


## 开始检索 

```
如果检索时想要过滤掉一些查找，比如不去查找邮件信息
NSPredicate *predicateKind = [NSPredicate predicateWithFormat:@"kMDItemContentTypeTree == 'public.image'"];
NSPredicate *emailExclusionPredicate = [NSPredicate predicateWithFormat:@"(kMDItemContentType != 'com.apple.mail.emlx') && (kMDItemContentType != 'public.vcard')"];
predicateKind = [NSCompoundPredicate andPredicateWithSubpredicates:[NSArray arrayWithObjects:predicateKind, emailExclusionPredicate, nil]];

```


## 检索操作 

```
设置好NSPredicate后，把NSPredicate实例设置给metadataQuery：
[metadataQuery setPredicate:predicateKind];

设置检索目录：
[metadataQuery setSearchScopes:pathArray];

开始检索：
[metadataQuery startQuery];

如果想要停止检索：
[metadataQuery stopQuery];

```
