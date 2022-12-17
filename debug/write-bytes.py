# from count_to_10 import const_pool, prog
from count_to_10 import const_pool, prog

def make_size(x: int) -> list[int]:
    return [((x >> 8) & 0xFF), x & 0xFF]

def main():
    version = 1

    data = [version]
    
    data.extend(make_size(len(prog)))
    data.extend(prog)
    
    data.extend(make_size(len(const_pool)))
    for const in const_pool:
        data.extend(const)

    with open("debug/main", "wb") as f:
        f.write(bytearray(data))


if __name__ == "__main__":
    main()
