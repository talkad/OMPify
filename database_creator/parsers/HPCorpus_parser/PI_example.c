
replaced code:

int func_879() {
    int arr_285[num_405 + num_293];
    int var_723, var_651;
    int var_914, var_640;
    int var_841 = num_167;

    for (var_723 = num_167; var_723 < num_405; var_723++) {
        arr_285[var_723] = num_76;
    }

    for (var_651 = num_405; var_651 > num_167; var_651 -= num_613) {
        var_640 = num_167;

        var_723 = var_651;
        for (;;) {
            var_640 += arr_285[var_723] * num_274;
            var_914 = num_194 * var_723 - num_293;

            arr_285[var_723] = var_640 % var_914;
            var_640 /= var_914;
            var_723--;
            if (var_723 == num_167) break;
            var_640 *= var_723;
        }
        func_819(str_523, var_841 + var_640 / num_274);
        var_841 = var_640 % num_274;
    }

    return num_167;
}


ast (XSBT):
['compound_statement__', 'declaration__', 'binary_expression', '__declaration', 'declaration', 'declaration', 'declaration', 'for_statement__', 'assignment_expression', 'binary_expression', 'update_expression', 'compound_statement__', 'expression_statement__', 'assignment_expression__', 'subscript_expression', '__assignment_expression', '__expression_statement', '__compound_statement', '__for_statement', 'for_statement__', 'assignment_expression', 'binary_expression', 'assignment_expression', 'compound_statement__', 'expression_statement__', 'assignment_expression', '__expression_statement', 'expression_statement__', 'assignment_expression', '__expression_statement', 'for_statement__', 'compound_statement__', 'expression_statement__', 'assignment_expression__', 'binary_expression__', 'subscript_expression', '__binary_expression', '__assignment_expression', '__expression_statement', 'expression_statement__', 'assignment_expression__', 'binary_expression__', 'binary_expression', '__binary_expression', '__assignment_expression', '__expression_statement', 'expression_statement__', 'assignment_expression__', 'subscript_expression', 'binary_expression', '__assignment_expression', '__expression_statement', 'expression_statement__', 'assignment_expression', '__expression_statement', 'expression_statement__', 'update_expression', '__expression_statement', 'if_statement__', 'parenthesized_expression__', 'binary_expression', '__parenthesized_expression', 'break_statement', '__if_statement', 'expression_statement__', 'assignment_expression', '__expression_statement', '__compound_statement', '__for_statement', 'expression_statement__', 'call_expression__', 'binary_expression__', 'binary_expression', '__binary_expression', '__call_expression', '__expression_statement', 'expression_statement__', 'assignment_expression__', 'binary_expression', '__assignment_expression', '__expression_statement', '__compound_statement', '__for_statement', 'return_statement', '__compound_statement']


DFG:

[(('num', '405'), 11, 'comesFrom', [], []), (('num', '293'), 13, 'comesFrom', [], []), (('var', '841'), 27, 'comesFrom', [], []), (('num', '167'), 29, 'comesFrom', [], []), (('var', '723'), 33, 'computedFrom', [('num', '167')], [35]), (('num', '167'), 35, 'comesFrom', [('num', '167')], [29]), (('var', '723'), 37, 'comesFrom', [('var', '723')], [33]), (('num', '405'), 39, 'comesFrom', [('num', '405')], [11]), (('var', '723'), 41, 'comesFrom', [('var', '723')], [33]), (('arr', '285'), 45, 'computedFrom', [('num', '76')], [50]), (('var', '723'), 47, 'computedFrom', [('num', '76')], [50]), (('num', '76'), 50, 'comesFrom', [], []), (('var', '651'), 55, 'computedFrom', [('num', '405')], [57]), (('num', '405'), 57, 'comesFrom', [('num', '405')], [11]), (('var', '651'), 59, 'comesFrom', [('var', '651')], [55]), (('num', '167'), 61, 'comesFrom', [('num', '167')], [29]), (('var', '651'), 63, 'computedFrom', [('num', '613')], [65]), (('num', '613'), 65, 'comesFrom', [], []), (('var', '640'), 68, 'computedFrom', [('num', '167')], [70]), (('num', '167'), 70, 'comesFrom', [('num', '167')], [29]), (('var', '723'), 72, 'computedFrom', [('var', '651')], [74]), (('var', '651'), 74, 'comesFrom', [('var', '651')], [63]), (('var', '640'), 82, 'computedFrom', [('var', '723'), ('num', '274'), ('arr', '285')], [84, 86, 89]), (('arr', '285'), 84, 'comesFrom', [('arr', '285')], [45]), (('var', '723'), 86, 'comesFrom', [('var', '723')], [72]), (('num', '274'), 89, 'comesFrom', [], []), (('var', '914'), 91, 'computedFrom', [('num', '293'), ('var', '723'), ('num', '194')], [93, 95, 97]), (('num', '194'), 93, 'comesFrom', [], []), (('var', '723'), 95, 'comesFrom', [('var', '723')], [72]), (('num', '293'), 97, 'comesFrom', [('num', '293')], [13]), (('arr', '285'), 99, 'computedFrom', [('var', '640'), ('var', '914')], [104, 106]), (('var', '723'), 101, 'computedFrom', [('var', '640'), ('var', '914')], [104, 106]), (('var', '640'), 104, 'comesFrom', [('var', '640')], [82]), (('var', '914'), 106, 'comesFrom', [('var', '914')], [91]), (('var', '640'), 108, 'computedFrom', [('var', '914')], [110]), (('var', '914'), 110, 'comesFrom', [('var', '914')], [91]), (('var', '723'), 112, 'comesFrom', [('var', '723')], [101]), (('var', '723'), 117, 'comesFrom', [('var', '723')], [101]), (('num', '167'), 119, 'comesFrom', [('num', '167')], [29]), (('var', '640'), 123, 'computedFrom', [('var', '723')], [125]), (('var', '723'), 125, 'comesFrom', [('var', '723')], [101]), (('var', '841'), 132, 'comesFrom', [('var', '841')], [27]), (('var', '640'), 134, 'comesFrom', [('var', '640')], [123]), (('num', '274'), 136, 'comesFrom', [('num', '274')], [89]), (('var', '841'), 139, 'computedFrom', [('var', '640'), ('num', '274')], [141, 143]), (('var', '640'), 141, 'comesFrom', [('var', '640')], [123]), (('num', '274'), 143, 'comesFrom', [('num', '274')], [89]), (('num', '167'), 147, 'comesFrom', [('num', '167')], [29])]