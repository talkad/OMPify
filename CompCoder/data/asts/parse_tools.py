from tree_sitter import Language, Parser



def get_parser(lang):
    LANGUAGE = Language('/homes/talkad/OMPify/database_creator/parsers/HPCorpus_parser/my-languages.so', lang.lower())
    parser = Parser()
    parser.set_language(LANGUAGE)

    return parser


parsers = {
    'c': get_parser('c'),
    'cpp': get_parser('cpp'),
    'fortran': get_parser('fortran')
}


def parse(code, lang):
    '''
    Convert @code into an AST according to its programming @lang
    '''
    parser = parsers[lang]

    tree = parser.parse(bytes(code, 'utf8'))
    return tree

