from pygccxml import utils
from pygccxml import declarations
from pygccxml import parser
from castxml import castxml

cast = castxml()
print(dir(cast))
print(cast.parse('example.cpp'))
print("?")
# print(dir(castxml.castxml))

# # Find the location of the xml generator (castxml or gccxml)
# generator_path, generator_name = utils.find_xml_generator()

# # Configure the xml generator
# xml_generator_config = parser.xml_generator_configuration_t(
#     xml_generator_path=generator_path,
#     xml_generator=generator_name)

# # Write a string containing some c++ code
# code = """
#     int main() {
#         int i;
#         int res = 0;

#         for(i = 0; i < 5; i++){
#             res += 1;
#         }
#     };
# """

# # Parse the code
# decls = parser.parse_string(code, xml_generator_config)

# # for decl in decls:
# #     for a in decl:
# #         print(a)

# # print(decls)
# declarations.print_declarations(decls)

# print(decls[0][0])