# Writes a simple žvm program that loads two values from const pool and adds them.

def make_size(x: int) -> list[int]:
    return [((x >> 8) & 0xFF), x & 0xFF]

def main():
    version = 1

    const_pool = [
        [
            0x01,  # type
            0x02,  # num of bytes
            # value
            0x01,
            0x02
        ],
        [
            0x01, # type
            0x01, # num of bytes
            # value
            0x03
        ]
    ]

    prog = [
        0x02, # LOAD_CONST
        0x00,

        0xFE, # DEBUG

        0x02, # LOAD_CONST
        0x01,

        0xFE, # DEBUG

        0x20, # ADD

        0xFE, # DEBUG
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
