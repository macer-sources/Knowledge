
```
// MARK: - NSTextFieldDelegate
    func control(_ control: NSControl, textView: NSTextView, doCommandBy commandSelector: Selector) -> Bool {
        if commandSelector == #selector(insertNewline(_:)) {
            if let modifierFlags = NSApplication.shared.currentEvent?.modifierFlags, (modifierFlags.rawValue & NSEvent.ModifierFlags.shift.rawValue) != 0 {
                print("Shift-Enter detected.")
                textView.insertNewlineIgnoringFieldEditor(self)
                return true
            } else {
                print("Enter detected.")
            }
        }
        return false
    }
    
    // MARK: - NSTextViewDelegate
    func textView(_ textView: NSTextView, doCommandBy commandSelector: Selector) -> Bool {
        if commandSelector == #selector(insertNewline(_:)) {
            if let modifierFlags = NSApplication.shared.currentEvent?.modifierFlags, (modifierFlags.rawValue & NSEvent.ModifierFlags.shift.rawValue) != 0 {
                print("Shift-Enter detected.")
                textView.insertNewlineIgnoringFieldEditor(self)
                return true
            } else {
                print("Enter detected.")
                return true
            }
        }
        return false
    }



```