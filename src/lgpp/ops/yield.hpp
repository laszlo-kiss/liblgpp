#ifndef LGPP_OPS_YIELD_HPP
#define LGPP_OPS_YIELD_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Yield {
    Yield(unsigned n): n(n) {}
    unsigned n;
  };

  template <>
  inline const Op *eval(const Op &op, const Yield &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    auto c = vm.thread().pop_coro();
    auto ret = vm.pop_ret();
    
    if (c) {
      if (c->done) { throw runtime_error("Coro is done"); }
      if (!((int)ret.opts & (int)lgpp::Ret::Opts::CORO)) { throw runtime_error("Tield from non-coro call"); }
      c->pc = op.pc+1;
      push(stack, lgpp::types::Coro, *c);
    } else {
      Coro c(op.pc+1);
      push(stack, lgpp::types::Coro, c);
    }
    
    return &op - op.pc + ret.pc;
  }

}

#endif