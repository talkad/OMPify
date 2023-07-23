# training mode
TRAINING_MODE_PRE_TRAIN = 'pre_train'
TRAINING_MODE_FINE_TUNE = 'fine_tune'

# pre-training task names
TASK_CODE_AST_PREDICTION = 'cap'
TASK_MASS = 'mass'

PRE_TRAIN_TASKS = [
    TASK_MASS,
    TASK_CODE_AST_PREDICTION
]

# downstream task names
TASK_PRAGMA = 'pragma_generation'
TASK_TRANSLATION = 'translation'

ALL_DOWNSTREAM_TASKS = [
    TASK_PRAGMA,
    TASK_TRANSLATION
]

# programming language
LANG_C = 'c'
LANG_CPP = 'cpp'
LANG_FORTRAN = 'fortran'

ALL_LANGS = [
    LANG_C,
    LANG_CPP,
    LANG_FORTRAN
]

# BART model mode
MODEL_MODE_CLS = 'bart_cls'
MODEL_MODE_GEN = 'bart_gen'
MODEL_MODE_SEARCH = 'bart_search'
