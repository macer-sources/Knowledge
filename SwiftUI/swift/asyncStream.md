# AsyncStream 

1. 作为属性存在 
```swift 




```



2. 方法返回值 






# AsyncThrowingStream 







# Task 
> 无论您是否保留对它的引用，任务都会运行。但是，如果您放弃对任务的引用，您将放弃等待该任务结果或取消该任务的能力。

__必须保存 task 才能取消任务__ 

```swift 
class Work {
    var task: Task <(), Never >?  

      func getSteps() -> AsyncStream<Int> {
            return .init { continutation in
            continutation.onTermination = { @Sendable _ in
                self.task?.cancel()
            }
            task = Task {
                var step: Int = 0
                for _ in 0..<1000 {
                continutation.yield(step)
                try? await Task.sleep(nanoseconds: 100000000)
                step += 1
                }
            }
            }
        }
}

class TaskViewController : UIViewController  {
    var task: Task <(), Never >? 
    let worker =  Worker ()  


    func doWork() {
        print("Task began")
        task = Task {
            for await step in worker.getSteps() {
                guard  let  self  =  self  else { return } 
                print(step)
            }
        }
    }
    deinit {
        task?.cancel()
        print("deinit called")
    }
}
```