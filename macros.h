#ifndef __FU_MACROS_H
#define __FU_MACROS_H

#define xstr(X) #X
#define STRINGIFY(X) xstr (X)

// Check if macro __VA_ARGS__ is empty
#define _ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define HAS_COMMA(...) _ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define _TRIGGER_PARENTHESIS_(...) ,
 
/* IFNEMPTY courtesy of
 * https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
 */
#define IFNEMPTY(...)                                                    \
_IFNEMPTY(                                                               \
          /* test if there is just one argument, eventually an empty    \
             one */                                                     \
          HAS_COMMA(__VA_ARGS__),                                       \
          /* test if _TRIGGER_PARENTHESIS_ together with the argument   \
             adds a comma */                                            \
          HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
          /* test if the argument together with a parenthesis           \
             adds a comma */                                            \
          HAS_COMMA(__VA_ARGS__ (/*empty*/)),                           \
          /* test if placing it between _TRIGGER_PARENTHESIS_ and the   \
             parenthesis adds a comma */                                \
          HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/))      \
          )
#define BLANK_COMMA(...) _ARG16(__VA_ARGS__,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  , ())
 
#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _IFNEMPTY(_0, _1, _2, _3) PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3)
#define _IS_EMPTY_CASE_0000(X) X
#define _IS_EMPTY_CASE_0001(X) 
#define _IS_EMPTY_CASE_0010(X) X
#define _IS_EMPTY_CASE_0011(X) X
#define _IS_EMPTY_CASE_0100(X) X
#define _IS_EMPTY_CASE_0101(X) X
#define _IS_EMPTY_CASE_1000(X) X
#define _IS_EMPTY_CASE_1001(X) X
#define _IS_EMPTY_CASE_1010(X) X
#define _IS_EMPTY_CASE_1011(X) X
#define _IS_EMPTY_CASE_1100(X) X
#define _IS_EMPTY_CASE_1101(X) X
#define _IS_EMPTY_CASE_1110(X) X
#define _IS_EMPTY_CASE_1111(X) X
// end check

// Make a FOREACH macro
#define FE_1(WHAT, X) WHAT(X) 
#define FE_2(WHAT, X, ...) WHAT(X)FE_1(WHAT, __VA_ARGS__)
#define FE_3(WHAT, X, ...) WHAT(X)FE_2(WHAT, __VA_ARGS__)
#define FE_4(WHAT, X, ...) WHAT(X)FE_3(WHAT, __VA_ARGS__)
#define FE_5(WHAT, X, ...) WHAT(X)FE_4(WHAT, __VA_ARGS__)
#define FE_6(WHAT, X, ...) WHAT(X)FE_5(WHAT, __VA_ARGS__)
#define FE_7(WHAT, X, ...) WHAT(X)FE_6(WHAT, __VA_ARGS__)
#define FE_8(WHAT, X, ...) WHAT(X)FE_7(WHAT, __VA_ARGS__)
#define FE_9(WHAT, X, ...) WHAT(X)FE_8(WHAT, __VA_ARGS__)
#define FE_10(WHAT, X, ...) WHAT(X)FE_9(WHAT, __VA_ARGS__)
#define FE_11(WHAT, X, ...) WHAT(X)FE_10(WHAT, __VA_ARGS__)
#define FE_12(WHAT, X, ...) WHAT(X)FE_11(WHAT, __VA_ARGS__)
#define FE_13(WHAT, X, ...) WHAT(X)FE_12(WHAT, __VA_ARGS__)
#define FE_14(WHAT, X, ...) WHAT(X)FE_13(WHAT, __VA_ARGS__)
#define FE_15(WHAT, X, ...) WHAT(X)FE_14(WHAT, __VA_ARGS__)
#define FE_16(WHAT, X, ...) WHAT(X)FE_15(WHAT, __VA_ARGS__)
#define FE_17(WHAT, X, ...) WHAT(X)FE_16(WHAT, __VA_ARGS__)
#define FE_18(WHAT, X, ...) WHAT(X)FE_17(WHAT, __VA_ARGS__)
#define FE_19(WHAT, X, ...) WHAT(X)FE_18(WHAT, __VA_ARGS__)
#define FE_20(WHAT, X, ...) WHAT(X)FE_19(WHAT, __VA_ARGS__)
//... repeat as needed

// Make an ENUMERATE macro
#define EN_1(WHAT, X) WHAT(0, X)
#define EN_2(WHAT, X, ...) WHAT(1, X)EN_1(WHAT, __VA_ARGS__)
#define EN_3(WHAT, X, ...) WHAT(2, X)EN_2(WHAT, __VA_ARGS__)
#define EN_4(WHAT, X, ...) WHAT(3, X)EN_3(WHAT, __VA_ARGS__)
#define EN_5(WHAT, X, ...) WHAT(4, X)EN_4(WHAT, __VA_ARGS__)
#define EN_6(WHAT, X, ...) WHAT(5, X)EN_5(WHAT, __VA_ARGS__)
#define EN_7(WHAT, X, ...) WHAT(6, X)EN_6(WHAT, __VA_ARGS__)
#define EN_8(WHAT, X, ...) WHAT(7, X)EN_7(WHAT, __VA_ARGS__)
#define EN_9(WHAT, X, ...) WHAT(8, X)EN_8(WHAT, __VA_ARGS__)
#define EN_10(WHAT, X, ...) WHAT(9, X)EN_9(WHAT, __VA_ARGS__)
#define EN_11(WHAT, X, ...) WHAT(10, X)EN_10(WHAT, __VA_ARGS__)
#define EN_12(WHAT, X, ...) WHAT(11, X)EN_11(WHAT, __VA_ARGS__)
#define EN_13(WHAT, X, ...) WHAT(12, X)EN_12(WHAT, __VA_ARGS__)
#define EN_14(WHAT, X, ...) WHAT(13, X)EN_13(WHAT, __VA_ARGS__)
#define EN_15(WHAT, X, ...) WHAT(14, X)EN_14(WHAT, __VA_ARGS__)
#define EN_16(WHAT, X, ...) WHAT(15, X)EN_15(WHAT, __VA_ARGS__)
#define EN_17(WHAT, X, ...) WHAT(16, X)EN_16(WHAT, __VA_ARGS__)
#define EN_18(WHAT, X, ...) WHAT(17, X)EN_17(WHAT, __VA_ARGS__)
#define EN_19(WHAT, X, ...) WHAT(18, X)EN_18(WHAT, __VA_ARGS__)
#define EN_20(WHAT, X, ...) WHAT(19, X)EN_19(WHAT, __VA_ARGS__)
//... repeat as needed

// Make a for pairs macro
#define FP_2(WHAT, X, Y) WHAT(X, Y)
#define FP_4(WHAT, X, Y, ...) WHAT(X, Y)FP_2(WHAT, __VA_ARGS__)
#define FP_6(WHAT, X, Y, ...) WHAT(X, Y)FP_4(WHAT, __VA_ARGS__)
#define FP_8(WHAT, X, Y, ...) WHAT(X, Y)FP_6(WHAT, __VA_ARGS__)
#define FP_10(WHAT, X, Y, ...) WHAT(X, Y)FP_8(WHAT, __VA_ARGS__)
#define FP_12(WHAT, X, Y, ...) WHAT(X, Y)FP_10(WHAT, __VA_ARGS__)
#define FP_14(WHAT, X, Y, ...) WHAT(X, Y)FP_12(WHAT, __VA_ARGS__)
#define FP_16(WHAT, X, Y, ...) WHAT(X, Y)FP_14(WHAT, __VA_ARGS__)
#define FP_18(WHAT, X, Y, ...) WHAT(X, Y)FP_16(WHAT, __VA_ARGS__)
#define FP_20(WHAT, X, Y, ...) WHAT(X, Y)FP_18(WHAT, __VA_ARGS__)
#define FP_22(WHAT, X, Y, ...) WHAT(X, Y)FP_20(WHAT, __VA_ARGS__)
#define FP_24(WHAT, X, Y, ...) WHAT(X, Y)FP_22(WHAT, __VA_ARGS__)
#define FP_26(WHAT, X, Y, ...) WHAT(X, Y)FP_24(WHAT, __VA_ARGS__)
#define FP_28(WHAT, X, Y, ...) WHAT(X, Y)FP_26(WHAT, __VA_ARGS__)
#define FP_30(WHAT, X, Y, ...) WHAT(X, Y)FP_28(WHAT, __VA_ARGS__)
#define FP_32(WHAT, X, Y, ...) WHAT(X, Y)FP_30(WHAT, __VA_ARGS__)
#define FP_34(WHAT, X, Y, ...) WHAT(X, Y)FP_32(WHAT, __VA_ARGS__)
#define FP_36(WHAT, X, Y, ...) WHAT(X, Y)FP_34(WHAT, __VA_ARGS__)
#define FP_38(WHAT, X, Y, ...) WHAT(X, Y)FP_36(WHAT, __VA_ARGS__)
#define FP_40(WHAT, X, Y, ...) WHAT(X, Y)FP_38(WHAT, __VA_ARGS__)
#define FP_42(WHAT, X, Y, ...) WHAT(X, Y)FP_40(WHAT, __VA_ARGS__)
#define FP_44(WHAT, X, Y, ...) WHAT(X, Y)FP_42(WHAT, __VA_ARGS__)
#define FP_46(WHAT, X, Y, ...) WHAT(X, Y)FP_44(WHAT, __VA_ARGS__)
#define FP_48(WHAT, X, Y, ...) WHAT(X, Y)FP_46(WHAT, __VA_ARGS__)
#define FP_50(WHAT, X, Y, ...) WHAT(X, Y)FP_48(WHAT, __VA_ARGS__)
#define FP_52(WHAT, X, Y, ...) WHAT(X, Y)FP_50(WHAT, __VA_ARGS__)
#define FP_54(WHAT, X, Y, ...) WHAT(X, Y)FP_52(WHAT, __VA_ARGS__)
#define FP_56(WHAT, X, Y, ...) WHAT(X, Y)FP_54(WHAT, __VA_ARGS__)
#define FP_58(WHAT, X, Y, ...) WHAT(X, Y)FP_56(WHAT, __VA_ARGS__)
#define FP_60(WHAT, X, Y, ...) WHAT(X, Y)FP_58(WHAT, __VA_ARGS__)
#define FP_62(WHAT, X, Y, ...) WHAT(X, Y)FP_60(WHAT, __VA_ARGS__)
#define FP_64(WHAT, X, Y, ...) WHAT(X, Y)FP_62(WHAT, __VA_ARGS__)
#define FP_66(WHAT, X, Y, ...) WHAT(X, Y)FP_64(WHAT, __VA_ARGS__)
#define FP_68(WHAT, X, Y, ...) WHAT(X, Y)FP_66(WHAT, __VA_ARGS__)
#define FP_70(WHAT, X, Y, ...) WHAT(X, Y)FP_68(WHAT, __VA_ARGS__)
#define FP_72(WHAT, X, Y, ...) WHAT(X, Y)FP_70(WHAT, __VA_ARGS__)
#define FP_74(WHAT, X, Y, ...) WHAT(X, Y)FP_72(WHAT, __VA_ARGS__)
#define FP_76(WHAT, X, Y, ...) WHAT(X, Y)FP_74(WHAT, __VA_ARGS__)
#define FP_78(WHAT, X, Y, ...) WHAT(X, Y)FP_76(WHAT, __VA_ARGS__)
#define FP_80(WHAT, X, Y, ...) WHAT(X, Y)FP_78(WHAT, __VA_ARGS__)
#define FP_82(WHAT, X, Y, ...) WHAT(X, Y)FP_80(WHAT, __VA_ARGS__)
#define FP_84(WHAT, X, Y, ...) WHAT(X, Y)FP_82(WHAT, __VA_ARGS__)
#define FP_86(WHAT, X, Y, ...) WHAT(X, Y)FP_84(WHAT, __VA_ARGS__)
#define FP_88(WHAT, X, Y, ...) WHAT(X, Y)FP_86(WHAT, __VA_ARGS__)
//... repeat as needed

#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,NAME,...) NAME 
#define FOR_EACH(action,...) \
  IFNEMPTY(__VA_ARGS__)(GET_MACRO(__VA_ARGS__,FE_20,FE_19,FE_18,FE_17,FE_16,FE_15,FE_14,FE_13,FE_12,FE_11,FE_10,FE_9,FE_8,FE_7,FE_6,FE_5,FE_4,FE_3,FE_2,FE_1,)(action,__VA_ARGS__))

#define ENUMERATE(action,...) \
  IFNEMPTY(__VA_ARGS__)(GET_MACRO(__VA_ARGS__,EN_20,EN_19,EN_18,EN_17,EN_16,EN_15,EN_14,EN_13,EN_12,EN_11,EN_10,EN_9,EN_8,EN_7,EN_6,EN_5,EN_4,EN_3,EN_2,EN_1,)(action,__VA_ARGS__))

#define FOR_PAIRS(action,...) \
  IFNEMPTY(__VA_ARGS__)(GET_MACRO(__VA_ARGS__,FP_20,FP_19,FP_18,FP_17,FP_16,FP_15,FP_14,FP_13,FP_12,FP_11,FP_10,FP_9,FP_8,FP_7,FP_6,FP_5,FP_4,FP_3,FP_2,FP_1,)(action,__VA_ARGS__))

// ENUMERATE TARGETS, use for function argument generation from type list
#define AS_ARG_NAMES(T, N) , N

#define AS_ARGS(X, Y) , X Y

#define AS_TYPES(T, N) , T

#endif /* __FU_MACROS_H */
