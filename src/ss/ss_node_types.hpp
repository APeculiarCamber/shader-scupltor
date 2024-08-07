#ifndef SHADER_SCUPLTOR_SS_NODE_TYPES_HPP
#define SHADER_SCUPLTOR_SS_NODE_TYPES_HPP

/**
 * Types of SHADER SCULPTOR m_nodes
 */
enum NODE_TYPE {
    NODE_DEFAULT,
    NODE_BUILTIN,
    NODE_CONSTANT,
    NODE_PARAM,
    NODE_VARYING,
    NODE_VECTOR_OP,
    NODE_CUSTOM,
    NODE_TERMINAL,
    NODE_BOILER_VAR
};

/**
 * Gen are used to specify a generic size, The other GLSL_Vecn/Scalar is what is used in propogation and building...
 */
enum GRAPH_PARAM_TYPE : unsigned int {
    SS_Float,
    SS_Double,
    SS_Int,
    SS_Texture2D,
    SS_TextureCube
};

/**
 * Gentypes is GLSL are used to specify generic size.
 * We allow this useful generalization but need to restrict once the (size)type is specified.
 */
enum GRAPH_PARAM_GENTYPE : unsigned int {
    SS_Scalar,
    SS_Vec2,
    SS_Vec3,
    SS_Vec4,
    SS_Mat2,
    SS_Mat3,
    SS_Mat4,
    SS_MAT = SS_Mat2 | SS_Mat3 | SS_Mat4,
    SS_VEC = SS_Vec2 | SS_Vec3 | SS_Vec4
};

/**
 * Type-mask for both type and gentype of a variable or output (or input)
 * Also specifies if array and if OUT-var. Not used in current build.
 */
enum GLSL_TYPE_ENUM {
    GLSL_Bool   = 1,
    GLSL_Int   = 2,
    GLSL_UInt   = 4,
    GLSL_Float   = 8,
    GLSL_Double   = 16,
    GLSL_AllTypes = 31,
    // Container
    GLSL_Scalar  = 32,
    GLSL_Vec2    = 64,
    GLSL_Vec3    = 128,
    GLSL_Vec4    = 256,
    GLSL_VecN    = GLSL_Vec2 | GLSL_Vec3 | GLSL_Vec4,
    GLSL_LenMask = GLSL_Scalar | GLSL_Vec2 | GLSL_Vec3 | GLSL_Vec4,
    GLSL_LenToGenPush = 4,
    GLSL_GenScalar = 512,
    GLSL_GenVec2   = 1024,
    GLSL_GenVec3   = 2048,
    GLSL_GenVec4   = 4096,
    GLSL_GenType = GLSL_GenScalar | GLSL_GenVec2 | GLSL_GenVec3 | GLSL_GenVec4,
    GLSL_GenVec = GLSL_GenVec2 | GLSL_GenVec3 | GLSL_GenVec4,

    // OTHER
    GLSL_Mat   = 8192,
    GLSL_TextureSampler2D = 8192,
    GLSL_TextureSampler3D = 16384,
    GLSL_TextureSamplerCube = 32768,

    GLSL_Out = 536870912,
    GLSL_Array = 2147483648, // size could be embed into unsigned int, but I think it's better to have seperate
    GLSL_TypeMask = GLSL_Bool | GLSL_Int | GLSL_UInt | GLSL_Float | GLSL_Double | GLSL_TextureSampler2D | GLSL_TextureSampler3D | GLSL_TextureSamplerCube
};




typedef unsigned int GLSL_TYPE_ENUM_BITS;
/**
 * Specifies a variable type, can be partially or fully generic (as for GENTYPES).
 * User classes of this must manage restricting types as the user makes specific decisions.
 */
struct GLSL_TYPE {
    GLSL_TYPE() = default;
    GLSL_TYPE(unsigned int flags, unsigned int size) : type_flags(flags), arr_size(size) {}
    GLSL_TYPE_ENUM_BITS type_flags{};
    unsigned int arr_size{};

    GLSL_TYPE Intersect(const GLSL_TYPE& other) {
        type_flags &= other.type_flags;
        arr_size = 1;
        return *this;
    }
    GLSL_TYPE IntersectCopy(const GLSL_TYPE& other) const{
        GLSL_TYPE type;
        type.type_flags = type_flags & other.type_flags;
        type.arr_size = 1;
        return type;
    }

    inline bool IsSameValueType(const GLSL_TYPE& other) const {
        return other.type_flags & type_flags & GLSL_TypeMask;
    }

    inline bool IsMatrix() const {
        return type_flags & GLSL_Mat;
    }
};



#endif //SHADER_SCUPLTOR_SS_NODE_TYPES_HPP
