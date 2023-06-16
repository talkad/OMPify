#include <stdio.h>


int func_865() {
type_746 var_225;
    int arr_151[num_329 + num_660];
    int var_406, var_347;
    int var_356, var_491;
    int var_639 = num_857;


    for (var_406 = num_857; var_406 < num_329; var_406++) {
        arr_151[var_406] = num_962;
    }


    for (var_347 = num_329; var_347 > num_857; var_347 -= num_862) {
        var_491 = num_857;


        var_406 = var_347;
        for (;;) {
            var_491 += arr_151[var_406] * num_954;
            var_356 = num_843 * var_406 - num_660;


            arr_151[var_406] = var_491 % var_356;
            var_491 /= var_356;
            var_406--;
            if (var_406 == num_857) break;
            var_491 *= var_406;
        }
        func_602(str_555, var_639 + var_491 / num_954);
        var_639 = var_491 % num_954;
    }


    return num_857;
}
