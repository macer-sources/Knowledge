

```

import Foundation

let sudo = Process()

// ls を実行してみる
// -S: パスワードを端末ではなく標準入力から読み込む
sudo.launchPath = "/usr/bin/sudo"
sudo.arguments = ["-S", "/bin/ls"]

// パイプを設定
let stdin = Pipe()
let stdout = Pipe()
sudo.standardOutput = stdout
sudo.standardError = stdout
sudo.standardInput = stdin

// 実行
sudo.launch()

// パスワードの末尾には改行が必要
let password = "password"
let passwordWithNewline = password + "\n"

// パスワードを標準入力へ書き込み
stdin.fileHandleForWriting.write(passwordWithNewline.data(using: .utf8)!)
try? stdin.fileHandleForWriting.close()

// 終了まで待機
sudo.waitUntilExit()

// 標準出力を取得
if let data = try? stdout.fileHandleForReading.readToEnd(),
   let output = String(data: data, encoding: .utf8) {
    print(output)
}


```