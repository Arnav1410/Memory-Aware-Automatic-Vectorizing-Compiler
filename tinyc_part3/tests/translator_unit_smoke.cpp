#include "../translator.hpp"
#include <iostream>

using namespace tinyc3;

int main() {
    TargetInfo target = TargetInfo::hostAbi();
    Translator t(target);
    gTranslator = &t;

    /* Test a two-dimensional array declaration and a[i][j]-style first index. */
    DeclInfo d;
    d.name = "a";
    d.arrayDims.push_back(8);
    d.arrayDims.push_back(4);
    t.install(d, t.baseType(tinyc::TK_INT), NULL);

    ExprAttr *a = t.identifier("a");
    ExprAttr *i = t.constant("1");
    ExprAttr *x = t.index(a, i);
    ExprAttr *v = t.ensureRValue(x);

    delete a;
    delete i;
    if (!v || t.quads.code.size() == 0) return 1;
    if (v != x) delete x;
    delete v;

    /* Test that prefix and postfix ++ both consume their input expression
       and return a fresh value. */
    DeclInfo n;
    n.name = "n";
    t.install(n, t.baseType(tinyc::TK_INT), NULL);

    ExprAttr *n1 = t.identifier("n");
    ExprAttr *post = t.incdec(n1, true, false);
    delete post;

    ExprAttr *n2 = t.identifier("n");
    ExprAttr *pre = t.incdec(n2, true, true);
    delete pre;

    std::cout << "translator unit smoke: PASS\n";
    return 0;
}
