# Table 

1. 创建数据对象 
```swift 
struct User: Identifiable {
    let id: Int
    var name: String
    var score: Int
}
```

2. 创建表格 
```swift 
Table(users) {
  TableColumn("Name", value: \.name)// 针对 string 类型
  TableColumn("Score") { user in
      Text(String(user.score))
  }
 }
```
* 针对数据类型是 `String ` 的，可以直接使用 `Key-Value` 获取到值 `TableColumn("Name", value: \.name)` 
* 针对非 String 类型的， 需要通过自定义 View 进行实现 Column
* UI 样式调整。
  * width
  ```swift 
  TableColumn("Score", value: \.score).width(100)
   TableColumn("Score", value: \.score).width(min: 50, max: 100)
  ```


3. 添加数据集 
```swift 
@State private var users = [
        User(id: 1, name: "Taylor Swift", score: 95),
        User(id: 2, name: "Justin Bieber", score: 80),
        User(id: 3, name: "Adele Adkins", score: 85)
    ]
```

* 如果数据不可更改，不可排序，可以直接添加为静态数据 


4. 选中 
```swift 
 @State private var selected: User.ID? // 单选 
@State private var selected = Set<User.ID.ID>() // 多选

Table(users, selection: $selected) { ... }
```

5. 排序 
   1. KeyPathComparator使用默认排序创建一个对象数组。
   2. 将其作为sortOrder参数添加到您的Table初始化程序中。
   3. 为“分数”列添加特定的键路径，以便 SwiftUI 理解排序的含义。
   4. 注意排序顺序的变化，并根据需要对数组进行排序。我们已经使用 标记了我们的数组@State，因此我们可以就地对其进行排序。

```swift 
 @State private var sortOrder = [KeyPathComparator(\User.name)]

 Table(users, selection: $selection, sortOrder: $sortOrder) { }
  .onChange(of: sortOrder) { newOrder in
            users.sort(using: newOrder)
   }
```

6. Row 显示问题 
> 默认情况下，通过 `TableColumn`创建时， 会自动填充 Row 数据 。但是如果想添加类型`First/Last`这样的静态数据 
```swift 
        Table(selection: $selection, sortOrder: $sortOrder) {
            TableColumn("Name", value: \.name)
            TableColumn("Score", value: \.score) { user in
                Text("test\(String(user.score))" )
            }
            .width(min: 50, max: 100)
        } rows: {
            // TODO: 在数据集上下插入静态数据 First/ Last
            TableRow(User(id: 0, name: "First", score: 0))
            ForEach(users, content: TableRow.init)
            TableRow(User(id: 4, name: "Last", score: 100))
        }
        .onChange(of: sortOrder) { newOrder in
            users.sort(using: newOrder)
        }


```

