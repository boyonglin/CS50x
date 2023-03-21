def main():

    height = get_height()
    print_grid(height)


def get_height():

    while True:
        try:
            n = int(input("Height: "))
            if 1 <= n <= 8:
                return n
        except ValueError:
            get_height()


def print_grid(height):

    for i in range(1, height + 1):
        num_grids = i
        num_spaces = height - i

        print(" " * num_spaces, end="")
        print("#" * num_grids, end="")
        print("  ", end="")
        print("#" * num_grids, end="")
        print("")


if __name__ == "__main__":
    main()
