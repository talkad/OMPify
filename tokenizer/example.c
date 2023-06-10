#include <stdio.h>


type_821 func_893() {
    type_821 arr_942[num_460 + num_773];
    type_821 var_587, var_297;
    type_821 var_166, var_54;
    type_821 var_983 = num_911;


    for (var_587 = num_911; var_587 < num_460; var_587++) {
        arr_942[var_587] = num_662;
    }


    for (var_297 = num_460; var_297 > num_911; var_297 -= num_975) {
        var_54 = num_911;


        var_587 = var_297;
        for (;;) {
            var_54 += arr_942[var_587] * num_714;
            var_166 = num_722 * var_587 - num_773;


            arr_942[var_587] = var_54 % var_166;
            var_54 /= var_166;
            var_587--;
            if (var_587 == num_911) break;
            var_54 *= var_587;
        }
        func_736(str_304, var_983 + var_54 / num_714);
        var_983 = var_54 % num_714;
    }


    return num_911;
}
