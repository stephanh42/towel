import pyglet
from pyglet.window import key
from pyglet import gl
import towel

key2direction = {
  key.H : (-1,0),
  key.L : (1,0),
  key.K : (0,1),
  key.J : (0,-1),
  key.Y : (-1, 1),
  key.U : (1, 1),
  key.B : (-1, -1),
  key.N : (1, -1)
}

image_map = pyglet.image.load("32x32.png")

NCOLS = image_map.width//32
NROWS = image_map.height//32
LASTROW = (NROWS-1)*NCOLS

image_map = pyglet.image.ImageGrid(image_map, NROWS, NCOLS)

image_wall = image_map[LASTROW+4]
image_player = image_map[LASTROW-18*NCOLS]
image_floor = image_map[LASTROW+3]

class SimpleRogue(pyglet.window.Window):
    player = (0, 0)
    player_view = 10
    initialized = False

    def __init__(self):
        pyglet.window.Window.__init__(self, resizable=True)
        self.passable = set(towel.random_walk(5000))
        self.remembered = {}

    def init_gl(self):
        texbin = pyglet.image.atlas.TextureBin(32, 3*32)
        self.image_wall = texbin.add(image_wall)
        self.image_player = texbin.add(image_player)
        self.image_floor = texbin.add(image_floor)


    def move(self, delta):
        dx, dy = delta
        x, y = self.player
        new_player = (x + dx, y + dy)
        if new_player in self.passable:
            self.player = new_player
            self.invalid = True

    def on_key_press(self, key, modifiers):
        if key in key2direction:
            self.move(key2direction[key])

    def on_expose(self):
        self.invalid = True

    def on_draw(self):
        from pyglet.sprite import Sprite

        if not self.initialized:
            self.initialized = True
            self.init_gl()

        width, height = self.get_size()
        x_max = width // 32 + 1
        y_max = height // 32 + 1

        gl.glBlendFunc(gl.GL_SRC_ALPHA, gl.GL_ONE_MINUS_SRC_ALPHA)
        gl.glEnable(gl.GL_BLEND)

        fov = towel.FieldOfVision(self.passable.__contains__)
        fov.place_viewer(self.player, self.player_view)
        x, y = self.player

        x_offset = x - x_max // 2
        y_offset = y - y_max // 2

        x_range = range(0, x_max + 1)
        y_range = range(0, y_max + 1)

        self.clear()
        batch = pyglet.graphics.Batch()
        sprites = []

        player_screen_pos = None

        for iy in y_range:
            for ix in x_range:
                p = (ix + x_offset, iy + y_offset)
                image = None
                color = (255, 255, 255)
                if fov.is_visible(p):
                    if p in self.passable:
                       image = self.image_floor 
                    else:
                       image = self.image_wall 
                    self.remembered[p] = image
                else:
                    image = self.remembered.get(p, None)
                    color = (128, 128, 128)
                if image:
                    sprite = Sprite(image, ix*32, iy*32, batch=batch)
                    sprite.color = color
                    sprites.append(sprite)
                if p == self.player:
                    player_screen_pos = (ix*32, iy*32)

        batch.draw()
        if player_screen_pos:
            self.image_player.blit(*player_screen_pos)
        self.invalid = False

rogue = SimpleRogue()

pyglet.app.run()
