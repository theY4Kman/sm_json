# SourceMod JSON Extension

A simple extension to parse and read JSON-encoded data. Relies on the wonderful [jansson](http://www.digip.org/jansson/) C JSON library.

## Basic Usage

```c++
#include <sourcemod>
#include <json>

public OnPluginStart()
{
    decl String:path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "plugins/mydata.json");
    
    new Handle:fp = OpenFile(path, "r");
    decl String:contents[2048];
    ReadFileString(fp, contents, sizeof(contents));
    
    new Handle:root = JSON_Parse(contents);
    
    for (new Handle:iter=JSON_ObjectIter(root); iter!=INVALID_HANDLE; iter=JSON_ObjectIterNext(iter))
    {
        decl String:key[128];
        decl Handle:value;
        decl String:repr[1024];
        
        value = JSON_ObjectIterRead(iter, key, sizeof(key));
        JSON_Dumps(value, repr, sizeof(repr));
        
        PrintToServer("key: \"%s\"  value: %s", key, repr);
        
        CloseHandle(value);
    }
}
```