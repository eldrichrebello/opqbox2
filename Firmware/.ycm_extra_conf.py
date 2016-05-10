flags = [
    # General flags
    '-Wall'
    ,'-Wextra'
    ,'-Werror'

    ,'-Wno-attributes'
    ,'-std=c++11'
    ,'-x'
    ,'c++'

    ,'-DSTM32F303VC'
    ,'-DSTM32F30X'
    ,'-D__FPU_USED'
    ,'-DUSE_STDPERIPH_DRIVER'

    ,'-Isrc/cmsis/headers'
    ,'-Isrc/cmsis_boot/headers'
    ,'-Isrc/stm_lib/headers'
]
