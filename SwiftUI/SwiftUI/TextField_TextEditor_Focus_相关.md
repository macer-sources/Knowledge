## iOS 
```swift 
struct ContentView: View {
    enum FocusedField {
        case firstName, lastName
    }

    @State private var firstName = ""
    @State private var lastName = ""
    @FocusState private var focusedField: FocusedField?

    var body: some View {
        Form {
            TextField("First name", text: $firstName)
                .focused($focusedField, equals: .firstName)

            TextField("Last name", text: $lastName)
                .focused($focusedField, equals: .lastName)
        }
        .onAppear {
            focusedField = .firstName
        }
    }
}

```


## macOS

```swift 
struct ContentView: View {
    enum FocusedField {
        case firstName, lastName
    }

    @State private var firstName = ""
    @State private var lastName = ""
    @FocusState private var focusedField: FocusedField?

    var body: some View {
        Form {
            TextField("First name", text: $firstName)
                .focused($focusedField, equals: .firstName)

            TextField("Last name", text: $lastName)
                .focused($focusedField, equals: .lastName)
        }
        .defaultFocus($focusedField, .firstName) // 设置默认焦点
    }
}

```