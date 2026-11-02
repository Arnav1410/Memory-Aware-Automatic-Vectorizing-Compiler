#ifndef TINYC_TARGET_INFO_H
#define TINYC_TARGET_INFO_H

#include <cstddef>
#include <string>
#include <vector>

namespace tinyc {

enum BaseKind { TK_VOID, TK_CHAR, TK_INT, TK_FLOAT, TK_BOOL, TK_POINTER, TK_ARRAY, TK_FUNCTION };

struct Type {
    BaseKind kind;
    Type *element;
    std::size_t arrayCount;
    Type *returnType;
    std::vector<std::pair<std::string, Type*> > params;

    Type(BaseKind k) : kind(k), element(NULL), arrayCount(0), returnType(NULL) {}

    static Type *make(BaseKind kind);
    static Type *makePointer(Type *element);
    static Type *makeArray(Type *element, std::size_t count);
    static Type *makeFunction(Type *returnType,
                              const std::vector<std::pair<std::string, Type*> >& params);
    static void clearPool();

    std::string str() const;
};

typedef Type *TypePtr;

struct TargetInfo {
    std::size_t charSize;
    std::size_t intSize;
    std::size_t floatSize;
    std::size_t pointerSize;
    std::size_t stackAlignment;

    TargetInfo();

    static TargetInfo hostAbi();
    static TargetInfo fromConfigFile(const std::string& path);

    std::size_t sizeOf(TypePtr type) const;
    std::size_t alignOf(TypePtr type) const;
    std::string describe() const;
};

} // namespace tinyc

#endif
