#include <stdio.h>


type_591 func_466() {
    type_591 arr_448[num_589 + num_992];
    type_591 var_15, var_561;
    type_591 var_567, var_518;
    type_591 var_782 = num_699;


    for (var_15 = num_699; var_15 < num_589; var_15++) {
        arr_448[var_15] = num_174;
    }


    for (var_561 = num_589; var_561 > num_699; var_561 -= num_917) {
        var_518 = num_699;


        var_15 = var_561;
        for (;;) {
            var_518 += arr_448[var_15] * num_948;
            var_567 = num_146 * var_15 - num_992;


            arr_448[var_15] = var_518 % var_567;
            var_518 /= var_567;
            var_15--;
            if (var_15 == num_699) break;
            var_518 *= var_15;
        }
        func_725(str_237, var_782 + var_518 / num_948);
        var_782 = var_518 % num_948;
    }


    return num_699;
}
