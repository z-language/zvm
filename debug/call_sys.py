const_pool = [
    [
        0x02, # TYPE
        0x02, # SIZE
        # VALUE
        ord('h'),
        ord('i')
    ],
]

prog = [
    0x02, # LOAD_CONST
    0x00,

    0xFE
]
