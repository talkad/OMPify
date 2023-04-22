# Copyright (c) Microsoft Corporation. 
# Licensed under the MIT license.

from tree_sitter import Language, Parser

Language.build_library(
  # Store the library in the `build` directory
  'my-languages.so',

  # Include one or more languages
  [
    'vendor/tree-sitter-go',
    'vendor/tree-sitter-javascript',
    'vendor/tree-sitter-python',
    'vendor/tree-sitter-php',
    'vendor/tree-sitter-java',
    'vendor/tree-sitter-ruby',
    'vendor/tree-sitter-c-sharp',
    'vendor/tree-sitter-c',
    'vendor/tree-sitter-cpp'
  ]
)

