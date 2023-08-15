## 大纲
* UICollectionViewDiffableDataSource以新换旧UICollectionViewDataSource 
* 使用NSDiffableDataSourceSnapshot
* 将部分添加到数据源
* 向数据源添加补充视图

此外，使用这种新型数据源对变化进行动画处理是非常容易  



## UICollectionViewDiffableDataSource
UICollectionViewDiffableDataSource抽象了大量UICollectionViewDataSource的逻辑， 为客户端代码错误留下了更小的空间 
diffable部分意味着`UICollectionViewDiffableDataSource`每当更新正在显示的项目时，集合视图将自动计算更新后的集合与之前显示的集合之间的差异。这将反过来导致集合视图对更改进行动画处理，例如更新、插入和删除。

### 优点
1. 自动数据变化动画  
        每当您添加、更新或删除数据时，您都可以自动获得数据变化动画 
2. 自动数据同步 
    要在没有 `UICollectionViewDiffableDataSource` 的情况下利用集合视图的标准动画.必须手动管理和同步集合视图与数据源之间的数据更改, 如果在其中一个同步操作中出现偏差，会看到如下错误" 
    ![](/i/442d9b8a-96ca-4ac9-ae73-5e08f4ecfe6c.jpg)

3. 减少代码
    可以编写更少的代码并受益于集合视图的数据更改动画和数据同步
    
    
    
#### 创建 data source
`UICollectionViewDiffableDataSource`有两种通用类型： `section`和 `item` 类型。 

* 创建枚举 

```swift
enum Section {
  case main
}

```
为了简洁起见，为数据源创建一个类型别名。这减少了在UICollectionViewDiffableDataSource需要配置数据源以及每次需要引用相同数据类型时编写值类型的需要。

```swift

class Video: Hashable {
func hash(into hasher: inout Hasher) {
  hasher.combine(id)
}

static func == (lhs: Video, rhs: Video) -> Bool {
  lhs.id == rhs.id
}
}
typealias DataSource = UICollectionViewDiffableDataSource<Section, Video>
 
``` 
> `Section`  为之上创建的枚举， `Video`为对应的数据源对象; 数据源对象必须实现`Hashable`协议，这将在diffable 数据源进行添加、删除或更新视频时，需要确定两个元素是否彼此相等
    
    
    
    
* 配置数据源
```
func makeDataSource() -> DataSource {
  // 1
  let dataSource = DataSource(
    collectionView: collectionView,
    cellProvider: { (collectionView, indexPath, video) ->
      UICollectionViewCell? in
      // 2
      let cell = collectionView.dequeueReusableCell(
        withReuseIdentifier: "VideoCollectionViewCell",
        for: indexPath) as? VideoCollectionViewCell
      cell?.video = video
      return cell
  })
  return dataSource
}

``` 
> 在cellProvider回调中编写的代码与`collectionView(_:cellForItemAt:)`中相同
> 通过以上一个方法，代替 `collectionView(_: numberOfItemsInSection:)` 和 `collectionView(_:cellForItemAt:)`方法
    
 * 使用数据源
 ```swift
 private lazy var dataSource = makeDataSource() 
 ```
> 此时，需要删除掉原有的 collectionView(_:didSelectItemAt:) 中的 `let video = videoList[indexPath.row]`
> 以及 `guard  let video = dataSource.itemIdentifier(for: indexPath) else { return }` 等直接操作数组对象的 
   
__UICollectionViewDiffableDataSource可能会在后台工作，从而videoList与当前显示的数据不一致。__ 
      
    
    
    

## NSDiffableDataSourceSnapshot
    在集合视图可以显示任何单元格之前，需要告诉它想要显示哪些数据。这就是快照的用武之地！
    `NSDiffableDataSourceSnapshot` 存储了`Sections` 和 `Items`, `UICollectionViewDiffableDataSource` 将引用这些内容，以决定将现实什么内容 
    
 * 先创建一个别名 
    `typealias Snapshot = NSDiffableDataSourceSnapshot<Section, Video>`
    
 * 创建 Snapshot 
```
// 1
func applySnapshot(animatingDifferences: Bool = true) {
  // 2
  var snapshot = Snapshot()
  // 3
  snapshot.appendSections([.main])
  // 4 videoList为一个数组，相当于之前的 datasource 数组
  snapshot.appendItems(videoList)
  // 5
  dataSource.apply(snapshot, animatingDifferences: animatingDifferences)
}
```
    > 创建一个将快照应用于数据源的新方法。该方法采用一个布尔值来确定对数据源的更改是否应该动画。 
    > 创建一个新Snapshot对象
    > 将.main部分添加到快照。这是您当前为应用程序定义的唯一部分类型。 
    > 将视频数组添加到快照中。
    > 告知dataSource有关最新快照的信息，以便它可以相应地更新和动画。
    
    __通过此种方式刷新所有数据__
    
* 调用方法
 `viewDidLoad()`中调用以下方法`applySnapshot(animatingDifferences：false)`


### 增量更新 
```
// Get the current state of the UI from the data source.
var updatedSnapshot = dataSource.snapshot()

// For each section, if needed, step through and perform the next sorting step.
updatedSnapshot.sectionIdentifiers.forEach {
    let section = $0
    if !section.isSorted {

        // Step the sort algorithm.
        section.sortNext()
        let items = section.values

        // Replace the items for this section with the newly sorted items.
        updatedSnapshot.deleteItems(items)
        updatedSnapshot.appendItems(items, toSection: section)

        sectionCountNeedingSort += 1
    }
}

```





## More (多 section实现方式) 
1. 通过枚举实现 
```
    enum SectionLayoutKind: Int, CaseIterable {
        case list, grid5, grid3
        var columnCount: Int {
            switch self {
            case .grid3:
                return 3

            case .grid5:
                return 5

            case .list:
                return 1
            }
        }
    }

```
2. 通过自定义 section 类实现 

### Section 部分，不止可以通过 enum 实现， 还可以通过定义类来实现。
```
import UIKit
// 1
class Section: Hashable {
  var id = UUID()
  // 2
  var title: String
  var videos: [Video]
  
  init(title: String, videos: [Video]) {
    self.title = title
    self.videos = videos
  }
  
  func hash(into hasher: inout Hasher) {
    hasher.combine(id)
  }
  
  static func == (lhs: Section, rhs: Section) -> Bool {
    lhs.id == rhs.id
  }
}

```
为了测试添加的mock 数据，这部分实际是通过服务器获取
```
extension Section {
  static var allSections: [Section] = [
    Section(title: "SwiftUI", videos: [
      Video(
        title: "SwiftUI",
        thumbnail: UIImage(named: "swiftui"),
        lessonCount: 37,
        link: URL(string: "https://www.raywenderlich.com/4001741-swiftui")
      )
    ]),
    Section(title: "UIKit", videos: [
      Video(
        title: "Demystifying Views in iOS",
        thumbnail: UIImage(named: "views"),
        lessonCount: 26,
        link:
        URL(string:
          "https://www.raywenderlich.com/4518-demystifying-views-in-ios")
      ),
      Video(
        title: "Reproducing Popular iOS Controls",
        thumbnail: UIImage(named: "controls"),
        lessonCount: 31,
        link: URL(string: """
          https://www.raywenderlich.com/5298-reproducing
          -popular-ios-controls
          """)
      )
    ]),
    Section(title: "Frameworks", videos: [
      Video(
        title: "Fastlane for iOS",
        thumbnail: UIImage(named: "fastlane"),
        lessonCount: 44,
        link: URL(string:
          "https://www.raywenderlich.com/1259223-fastlane-for-ios")
      ),
      Video(
        title: "Beginning RxSwift",
        thumbnail: UIImage(named: "rxswift"),
        lessonCount: 39,
        link: URL(string:
          "https://www.raywenderlich.com/4743-beginning-rxswift")
      )
    ]),
    Section(title: "Miscellaneous", videos: [
      Video(
        title: "Data Structures & Algorithms in Swift",
        thumbnail: UIImage(named: "datastructures"),
        lessonCount: 29,
        link: URL(string: """
          https://www.raywenderlich.com/977854-data-structures
          -algorithms-in-swift
        """)
      ),
      Video(
        title: "Beginning ARKit",
        thumbnail: UIImage(named: "arkit"),
        lessonCount: 46,
        link: URL(string:
          "https://www.raywenderlich.com/737368-beginning-arkit")
      ),
      Video(
        title: "Machine Learning in iOS",
        thumbnail: UIImage(named: "machinelearning"),
        lessonCount: 15,
        link: URL(string: """
          https://www.raywenderlich.com/1320561-machine-learning-in-ios
        """)
      ),
      Video(
        title: "Push Notifications",
        thumbnail: UIImage(named: "notifications"),
        lessonCount: 33,
        link: URL(string:
          "https://www.raywenderlich.com/1258151-push-notifications")
      ),
    ])
  ]
}
```
使用 
```
private var sections = Section.allSections

// 替换 pplySnapshot(animatingDifferences:)方法中的内容 
snapshot.appendSections([.main])
snapshot.appendItems(videos)
为 
snapshot.appendSections(sections)
sections.forEach { section in
  snapshot.appendItems(section.videos, toSection: section)
}
```

-----------------------------
## 添加 header view 
1. 创建 `UICollectionReusableView`子类 
2. register 注册 
```
collectionView.register(
  SectionHeaderReusableView.self, 
  forSupplementaryViewOfKind: UICollectionView.elementKindSectionHeader, 
  withReuseIdentifier: SectionHeaderReusableView.reuseIdentifier
)
```
3. 补充标题视图设置
```
// Supplementary header view setup
let headerFooterSize = NSCollectionLayoutSize(
  widthDimension: .fractionalWidth(1.0), 
  heightDimension: .estimated(20)
)
let sectionHeader = NSCollectionLayoutBoundarySupplementaryItem(
  layoutSize: headerFooterSize, 
  elementKind: UICollectionView.elementKindSectionHeader, 
  alignment: .top
)
section.boundarySupplementaryItems = [sectionHeader]

```
4. `DataSource`中添加相关配置 
```
// 1
dataSource.supplementaryViewProvider = { collectionView, kind, indexPath in
  // 2
  guard kind == UICollectionView.elementKindSectionHeader else {
    return nil
  }
  // 3
  let view = collectionView.dequeueReusableSupplementaryView(
    ofKind: kind,
    withReuseIdentifier: SectionHeaderReusableView.reuseIdentifier,
    for: indexPath) as? SectionHeaderReusableView
  // 4
  let section = self.dataSource.snapshot()
    .sectionIdentifiers[indexPath.section]
  view?.titleLabel.text = section.title
  return view
}
```

