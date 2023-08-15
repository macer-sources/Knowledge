# Macos 执行shell 

```swift 

let taskQueue = DispatchQueue.global(qos: .background)
taskQueue.async {
  let task = Process()
  task.launchPath = Bundle.main.path(forResource: "xxx", ofType:"command")
  task.arguments = arguments // eg: selectedItem, buildLocation, httpProxy, etc.
  task.terminationHandler = { task in
    DispatchQueue.main.async(execute: {
      // finish code here
    }
  }
  let outputPipe = Pipe()
  task.standardOutput = outputPipe
  outputPipe.fileHandleForReading.waitForDataInBackgroundAndNotify()
  NotificationCenter.default.addObserver(forName: NSNotification.Name.NSFileHandleDataAvailable, object: outputPipe.fileHandleForReading , queue: nil) { notification in
      let output = outputPipe.fileHandleForReading.availableData
      if output.count > 0 {
          outputPipe.fileHandleForReading.waitForDataInBackgroundAndNotify()
          let outputString = String(data: output, encoding: String.Encoding.utf8) ?? ""
          // analyse outputString here
      }
  }
  self.buildTask.launch()
  self.buildTask.waitUntilExit()
}


```