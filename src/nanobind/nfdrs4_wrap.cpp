#include <nanobind/nanobind.h>

namespace nb = nanobind;

NB_MODULE(nfdrs4_wrap, m) {
    m.doc() = "NFDRS4Py :: Nanobind Python wrappers for NFDRS4";
    m.def(
        "add", [](int a, int b) { return a + b; }, nb::arg("a"), nb::arg("b"));
}
