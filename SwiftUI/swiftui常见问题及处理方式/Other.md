
## ignore Safe Area 
* 方式1 
    ```swift
        Color("Purple").ignoresSafeArea()


    ```
* 方式2(__推荐这种方式__)
    ```swift 
    Color("Purple")
        .background(Color("Purple"))

    // 这种方式frame 将是 带有安全区的 
    ```
* 方式3 
    ```swift 
    Color("Purple")
        .overlay {
            Color("Purple").ignoresSafeArea()
        }

    ```

## 自定义 TabBar 的方式 
* 方式1 
    __使用系统的__ 
    ```swift 
    TabView(selection: $selected) {
        HomePage()
            .tag(0)
        HomeStorePage()
            .tag(1)
        SettingPage()
            .tag(2)
    }

    struct HomePage: View {
        var body: some View {
            NavigationStack {
                ContentView()
            }
            .tabItem {
                Label("Menu", systemImage: "list.dash")
            }
        }
    }
    ```


* 方式2 
    __Custom的方式1__ 
    ```swift 

    struct Main: View {
        // 先隐藏系统自带的 

        init() {
            UITabBar.appearance().isHidden  = true
        }
        
        @State var currentTab = "house.fill"
        
        
        var body: some View {
            VStack(spacing: 0) {
                // 这个是每个 tabbar 对应的 content view 

                TabView(selection: $currentTab) {
                    Text("Home")
                        .tag("house.fill")
                    Text("Search")
                        .tag("magnifyingglass")
                    ReelsView()
                        .tag("Reels")
                    
                    Text("Liked")
                        .tag("suit.heart")
                    Text("Profile")
                        .tag("person.circle")
                }

                // 自定义的 TabBar   

                HStack(spacing: 0) {
                    ForEach(["house.fill","magnifyingglass","video.badge.plus", "suit.heart", "person.circle"], id: \.self) { image in
                        TabBarButton(image: image, isSystemImage: true, currentTab: $currentTab)
                    }
                }
                .padding(.horizontal)
                .padding(.vertical, 10)
                
            }
        }
    }


    ```