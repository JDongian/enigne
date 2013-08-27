import PIL
import os
import yesno

directory = raw_input("Directory of images: ")
recursive = yesno.query_yes_no("Recursive? ", default="no")

def eval(arrangement):
    x = 0
    y = 0
    for xval,yval in arrangement:
        if xval > x:
            x = xval
        if yval > y:
            y = yval
    return x*y

def arrange(image_data):
    image_data.sort()
    arrangement = image_data[0][0], (0,0)
    print arrangement
    for im in image_data:
        print im

arrange([('apple', (90, 29)), ('pear', (10, 30)), ('bannana', (100, 100))])



