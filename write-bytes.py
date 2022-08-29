def make_size(x: int) -> list[int]:
    return [((x >> 8) & 0xFF), x & 0xFF]

def main():
    version = 1

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

    data = [version]

    data.extend(make_size(len(const_pool)))
    for const in const_pool:
        data.extend(const)

    data.extend(make_size(len(prog)))
    data.extend(prog)

    with open("out.žvm", "wb") as f:
        f.write(bytearray(data))


if __name__ == "__main__":
    main()
