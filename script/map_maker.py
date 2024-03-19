import pygame

def print_map(mapToDraw):
    for y in range(len(mapToDraw)):
        line = ''
        for x in range(len(mapToDraw[y])):
            line += str(mapToDraw[y][x][1])
        print(line)

SIZE_X = int(input('taille window x: '))
SIZE_Y = int(input('taille window y: '))
TILE_X = int(input('taille tile x: ')) 
TILE_Y = int(input('taille tile y: '))

map = [[[pygame.Rect(x*TILE_X+1, y*TILE_Y+1, TILE_X-1, TILE_Y-1),0] for x in range(SIZE_X)] for y in range(SIZE_Y)]

SCREEN_X = SIZE_X*TILE_X
SCREEN_Y = SIZE_Y*TILE_Y

print('map de '+str(SCREEN_X)+'x'+str(SCREEN_Y)+' pixel')

WINDOW  = pygame.display.set_mode((SCREEN_X, SCREEN_Y))

COLOR_0 = (255,255,255)
COLOR_1 = (255,0,0)

running = True

while running:

    for event in pygame.event.get():

        if event.type == pygame.QUIT:
            running = False
            
            print_map(map)

        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1: # 1 == left button
                coord = pygame.mouse.get_pos()
                map[coord[1]//TILE_Y][coord[0]//TILE_X][1] = 1
            if event.button == 3:
                coord = pygame.mouse.get_pos()
                map[coord[1]//TILE_Y][coord[0]//TILE_X][1] = 0

    for y in map:
        for x in y:
            # Drawing Rectangle
            if x[1] == 0:
                pygame.draw.rect(WINDOW,COLOR_0,x[0])
            else:
                pygame.draw.rect(WINDOW,COLOR_1,x[0])

    pygame.display.update()
