#include "zenith/log/formatters.hpp"

ZTH_DEFINE_FORMATTER(glm::mat3x3, mat)
{
    return ZTH_FORMAT_OUT("Matrix 3x3 {{\n"
                          "\t[ {:>6.3f} {:>6.3f} {:>6.3f} ]\n"
                          "\t[ {:>6.3f} {:>6.3f} {:>6.3f} ]\n"
                          "\t[ {:>6.3f} {:>6.3f} {:>6.3f} ]\n"
                          "}}",
                          // clang-format off
                          mat[0][0], mat[1][0], mat[2][0],
                          mat[0][1], mat[1][1], mat[2][1],
                          mat[0][2], mat[1][2], mat[2][2]
                          // clang-format on
    );
}

ZTH_DEFINE_FORMATTER(glm::mat4x4, mat)
{
    return ZTH_FORMAT_OUT("Matrix 4x4 {{\n"
                          "\t[ {:>6.3f} {:>6.3f} {:>6.3f} {:>6.3f} ]\n"
                          "\t[ {:>6.3f} {:>6.3f} {:>6.3f} {:>6.3f} ]\n"
                          "\t[ {:>6.3f} {:>6.3f} {:>6.3f} {:>6.3f} ]\n"
                          "\t[ {:>6.3f} {:>6.3f} {:>6.3f} {:>6.3f} ]\n"
                          "}}",
                          // clang-format off
                          mat[0][0], mat[1][0], mat[2][0], mat[3][0],
                          mat[0][1], mat[1][1], mat[2][1], mat[3][1],
                          mat[0][2], mat[1][2], mat[2][2], mat[3][2],
                          mat[0][3], mat[1][3], mat[2][3], mat[3][3]
                          // clang-format on
    );
}
