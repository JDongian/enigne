from PIL import Image
import os
import yesno

from pprint import pprint

def eval(positions):
    x = 0
    y = 0
    #print 'Evaluating:'
    #pprint(positions)
    for x_val, y_val in positions:
        if x_val > x:
            x = x_val
        if y_val > y:
            y = y_val
    return x*y

def update_positions(positions, new_box):
    #print 'args:', positions, new_box
    old = new_box[0]
    p = positions[:]
    p.remove(old)
    p.append((new_box[1][0][0]+old[0], old[1]))
    p.append((old[0], new_box[1][0][1]+old[1]))
    return p

def fit_box(positions, arrangement, box):
    test = update_positions(positions, (positions[0], (box[1], box[0])))
    best_fit = eval(test), positions[0]
    for p in positions[1:]:
        test = update_positions(positions, (p, (box[1], box[0])))
        if eval(test) < best_fit[0]:
            best_fit = test, p
        if len(test) > 5:
            break
    return best_fit[1]

def arrange(boxes):
    boxes.sort(key=lambda v: v[1])
    arrangement = [((0,0), boxes.pop())]
    curr_box = arrangement[0][1]
    positions = [(curr_box[1][0], arrangement[0][0][1]), (arrangement[0][0][0], curr_box[1][1])]
    for b in boxes[::-1]:
        #print 'fitting...', b
        optimal_fit = fit_box(positions, arrangement, b)
        arrangement.append((optimal_fit, b))
        positions.remove(optimal_fit)
        positions.append((arrangement[-1][1][1][0]+optimal_fit[0], optimal_fit[1]))
        positions.append((optimal_fit[0], arrangement[-1][1][1][1]+optimal_fit[1]))
    return arrangement

def place(boxes):
    boxes.sort(key=lambda v: max(v[1][0], v[1][1]), reverse=True)
    pprint(boxes)
    return arrange(boxes)

def composite(arrangement):
    x = 0
    y = 0
    boundaries = zip(*arrangement)
    #TODO: something
    for x_val, y_val in zip(boundaries[0][l]+boundaries[0][0]):
        if x_val > x:
            x = x_val
        if y_val > y:
            y = y_val
    im = Image.new("RGBA", (512, 512), "white")

def load_images(dirname, recursive=False):
    if recursive:
        for dirname, dirnames, filenames in os.walk(directory):
            for filename in filenames:
                if PIL_check_image(dirname+'/'+filename):
                    print 'file:', os.path.join(dirname, filename)
    else:
        for filename in os.listdir(directory):
            if PIL_check_image(dirname+'/'+filename):
                print 'file:', os.path.join(dirname, filename)

def PIL_check_image(path):
    try:
        return Image.open(path)
    except IOError:
        return False

if __name__ == '__main__':
    #directory = raw_input("Directory of images: ")
    #recursive = yesno.query_yes_no("Recursive? ", default="no")
    #load_images(directory, recursive)
    test = ([('apple', (90, 29)), ('orange', (11, 19)), ('durian', (40, 50)),
        ('cane', (80, 10)), ('pear', (10, 30)), ('bannana', (100, 100))])
    print place(test)
