# CS145-8-FinalProject

##### For C Implementation
If using (vscode) coderunner, add this:
settings.json
```
{
    "code-runner.runInTerminal": true,
    "code-runner.executorMap": {
        "c": "cd $dir && gcc $fileName lint\\*.c lint\\helper_lib\\*.c -o .\\$fileNameWithoutExt && $dir$fileNameWithoutExt"
    }
}
```
