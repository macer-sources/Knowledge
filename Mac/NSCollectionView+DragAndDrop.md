

## NSCollectionView Drag And Drop 

### 1. 初始化 collectionView 


```swift
    private func installSubviews() {
        collectionView.delegate = self
        collectionView.dataSource = self
        collectionView.collectionViewLayout = NSCollectionViewFlowLayout.init()
        collectionView.register(CollectionViewItem.self, forItemWithIdentifier: NSUserInterfaceItemIdentifier.init(rawValue: "NSCollectionViewItem"))
        view.addSubview(collectionView)
        collectionView.snp.makeConstraints { make in
            make.edges.equalToSuperview()
        }
    }
    func collectionView(_ collectionView: NSCollectionView, numberOfItemsInSection section: Int) -> Int {
        return 20
    }

    func collectionView(_ collectionView: NSCollectionView, itemForRepresentedObjectAt indexPath: IndexPath) -> NSCollectionViewItem {
        let cell = collectionView.makeItem(withIdentifier: .init(rawValue: "NSCollectionViewItem"), for: indexPath)
        return cell
    }
```



### 支持拖入 
1. 注册支持拖入的types 

```swift 
    private func registerForDraggedTypes() {
        collectionView.registerForDraggedTypes([
            .fileURL,
            .string
        ])
        collectionView.registerForDraggedTypes(Appearance.draggedTypes)
        collectionView.setDraggingSourceOperationMask([.copy], forLocal: true)
        collectionView.setDraggingSourceOperationMask([.copy, .delete], forLocal: false)
    }

```

2. 实现代理方法

```swift

    func collectionView(_ collectionView: NSCollectionView,
                        validateDrop draggingInfo: NSDraggingInfo,
                        proposedIndexPath proposedDropIndexPath: AutoreleasingUnsafeMutablePointer<NSIndexPath>,
                        dropOperation proposedDropOperation: UnsafeMutablePointer<NSCollectionView.DropOperation>) -> NSDragOperation {
        var dragOperation: NSDragOperation = []

        // The sample only supports dropping items between rows (not on top of a row).
        if proposedDropOperation.pointee != .on {
            let pasteboard = draggingInfo.draggingPasteboard
            
            //在试图内部进行拖动，代表的是移动操作
            if let draggingSource = draggingInfo.draggingSource as? NSCollectionView {
                if draggingSource == collectionView {
                    // Drag source came from our own table view.
                    dragOperation = [.move]
                }
            } else {
                // 拖动源来自其他应用程序
                guard let items = pasteboard.pasteboardItems else { return dragOperation }
                for item in items {
                    // 验证是否是支持的类型
                    if item.availableType(from: [(kUTTypeImage as NSPasteboard.PasteboardType)]) != nil {
                        // Drag source is coming from another app as a promised image file.
                        dragOperation = [.copy]
                    }
                }
            }
            
            // Has a drop operation been determined yet?
            // 补偿 操作
            if dragOperation == [] {
                // Look for possible URLs you can consume.
                let acceptedTypes = [kUTTypeImage]
                let options = [NSPasteboard.ReadingOptionKey.urlReadingFileURLsOnly: true,
                               NSPasteboard.ReadingOptionKey.urlReadingContentsConformToTypes: acceptedTypes]
                    as [NSPasteboard.ReadingOptionKey: Any]
                // Look only for image urls.
                if let urls = pasteboard.readObjects(forClasses: [NSURL.self], options: options) {
                    if !urls.isEmpty {
                        // 该示例允许这样做;用户可以在混合文件中拖动，其中任何一个是图像文件
                        dragOperation = [.copy]
                    }
                }
            }
        }
        return dragOperation
    }

 func collectionView(_ collectionView: NSCollectionView,
                        acceptDrop draggingInfo: NSDraggingInfo,
                        indexPath: IndexPath,
                        dropOperation: NSCollectionView.DropOperation) -> Bool {
        
        if let draggingSource = draggingInfo.draggingSource as? NSCollectionView, draggingSource == collectionView {
            // collectionView内部进行拖动， 移动操作
            dropInternalPhotos(collectionView, draggingInfo: draggingInfo, indexPath: indexPath)
        } else {
            // 外部拖入
            dropExternalPhotos(collectionView, draggingInfo: draggingInfo, indexPath: indexPath)
        }
        return true
    }

    // 此方法可以不需要
    func collectionView(_ collectionView: NSCollectionView,
                        draggingSession session: NSDraggingSession,
                        endedAt screenPoint: NSPoint,
                        dragOperation operation: NSDragOperation) {
        if operation == .delete, let items = session.draggingPasteboard.pasteboardItems {
            // User dragged the photo to the Finder's trash.
            for pasteboardItem in items {
                if let photoIdx = pasteboardItem.propertyList(forType: .itemDragType) as? Int {
                    // User dragged the photo at index 'photoIdx' to the Trash.
                    let indexPath = IndexPath(item: photoIdx, section: 0)
                    let photo = self.dataSource.itemIdentifier(for: indexPath)
                    Swift.debugPrint("Remove \(photo!.title)")
                }
            }
        }
    }

```


3. 一些辅助函数 

```swift

    func handlePromisedDrops(draggingInfo: NSDraggingInfo, toIndexPath: IndexPath) -> Bool {
        var handled = false
        if let promises = draggingInfo.draggingPasteboard.readObjects(forClasses: [NSFilePromiseReceiver.self], options: nil) {
            if !promises.isEmpty {
                // We have incoming drag items that are file promises.
                for promise in promises {
                    if let promiseReceiver = promise as? NSFilePromiseReceiver {
                        // Show the progress indicator as we start receiving this promised file.
                        progressIndicator.isHidden = false
                        progressIndicator.startAnimation(self)
                        
                        // Ask your file promise receiver to fulfill on their promise.
                        promiseReceiver.receivePromisedFiles(atDestination: destinationURL, options: [:],
                                                             operationQueue: self.filePromiseQueue) { (fileURL, error) in
                            /** Finished copying the promised file.
                                Back on the main thread, insert the newly created image file into the table view.
                            */
                            OperationQueue.main.addOperation {
                                if error != nil {
                                    self.reportURLError(fileURL, error: error!)
                                } else {
                                    _ = self.insertURL(fileURL, toIndexPath: toIndexPath)
                                }
                                // Stop the progress indicator as you are done receiving this promised file.
                                self.progressIndicator.isHidden = true
                                self.progressIndicator.stopAnimation(self)
                            }
                        }
                    }
                }
                handled = true
            }
        }
        return handled
    }
    
    func dropExternalPhotos(_ collectionView: NSCollectionView, draggingInfo: NSDraggingInfo, indexPath: IndexPath) {
        // Find the proper indexPath drop point for the external photos.
        let toIndexPath = dropLocation(indexPath: indexPath)

        // If possible, first handle the incoming dragged photos as file promises.
        if handlePromisedDrops(draggingInfo: draggingInfo, toIndexPath: toIndexPath) {
            // Successfully processed the dragged items that were promised.
        } else {
            // Incoming drag was not propmised, so move in all the outside dragged items as URLs.
            var foundNonImageFiles = false
            
            // Move in all the outside dragged items as URLs.
            draggingInfo.enumerateDraggingItems(
                options: NSDraggingItemEnumerationOptions.concurrent,
                for: collectionView,
                classes: [NSPasteboardItem.self],
                searchOptions: [:],
                using: {(draggingItem, idx, stop) in
                    if let pasteboardItem = draggingItem.item as? NSPasteboardItem,
                        // Are we being passed a file URL as the drag type?
                        let itemType = pasteboardItem.availableType(from: [.fileURL]),
                        let filePath = pasteboardItem.string(forType: itemType),
                        let url = URL(string: filePath) {
                            if !self.insertURL(url, toIndexPath: toIndexPath) {
                                foundNonImageFiles = true
                            }
                    }
                })
            
            // If any of the dragged URLs were not image files, alert the user.
            if foundNonImageFiles {
                let alert = NSAlert()
                alert.messageText = NSLocalizedString("CannotImportTitle", comment: "")
                alert.informativeText = NSLocalizedString("CannotImportMessage", comment: "")
                alert.addButton(withTitle: NSLocalizedString("OKTitle", comment: ""))
                alert.alertStyle = .warning
                alert.beginSheetModal(for: self.view.window!, completionHandler: nil)
            }
        }
    }
    func insertURL(_ url: URL, toIndexPath: IndexPath) -> Bool {
        var urlInserted = false
        do {
            let resourceValues = try url.resourceValues(forKeys: Set([.typeIdentifierKey]))
            var urlTypeConformsToImage = false
            
            if let typeIdentifier = resourceValues.typeIdentifier {
                // The file URL has a type identifier.
                if UTTypeConformsTo(typeIdentifier as CFString, kUTTypeImage) {
                    urlTypeConformsToImage = true
                }
            } else {
                // The file URL does not have a type identifier; use the extension to determine if it's an image type.
                let urlExtension = url.pathExtension
                let typeIdentifier =
                    UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, urlExtension as CFString, nil)
                if UTTypeConformsTo(typeIdentifier!.takeRetainedValue() as CFString, kUTTypeImage) {
                    urlTypeConformsToImage = true
                }
            }
            
            if urlTypeConformsToImage {
                // URL is an image file; add it to the collection view.
                var snapshot = self.dataSource.snapshot()
                
                let photoItem = PhotoItem(url: url)

                // Set yourself to be notified when the photo's thumbnail is ready.
                photoItem.thumbnailDelegate = self
                
                // Start to load the image.
                photoItem.loadImage()
                
                let dropItemLocation = snapshot.itemIdentifiers[toIndexPath.item]
                if toIndexPath.item == 0 {
                    // Item is being dropped at the beginning.
                    snapshot.insertItems([photoItem], beforeItem: dropItemLocation)
                } else {
                    // Item is being dropped between items or at the very end.
                    snapshot.insertItems([photoItem], afterItem: dropItemLocation)
                }
                self.dataSource.apply(snapshot, animatingDifferences: true)
                urlInserted = true
            }
        } catch {
            Swift.debugPrint("Can't obtain the type identifier for \(url): \(error)")
        }
        return urlInserted
    }

    func dropLocation(indexPath: IndexPath) -> IndexPath {
        var toIndexPath = indexPath
        if indexPath.item == 0 {
            toIndexPath = IndexPath(item: indexPath.item, section: indexPath.section)
        } else {
            toIndexPath = IndexPath(item: indexPath.item - 1, section: indexPath.section)
        }
        return toIndexPath
    }
    
    func dropInternalPhotos(_ collectionView: NSCollectionView, draggingInfo: NSDraggingInfo, indexPath: IndexPath) {
        var snapshot = self.dataSource.snapshot()

        draggingInfo.enumerateDraggingItems(
            options: NSDraggingItemEnumerationOptions.concurrent,
            for: collectionView,
            classes: [NSPasteboardItem.self],
            searchOptions: [:],
            using: {(draggingItem, idx, stop) in
                if let pasteboardItem = draggingItem.item as? NSPasteboardItem {
                    do {
                        if let indexPathData = pasteboardItem.data(forType: .itemDragType), let photoIndexPath =
                            try NSKeyedUnarchiver.unarchiveTopLevelObjectWithData(indexPathData) as? IndexPath {
                                if let photoItem = self.dataSource.itemIdentifier(for: photoIndexPath) {
                                    // Find out the proper indexPath drop point.
                                    let toIndexPath = self.dropLocation(indexPath: indexPath)
                                    
                                    let dropItemLocation = snapshot.itemIdentifiers[toIndexPath.item]
                                    if toIndexPath.item == 0 {
                                        // Item is being dropped at the beginning.
                                        snapshot.moveItem(photoItem, beforeItem: dropItemLocation)
                                    } else {
                                        // Item is being dropped between items or at the very end.
                                        snapshot.moveItem(photoItem, afterItem: dropItemLocation)
                                    }
                                }
                            }
                    } catch {
                        Swift.debugPrint("failed to unarchive indexPath for dropped photo item.")
                    }
                }
            })
        dataSource.apply(snapshot, animatingDifferences: true)
    }
```



------------------------------------------


### 支持拖出到其他view 
实现代理方法。拖入的那个view 需要实现drop source 来接收资源 

```swift

    func collectionView(_ collectionView: NSCollectionView, canDragItemsAt indexPaths: Set<IndexPath>, with event: NSEvent) -> Bool {
        return true
    }
    func collectionView(_ collectionView: NSCollectionView, pasteboardWriterForItemAt indexPath: IndexPath) -> NSPasteboardWriting? {
        // 1. 获取数据
//        let imageFile = imageDirectoryLoader.imageFileForIndexPath(indexPath)
//            return imageFile.url.absoluteURL
        // 1. 返回一个实现 NSPasteboardWriting 协议的对象， 返回nil 不允许拖动
        // let item = NSPasteboardItem.init()
        //item.setString("测试", forType: NSPasteboard.PasteboardType.string)
        //return item 
        //        UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension,photoFileExtension as CFString, nil)
        
        // 1. 获取item model
        
        // 2. NSKeyedArchiver
//        do {
//            let data = try NSKeyedArchiver.archivedData(withRootObject: indexPath, requiringSecureCoding: false)
//            provider!.userInfo = [FilePromiseProvider.UserInfoKeys.urlKey: photoItem.fileURL as Any,
//                                  FilePromiseProvider.UserInfoKeys.indexPathKey: data]
//        } catch {
//            fatalError("failed to archive indexPath to pasteboard")
//        }
        // 3. 设置到NSPasteboardItem 中

        //4. return item
    }


```






------------
## Other 


```swift 

extension ViewController: NSFilePromiseProviderDelegate {
    
    /** This function is called at drop time to provide the title of the file being dropped.
        This sample uses a hard-coded string for simplicity, but depending on your use case, you should take the fileType parameter into account.
    */
    func filePromiseProvider(_ filePromiseProvider: NSFilePromiseProvider, fileNameForType fileType: String) -> String {
        // Return the photoItem's URL file name.
        let photoItem = photoFromFilePromiserProvider(filePromiseProvider: filePromiseProvider)
        return (photoItem?.fileURL.lastPathComponent)!
    }
    
    func filePromiseProvider(_ filePromiseProvider: NSFilePromiseProvider,
                             writePromiseTo url: URL,
                             completionHandler: @escaping (Error?) -> Void) {
        do {
            if let photoItem = photoFromFilePromiserProvider(filePromiseProvider: filePromiseProvider) {
                /** Copy the file to the location provided to you. You always do a copy, not a move.
                    It's important you call the completion handler.
                */
                try FileManager.default.copyItem(at: photoItem.fileURL, to: url)
                completionHandler(nil)
            }
        } catch let error {
            OperationQueue.main.addOperation {
                self.presentError(error, modalFor: self.view.window!,
                                  delegate: nil, didPresent: nil, contextInfo: nil)
            }
            completionHandler(error)
        }
    }
    
    /** You should provide a non main operation queue (e.g. one you create) via this function.
        This way you don't stall the main thread while writing the promise file.
    */
    func operationQueue(for filePromiseProvider: NSFilePromiseProvider) -> OperationQueue {
        return filePromiseQueue
    }
    
    // Utility function to return a PhotoItem object from the NSFilePromiseProvider.
    func photoFromFilePromiserProvider(filePromiseProvider: NSFilePromiseProvider) -> PhotoItem? {
        var returnPhoto: PhotoItem?
        if let userInfo = filePromiseProvider.userInfo as? [String: AnyObject] {
            do {
                if let indexPathData = userInfo[FilePromiseProvider.UserInfoKeys.indexPathKey] as? Data {
                    if let indexPath =
                        try NSKeyedUnarchiver.unarchiveTopLevelObjectWithData(indexPathData) as? IndexPath {
                            returnPhoto = dataSource.itemIdentifier(for: indexPath)
                        }
                }
            } catch {
                fatalError("failed to unarchive indexPath from promise provider.")
            }
        }
        return returnPhoto
    }
    
}

```
