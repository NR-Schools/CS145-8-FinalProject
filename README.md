# CS145-8-FinalProject

*Note: This will be first written in c/c++ to ensure that we have deliverables at the time of deadline

If using (vscode) coderunner, add this:

settings.json
```
{
    "code-runner.runInTerminal": true,
    "code-runner.executorMap": {
        "c": "cd $dir && gcc $fileName lint\\*.c -o .\\$fileNameWithoutExt && $dir$fileNameWithoutExt"
    }
}
```