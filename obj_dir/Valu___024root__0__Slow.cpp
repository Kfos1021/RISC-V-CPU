// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Valu.h for the primary calling header

#include "Valu__pch.h"

VL_ATTR_COLD void Valu___024root___eval_static(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval_static\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Valu___024root___eval_initial(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval_initial\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Valu___024root___eval_final(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval_final\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Valu___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Valu___024root___eval_phase__stl(Valu___024root* vlSelf);

VL_ATTR_COLD void Valu___024root___eval_settle(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval_settle\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Valu___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("rtl/core/alu.sv", 2, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 10000 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = Valu___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void Valu___024root___eval_triggers_vec__stl(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval_triggers_vec__stl\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
}

VL_ATTR_COLD bool Valu___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Valu___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Valu___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Valu___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___trigger_anySet__stl\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

VL_ATTR_COLD void Valu___024root___eval_stl(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval_stl\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        vlSelfRef.result = (((4U & (IData)(vlSelfRef.op))
                              ? ((2U & (IData)(vlSelfRef.op))
                                  ? ((1U & (IData)(vlSelfRef.op))
                                      ? (vlSelfRef.a 
                                         >> (0x0000001fU 
                                             & vlSelfRef.b))
                                      : (vlSelfRef.a 
                                         << (0x0000001fU 
                                             & vlSelfRef.b)))
                                  : ((1U & (IData)(vlSelfRef.op))
                                      ? (1U & (- (IData)(
                                                         VL_LTS_III(32, vlSelfRef.a, vlSelfRef.b))))
                                      : (vlSelfRef.a 
                                         ^ vlSelfRef.b)))
                              : ((2U & (IData)(vlSelfRef.op))
                                  ? ((1U & (IData)(vlSelfRef.op))
                                      ? (vlSelfRef.a 
                                         | vlSelfRef.b)
                                      : (vlSelfRef.a 
                                         & vlSelfRef.b))
                                  : ((1U & (IData)(vlSelfRef.op))
                                      ? (vlSelfRef.a 
                                         - vlSelfRef.b)
                                      : (vlSelfRef.a 
                                         + vlSelfRef.b)))) 
                            & (- (IData)((1U & (~ ((IData)(vlSelfRef.op) 
                                                   >> 3U))))));
    }
}

VL_ATTR_COLD bool Valu___024root___eval_phase__stl(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval_phase__stl\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Valu___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Valu___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Valu___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Valu___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Valu___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Valu___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(Valu___024root___trigger_anySet__ico(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Valu___024root___ctor_var_reset(Valu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___ctor_var_reset\n"); );
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->a = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 510903276987443985ull);
    vlSelf->b = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16900879642891266615ull);
    vlSelf->op = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 3630531923276091163ull);
    vlSelf->result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16664408842984530663ull);
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
}
