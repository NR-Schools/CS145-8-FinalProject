#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

enum SymbolDataType {
    INTEGER,
    DOUBLE
};

struct SymbolEntry {
    std::string name;
    SymbolDataType type;
    int scope;
};


typedef struct {
    std::vector<std::unordered_map<std::string, SymbolEntry>> scopes;
    int currentScope = 0;

    void addSymbol(const std::string& name, SymbolDataType type) {
        SymbolEntry entry;
        entry.name = name;
        entry.type = type;
        entry.scope = currentScope;

        // Add entry to symbol table
        scopes[currentScope][name] = entry;
    }

    SymbolEntry* lookup(const std::string& name) {
        for (int scope = currentScope; scope >= 0; --scope) {
            auto it = scopes[scope].find(name);
            if (it != scopes[scope].end()) {
                return &(it->second);
            }
        }
        return nullptr;
    }

    // Function to enter a new scope
    void enterScope() {
        currentScope++;
        if (currentScope >= static_cast<int>(scopes.size())) {
            scopes.push_back(std::unordered_map<std::string, SymbolEntry>());
        }
    }

    // Function to exit the current scope
    void exitScope() {
        if (currentScope > 0) {
            currentScope--;
        }
    }
} SymbolTable;
