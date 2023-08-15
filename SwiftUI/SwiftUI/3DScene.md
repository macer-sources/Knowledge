# 3D 支持 
> 使用 SceneKit 框架实现

## 1. 获取3D 资源 



## 2. SceneKit SwiftUI 封装
```swift 
import SceneKit 

struct CustomSceneKitView: UIViewRepresentable {
    @Binding var scene: SCNScene?
    var isTopPortion: Bool = false // 这里是将场景分为两个， 针对披萨打包的时候，如果是1个，那披萨会 overlay 在场景上边，
    // TODO: 具体使用几个场景，需要根据实际情况进行处理 

    func  makeUIView(context: Context) -> some SCNView {
        let view = SCNView() 
        view.scene = scene 

        view.backgroundColor = .clear 
        view.isJitteringEnabled = true 
        view.antialiasingMode = .multisampling4X 
        view.allowCameraControl = false 
        view.autoenablesDefaultLighting = true 

        if isTopPortion {
            view.scene?.rootNode.childNode(withName: "Closed_Box", recursively: true).isHidden = true
        }

        return view

    }

}



```

