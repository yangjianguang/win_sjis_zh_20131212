#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "crowbar.h"

// 为字符串分配内存
static CRB_String * alloc_crb_string(CRB_Interpreter *inter, char *str, CRB_Boolean is_literal)
{
    CRB_String *ret;

    ret = MEM_malloc(sizeof(CRB_String));
    ret->ref_count = 0;
    ret->is_literal = is_literal;
    ret->string = str;

    return ret;
}

// 字面量的字符串
CRB_String * crb_literal_to_crb_string(CRB_Interpreter *inter, char *str)
{
    CRB_String *ret;

    ret = alloc_crb_string(inter, str, CRB_TRUE);
    ret->ref_count = 1;

    return ret;
}

// 为字符串添加引用计数
void crb_refer_string(CRB_String *str)
{
    str->ref_count++;
}

// 为字符串减少引用计数
void crb_release_string(CRB_String *str)
{
    str->ref_count--;

    DBG_assert(str->ref_count >= 0, ("str->ref_count..%d\n",
                                     str->ref_count));
    if (str->ref_count == 0) {
        if (!str->is_literal) {
            MEM_free(str->string);
        }
        MEM_free(str);
    }
}

// 创建一个字符串
CRB_String * crb_create_crowbar_string(CRB_Interpreter *inter, char *str)
{
    CRB_String *ret = alloc_crb_string(inter, str, CRB_FALSE);
    ret->ref_count = 1;

    return ret;
}
