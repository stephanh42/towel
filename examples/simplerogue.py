import curses
import towel

key2direction = {
  ord('h') : (-1,0),
  ord('l') : (1,0),
  ord('k') : (0,-1),
  ord('j') : (0,1),
  ord('y') : (-1, -1),
  ord('u') : (1, -1),
  ord('b') : (-1, 1),
  ord('n') : (1, 1)
}

class SimpleRogue:
    player = (0, 0)
    player_view = 10

    def __init__(self, screen):
        self.screen = screen
        self.passable = set(towel.random_walk(5000))
        self.remembered = {}

    def move(self, delta):
        dx, dy = delta
        x, y = self.player
        new_player = (x + dx, y + dy)
        if new_player in self.passable:
            self.player = new_player

    def draw(self):
        fov = towel.FieldOfVision(self.passable.__contains__)
        fov.place_viewer(self.player, self.player_view)
        x, y = self.player
        x_range = range(x - self.player_view, x + self.player_view + 1)
        y_range = range(y - self.player_view, y + self.player_view + 1)
        line = 1
        screen = self.screen
        for iy in y_range:
            screen.move(line, 1)
            line = line + 1
            for ix in x_range:
                p = (ix, iy)
                if fov.is_visible(p):
                    screen.attrset(curses.A_BOLD)
                    if p == self.player:
                        ch = '@'
                    elif p in self.passable:
                        ch = '.'
                    else:
                        ch = '#'
                    self.remembered[p] = ch
                else:
                    screen.attrset(curses.A_NORMAL)
                    ch = self.remembered.get(p, ' ')
                screen.addch(ch)


def main(screen):
    rogue = SimpleRogue(screen)
    rogue.draw()
    while True:
        ch = screen.getch()
        if ch in key2direction:
            rogue.move(key2direction[ch])
            rogue.draw()
        elif ch == ord('q'):
            return


curses.wrapper(main)

