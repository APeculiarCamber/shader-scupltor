
#include <algorithm>
#include "ss_parser.hpp"

char SS_Parser::GetVecLenChar(GLSL_TYPE_ENUM_BITS type) {
    bool b_vec2 = type & GLSL_Vec2;
    bool b_vec3 = type & GLSL_Vec3;
    bool b_vec4 = type & GLSL_Vec4;

    if (b_vec2 && b_vec3 && b_vec4)
        return 'n';
    else if (b_vec2)
        return '2';
    else if (b_vec3)
        return '3';
    else if (b_vec4)
        return '4';
    return 'n';
}

char GetBaseTypeToChar(GLSL_TYPE_ENUM_BITS type) {
    if (type & GLSL_Bool)
        return 'b';
    else if (type & GLSL_Float)
        return ' ';
    else if (type & GLSL_Double)
        return 'd';
    else if (type & GLSL_Int)
        return 'i';
    else if (type & GLSL_UInt)
        return 'u';
    return '?';
}

std::string SS_Parser::GLSLTypeToString(GLSL_TYPE type) {
    GLSL_TYPE_ENUM_BITS type_flags = type.type_flags;
    if ((type_flags & GLSL_LenMask) == GLSL_LenMask && !(type_flags & GLSL_Mat)) {
        // GenType
        char vec_type = GetBaseTypeToChar(type_flags);
        if (vec_type == ' ')
            return "GenType";
        else
            return std::string("Gen") + (char)toupper(vec_type) + "Type";
    } if ((type_flags & GLSL_LenMask) != GLSL_Scalar && !(type_flags & GLSL_Mat)) {
        // VECTOR
        char vec_len = GetVecLenChar(type_flags);
        char vec_type = GetBaseTypeToChar(type_flags);
        std::string vec_str = "vec";
        return vec_type + vec_str + vec_len;
    } else if (type_flags & GLSL_Mat && (type_flags & (GLSL_Bool | GLSL_Float | GLSL_Double | GLSL_Int))) {
        // MATRIX
        return std::string("mat") + GetVecLenChar(type_flags);
    }
    if (type_flags & GLSL_TextureSampler2D) {
        return "sampler2D";
    }
    if (type_flags & GLSL_TextureSampler3D) {
        return "sampler3D";
    } 
    if (type_flags & GLSL_TextureSamplerCube) {
        return "samplerCube";
    }
    if (type_flags & GLSL_Float) {
        return "float";
    }
    if (type_flags & GLSL_UInt) {
        return "uint";
    }
    if (type_flags & GLSL_Int) {
        return "int";
    }
    if (type_flags & GLSL_Bool) {
        return "bool";
    }
    if (type_flags & GLSL_Double) {
        return "double";
    }
    return "";
}


/*
Scalars:
    float, double, uint, int, bool
Vecs:
     vecn, dvecn, uvecn, ivecn, bvecn :: maybe not real...
GenType:
    GenDType, GenType, GenIType, GenUType, GenBType
Mat:
    Matn :: is all you get
Samplers
    They actually use gsampler1D where g is anything but I'm only gonna let them have floats so g = ''
    sampler2D, sampler3D, samplerCube
*/
GLSL_TYPE_ENUM_BITS typeCharToFlag(char t) {
    t = (char)std::tolower(t);
    switch (t) {
        case 'd': return GLSL_Double;
        case 'f': return GLSL_Float;
        case 'b': return GLSL_Bool;
        case 'u': return GLSL_UInt;
        case 'i': return GLSL_Int;
        default: return GLSL_Float;
    }
}
GLSL_TYPE_ENUM_BITS vecLenCharToFlag(char t) {
    t = (char)std::tolower(t);
    switch (t) {
        case 'n': return GLSL_GenVec | GLSL_VecN;
        case '2': return GLSL_Vec2;
        case '3': return GLSL_Vec3;
        case '4': return GLSL_Vec4;
        case '1': return GLSL_Scalar;
        default: return GLSL_GenVec;
    }
}

GLSL_TYPE SS_Parser::StringToGLSLType(std::string type_str) {
    size_t v_i;
    GLSL_TYPE type;
    type.type_flags = 0;
    // VECTOR
    if ((v_i = type_str.find("vec")) != std::string::npos) {
        if (v_i == 0) 
            type.type_flags |= GLSL_Float;
        else 
            type.type_flags |= typeCharToFlag(type_str[0]);
    
        if (type_str[v_i + 3] == 'n')
            type.type_flags |= GLSL_GenVec | GLSL_VecN;
        else 
            type.type_flags |= vecLenCharToFlag(type_str[v_i + 3]);
    } 
    // GEN TYPE
    else if ((v_i = type_str.find("Type")) != std::string::npos) {
        type.type_flags |= GLSL_GenType | GLSL_LenMask;
        if (type_str[v_i] == 'n') // the n of Gen in GenType (float type GenType)
            type.type_flags |= GLSL_Float;
        else 
            type.type_flags |= typeCharToFlag(type_str[v_i - 1]);
    } 
    // MAT
    else if ((v_i = type_str.find("mat")) != std::string::npos) {
        type.type_flags |= GLSL_Mat;
        if (v_i == 0)
            type.type_flags |= GLSL_Float;
        else 
            type.type_flags |= GLSL_Double;

        type.type_flags |= vecLenCharToFlag(type_str[v_i + 3]);
    } 
    // SCALAR
    else {
        type.type_flags |= GLSL_Scalar;
        if (type_str == "float") {
            type.type_flags |= GLSL_Float;
        } else if (type_str == "double") {
            type.type_flags |= GLSL_Double;
        } else if (type_str == "uint") {
            type.type_flags |= GLSL_UInt;
        } else if (type_str == "bool") {
            type.type_flags |= GLSL_Bool;
        } else if (type_str == "int") {
            type.type_flags |= GLSL_Int;
        }  // Texture Samplers
        else if (type_str == "sampler2D") {
            type.type_flags |= GLSL_TextureSampler2D;
        } else if (type_str == "sampler3D") {
            type.type_flags |= GLSL_TextureSampler3D;
        } else if (type_str == "samplerCube") {
            type.type_flags |= GLSL_TextureSamplerCube;
        } else {
            assert(false);
        }
    }
    return type;
}
std::string SS_Parser::GetUniqueVarName(int node_id, int var_id, GLSL_TYPE type) {
    return std::string("INTERNAL_VAR_") + std::to_string(node_id) + std::string("_") + std::to_string(var_id);
}

ImU32 SS_Parser::GLSLTypeToColor(GLSL_TYPE type) {
    if ((GLSL_AllTypes & type.type_flags) == GLSL_AllTypes)
        return 0xffffffff;
    if (GLSL_Bool & type.type_flags) {
        return 0xff006400;
    } else if (GLSL_Double & type.type_flags) {
        return 0xff00ffff;
    } else if (GLSL_Float & type.type_flags) {
        return 0xffffff00;
    } else if (GLSL_Int & type.type_flags) {
        return 0xffff5555;
    }
    return 0xffffffff;
}

GLSL_TYPE SS_Parser::ConstantTypeToGLSLType(GRAPH_PARAM_GENTYPE gentype, GRAPH_PARAM_TYPE type, unsigned int arr_size) {
    GLSL_TYPE t;
    t.type_flags = 0;
    t.arr_size = arr_size;
    switch (gentype) {
        case SS_Scalar: t.type_flags |= GLSL_Scalar; break;
        case SS_Vec2: t.type_flags |= GLSL_Vec2; break;
        case SS_Vec3: t.type_flags |= GLSL_Vec3; break;
        case SS_Vec4: t.type_flags |= GLSL_Vec4; break;
        case SS_Mat2: t.type_flags |= (GLSL_Vec2 | GLSL_Mat); break;
        case SS_Mat3: t.type_flags |= (GLSL_Vec3 | GLSL_Mat); break;
        case SS_Mat4: t.type_flags |= (GLSL_Vec4 | GLSL_Mat); break;
        case SS_MAT:
            // NOTHING
            break;
    }
    switch (type) {
        case SS_Double: t.type_flags |= GLSL_Double; break;
        case SS_Texture2D: t.type_flags |= GLSL_TextureSampler2D | GLSL_Scalar; break;
        case SS_TextureCube: t.type_flags |= GLSL_TextureSamplerCube | GLSL_Scalar; break;
        case SS_Int: t.type_flags |= GLSL_Int; break;
        case SS_Float: t.type_flags |= GLSL_Float; break;
    }
    return t;
}

std::string make_default_mat_str_val(char l) {
    std::string mat_str = "mat";
    mat_str += l;
    mat_str += '(';
    assert(l >= '2' && l <= '4');
    int size = l - '0';
    for (int e = 0; e < size*size; ++e) {
        int y = e / size;
        int x = e % size;
        mat_str += y == x ? "1" : "0";
        mat_str += e + 1 == size*size ? ", " : ");";
    }
    return mat_str;
}

std::string make_default_vec_str_val(char vec_len, char vec_type) {
    return vec_type + std::string("vec") + vec_len + "()";
}

std::string SS_Parser::GLSLTypeToDefaultValue(GLSL_TYPE type) {
    GLSL_TYPE_ENUM_BITS type_flags = type.type_flags;
    if ((type_flags & GLSL_LenMask) == GLSL_LenMask && !(type_flags & GLSL_Mat)) {
        return "FAILURE";
    } if ((type_flags & GLSL_LenMask) != GLSL_Scalar && !(type_flags & GLSL_Mat)) {
        // VECTOR
        char vec_len = GetVecLenChar(type_flags);
        char vec_type = GetBaseTypeToChar(type_flags);
        return make_default_vec_str_val(vec_len, vec_type);
    } else if (type_flags & GLSL_Mat) {
        // MATRIX
        return make_default_mat_str_val(GetVecLenChar(type_flags));
    }
    if (type_flags & GLSL_TextureSampler2D) {
        return "0";
    }
    if (type_flags & GLSL_TextureSampler3D) {
        return "0";
    } 
    if (type_flags & GLSL_TextureSamplerCube) {
        return "0";
    }
    if (type_flags & GLSL_Float) {
        return "1.0f";
    }
    if (type_flags & GLSL_UInt) {
        return "1";
    }
    if (type_flags & GLSL_Int) {
        return "1";
    }
    if (type_flags & GLSL_Bool) {
        return "True";
    }
    if (type_flags & GLSL_Double) {
        return "1.0";
    }
    return "";
}

std::string SS_Parser::ConvertOutputToColorStr(std::string pin_name, GLSL_TYPE type) {
    if (GLSL_Float & type.type_flags) {
        if (type.type_flags & GLSL_Scalar)
            return "vec4(" + pin_name + ", "  + pin_name + ", "  + pin_name + ", 1)";
        if (type.type_flags & GLSL_Vec2)
            return "vec4(" + pin_name + ".x, "  + pin_name + ".y, 0, 1)";
        if (type.type_flags & GLSL_Vec3)
            return "vec4(" + pin_name + ".x, "  + pin_name + ".y, "  + pin_name + ".z, 1)";
        if (type.type_flags & GLSL_Vec4)
            return "vec4(" + pin_name + ".xyz, 1)";
    } else if (GLSL_Bool & type.type_flags) {
        if (type.type_flags & GLSL_Scalar)
            return pin_name + "? vec4(1,1,1,1) : vec4(0,0,0,1)";
        if (type.type_flags & GLSL_Vec2)
            return "mix(vec4(0,0,0,1), vec4(1,1,1,1),  bvec4(" + pin_name + ", true, true))";
        if (type.type_flags & GLSL_Vec3)
            return "mix(vec4(0,0,0,1), vec4(1,1,1,1),  bvec4(" + pin_name + ", true))";
        if (type.type_flags & GLSL_Vec4)
            return "mix(vec4(0,0,0,1), vec4(1,1,1,1), " + pin_name + ")";
    } else if (GLSL_Double & type.type_flags) {
        if (type.type_flags & GLSL_Scalar)
            return "vec4(float(" + pin_name + "), float("  + pin_name + "), float("  + pin_name + "), 1)";
        if (type.type_flags & GLSL_Vec2)
            return "vec4(float(" + pin_name + ".x), float("  + pin_name + ".y), 0, 1)";
        if (type.type_flags & GLSL_Vec3)
            return "vec4(float(" + pin_name + ".x), float("  + pin_name + ".y), float("  + pin_name + ".z), 1)";
        if (type.type_flags & GLSL_Vec4)
            return "vec4(float(" + pin_name + ".x), float(" + pin_name + ".y), float(" +  pin_name + ".z), " + "1)";
    }
    return "vec4(1, 1, 1, 1)";
}

std::string SS_Parser::StringToLower(const std::string &str) {
    std::string lowerStr(str.size(), ' ');
    std::transform(str.begin(), str.end(), lowerStr.begin(), [](char c) { return tolower(c); });
    return str;
}
