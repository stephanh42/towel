import towel
import sys
import random
import copy

def make_random_walk(N):
    steps = ((0, 1), (0, -1), (1, 0), (-1, 0))
    x, y = 0, 0
    path = []
    for i in range(N):
        path.append((x, y))
        dx, dy = random.choice(steps)
        x += dx; y += dy
    return path

def print_map(N, transparent, fov):
    r = range(-N, N+1)
    for iy in r:
        for ix in r:
            ch = ' '
            p = (ix, iy)
            if p == (0, 0):
                ch = '@'
            elif fov.is_visible(p):
                if p in transparent:
                    ch = '.'
                else:
                    ch = '#'
            sys.stdout.write(ch)
        sys.stdout.write("\n")


def test(N):
    walk = make_random_walk(100*N)
    the_map = set(walk)
    f = the_map.__contains__
    fov = towel.FieldOfVision(f)
    fov.place_viewer((0, 0), N)
    print_map(N, the_map, fov)

if __name__ == '__main__':
    test(15)
