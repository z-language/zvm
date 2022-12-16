const_pool = [
    [
        0x01, # TYPE
        0x01, # SIZE
        # VALUE
        11
    ],
    [
        0x01, # TYPE
        0x01, # SIZE
        # VALUE
        0x02
    ]
]

prog = [
    0x01, # PUSH
    0x00,

    0xFE, # DEBUG
    0x13, # DUP
    0x01, # PUSH
    0x0A,

    0x31, # JMPE
    0x00,

    
    0x29, # INCR
    0x30, # JMP
    0x01,

    0xFF  # HLT
]


