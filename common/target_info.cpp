#include "target_info.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace tinyc {

static std::vector<Type*> typePool;

static Type *saveType(Type *t) {
    typePool.push_back(t);
    return t;
}

Type *Type::make(BaseKind kind) {
    return saveType(new Type(kind));
}

Type *Type::makePointer(Type *elementType) {
    Type *t = new Type(TK_POINTER);
    t->element = elementType;
    return saveType(t);
}

Type *Type::makeArray(Type *elementType, std::size_t count) {
    Type *t = new Type(TK_ARRAY);
    t->element = elementType;
    t->arrayCount = count;
    return saveType(t);
}

Type *Type::makeFunction(Type *returnType_,
                         const std::vector<std::pair<std::string, Type*> >& params_) {
    Type *t = new Type(TK_FUNCTION);
    t->returnType = returnType_;
    t->params = params_;
    return saveType(t);
}

void Type::clearPool() {
    std::size_t i;
    for (i = 0; i < typePool.size(); ++i) delete typePool[i];
    typePool.clear();
}

std::string Type::str() const {
    switch (kind) {
        case TK_VOID: return "void";
        case TK_CHAR: return "char";
        case TK_INT: return "int";
        case TK_FLOAT: return "float";
        case TK_BOOL: return "bool";
        case TK_POINTER:
            return std::string("ptr(") + (element ? element->str() : "?") + ")";
        case TK_ARRAY:
            return "array(" + std::to_string(arrayCount) + ", " +
                   (element ? element->str() : "?") + ")";
        case TK_FUNCTION: {
            std::string s = "function(" + (returnType ? returnType->str() : "?") + ": ";
            for (std::size_t i = 0; i < params.size(); ++i) {
                if (i != 0) s += ", ";
                s += params[i].second->str();
            }
            s += ")";
            return s;
        }
    }
    return "?";
}

TargetInfo::TargetInfo()
    : charSize(0), intSize(0), floatSize(0), pointerSize(0), stackAlignment(0) {}

TargetInfo TargetInfo::hostAbi() {
    TargetInfo t;
    t.charSize = sizeof(char);
    t.intSize = sizeof(int);
    // TinyC Part 3 defines float as an 8-byte teaching-language type.
    // The target configuration, not the semantic translator, controls the size.
    t.floatSize = sizeof(double);
    t.pointerSize = sizeof(void*);
    t.stackAlignment = alignof(std::max_align_t);
    return t;
}

static void trim(std::string *s) {
    const char *ws = " \t\r\n";
    std::size_t first = s->find_first_not_of(ws);
    if (first == std::string::npos) {
        s->clear();
        return;
    }
    std::size_t last = s->find_last_not_of(ws);
    *s = s->substr(first, last - first + 1);
}

TargetInfo TargetInfo::fromConfigFile(const std::string& path) {
    std::ifstream in(path.c_str());
    if (!in) throw std::runtime_error("cannot open target config: " + path);

    TargetInfo t;
    std::string line;
    while (std::getline(in, line)) {
        std::size_t comment = line.find('#');
        if (comment != std::string::npos) line.erase(comment);
        std::size_t eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        trim(&key);
        trim(&value);

        if (key == "char_size") t.charSize = std::stoull(value);
        else if (key == "int_size") t.intSize = std::stoull(value);
        else if (key == "float_size") t.floatSize = std::stoull(value);
        else if (key == "pointer_size") t.pointerSize = std::stoull(value);
        else if (key == "stack_alignment") t.stackAlignment = std::stoull(value);
        else throw std::runtime_error("unknown target key: " + key);
    }

    if (t.charSize == 0 || t.intSize == 0 || t.floatSize == 0 ||
        t.pointerSize == 0 || t.stackAlignment == 0) {
        throw std::runtime_error("target config is incomplete");
    }
    return t;
}

std::size_t TargetInfo::sizeOf(TypePtr type) const {
    if (!type) throw std::runtime_error("null type in sizeOf");
    switch (type->kind) {
        case TK_VOID: return 0;
        case TK_CHAR: return charSize;
        case TK_INT: return intSize;
        case TK_FLOAT: return floatSize;
        case TK_BOOL: return 0;
        case TK_POINTER: return pointerSize;
        case TK_ARRAY: return type->arrayCount * sizeOf(type->element);
        case TK_FUNCTION: return 0;
    }
    return 0;
}

std::size_t TargetInfo::alignOf(TypePtr type) const {
    if (!type) throw std::runtime_error("null type in alignOf");
    switch (type->kind) {
        case TK_VOID:
        case TK_BOOL:
        case TK_FUNCTION: return 1;
        case TK_CHAR: return charSize;
        case TK_INT: return intSize;
        case TK_FLOAT: return floatSize;
        case TK_POINTER: return pointerSize;
        case TK_ARRAY: return alignOf(type->element);
    }
    return 1;
}

std::string TargetInfo::describe() const {
    std::ostringstream out;
    out << "char=" << charSize
        << " int=" << intSize
        << " float=" << floatSize
        << " pointer=" << pointerSize
        << " stack_alignment=" << stackAlignment;
    return out.str();
}

} // namespace tinyc
