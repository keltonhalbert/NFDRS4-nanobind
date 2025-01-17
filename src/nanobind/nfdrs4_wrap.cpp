// clang-format off
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nfdrs4.h>

namespace nb = nanobind;

// Define a type used for read-only 2D-arrays from Python.
// Array types are double 2 dimensions, CPU only (no CUDA/GPU),
// contiguous C-style arrays, read-only.
using const_2D_arr_t =
    nb::ndarray<double, nb::ndim<2>, nb::device::cpu, nb::c_contig, nb::ro>;

// clang-format on
NB_MODULE(nfdrs4_wrap, m) {
    m.doc() = "NFDRS4Py :: Nanobind Python wrappers for NFDRS4";

    // Here we are defining a wrapper function that does some NFDRS4
    // library calculations. A lamba function (an anonymous C++ function)
    // is used to take in the 2D arrays, and perform computations within
    // a loop, returning some output array.
    m.def(
        // This is the name of the function
        // that will be available in Python
        "do_some_NFDRS_stuff",
        // This is the function lamda. The [] is a capture group
        // (not needed), and the () is the input group. This is where
        // you will add arrays and such... these are just example
        // placeholers, rename and add the appropriate number
        // of arrays for whatever work you want to do in NFDRS4
        [](const_2D_arr_t air_temp_arr, const_2D_arr_t relhum_arr,
           const_2D_arr_t precip_arr, const_2D_arr_t wspd_arr) {
            // First, we need a high performance zero-copy
            // view into our arrays. This allows for the compiler
            // to optimize the array indexing with minimal overhead.
            auto air_temp = air_temp_arr.view();
            auto relhum = relhum_arr.view();
            auto precip = precip_arr.view();
            auto wspd = wspd_arr.view();

            // Next, we need to enforce that our arrays are all the same size,
            // otherwise we could get unexpected, undefined, or erroneous
            // behavior. We don't want that! We shouldn't have to check
            // whether or not the arrays are 2D, because our const_2D_arr_t
            // should enforce that and error if not.
            for (std::size_t idx = 0; idx < air_temp.ndim(); ++idx) {
                if ((air_temp.shape(idx) != relhum.shape(idx)) ||
                    (air_temp.shape(idx) != precip.shape(idx)) ||
                    (air_temp.shape(idx) != wspd.shape(idx))) {
                    throw nb::buffer_error(
                        "Input arrays must have the same dimensionality and "
                        "shape!");
                }
            }

            // Now that our runtime checks are done, we can enter the
            // computation loop and do some work.
            for (std::size_t j = 0; j < air_temp.shape(0); ++j) {
                for (std::size_t i = 0; i < air_temp.shape(1); ++i) {
                    // Here we can do some work, or in this case, just print
                    // the output to the console...
                    printf("%d %d\t%f\t%f\t%f\t%f\n", j, i, air_temp(j, i),
                           relhum(j, i), precip(j, i), wspd(j, i));
                    // We included the nfdrs4.h header file, so anything
                    // in there can be called. Other header files can be
                    // included as well.
                }
            }

            // Later, we can figure out what kind of return array or type that
            // you want and have it return a numpy array of state objects,
            // or double values, etc etc.
        },
        // The binding library can't detect the actual variable names,
        // just the types (e.g. float, double). This allows for you to name
        // the variables for the python documentation (i.e.
        // help(NFDRS4Py.do_some_NFDRS_stuff)).
        nb::arg("air_temp_arr"), nb::arg("relhum_arr"), nb::arg("precip_arr"),
        nb::arg("wspd_arr"),
        // This lets you set the rest of the Python docstring. This step
        // is not critical or necessary, but can be good if you plan to have
        // other people use the binding code.
        R"pbdoc(
This is the custom Python docstring. Tabs, spaces, and carraige returns matter, 
which is why the indentation is all the way to the left-hand side. 

    )pbdoc");
}
