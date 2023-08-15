# PhotosPicker 相关 

## view model 
```swift
class ViewModel: ObserableObject {
    @published var currentItem:PhotosPickerItem?  
    didSet {
        processPhoto(currentItem)
    }

    func processPhoto(photo: PhotosPickerItem) {
        photo.loadTransferable(type: Data.self) { result in 
            switch result {
                case .success(let data): 
                    if let data, let image = UIImage(data: data) {
                        
                    }
                    break 
                case .failure(let failure):
                    break
            }

        }
    }

}
```


## UI 
```swift 

PhotoPicker(selection: $viewModel.currentItem, matching: .any(of:[.images])) {

}


```



