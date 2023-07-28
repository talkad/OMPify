from tree_sitter import Language, Parser



def get_parser(lang):
    # LANGUAGE = Language('/home/talkad/Downloads/thesis/data_gathering_script/database_creator/parsers/HPCorpus_parser/my-languages.so', lang.lower())
    LANGUAGE = Language('/home/1010/talkad/OMPify/database_creator/parsers/HPCorpus_parser/my-languages.so', lang.lower())
    parser = Parser()
    parser.set_language(LANGUAGE)

    return parser


parsers = {
    'c': get_parser('c'),
    'cpp': get_parser('cpp'),
    'fortran': get_parser('fortran')
}


def parse(code, lang='c'):
    '''
    Convert @code into an AST according to its programming @lang
    '''
    parser = parsers[lang]

    tree = parser.parse(bytes(code, 'utf8'))
    return tree

