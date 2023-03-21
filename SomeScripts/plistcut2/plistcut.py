#!python
#-*-coding:utf-8-*-
import os,sys
from xml.etree import ElementTree
from PIL import Image
 
def tree_to_dict(tree):
    d = {}  #字典 {key:value}
    for index, item in enumerate(tree):
        if item.tag == 'key':
            if tree[index+1].tag == 'string':
                d[item.text] = tree[index + 1].text
            elif tree[index + 1].tag == 'true':
                d[item.text] = True
            elif tree[index + 1].tag == 'false':
                d[item.text] = False
            elif tree[index+1].tag == 'dict':
                d[item.text] = tree_to_dict(tree[index+1])
    #遍历打印我们需要的参数包括key，string，true,false
    #for v,k in d.items():
    #    print('{v}:{k}'.format(v = v, k = k))    
    return d 
    
def gen_png_from_plist(plist_filename, png_filename):
    file_path = plist_filename.replace('.plist', '')
    #print(file_path)
    big_image = Image.open(png_filename)
 
    str_tmp = open(plist_filename, 'r').read()
    #print(str_tmp)
 
    root = ElementTree.fromstring(str_tmp)
 
    plist_dict = tree_to_dict(root[0])
    to_list = lambda x: x.replace('{','').replace('}','').split(',') 
 
    print("*****************************************")
    #print(plist_dict['frames'].items())
 
    for k,v in plist_dict['frames'].items():
        spriteSourceSize = v["spriteSourceSize"] if v.__contains__("spriteSourceSize") else v["sourceSize"]
        spriteSourceSize = [ int(x) for x in to_list(spriteSourceSize) ]
        # pack后剩下的有效区域
        textureRect = v["textureRect"] if v.__contains__("textureRect") else v["frame"]
        textureRect = [ int(x) for x in to_list(textureRect) ]
        # 是否旋转
        isRotate = v["textureRotated"] if v.__contains__("textureRotated") else v["rotated"]
        print(isRotate)
        # 小图在大图上的区域
        spriteOffset = v["spriteOffset"] if v.__contains__("spriteOffset") else v["offset"]
        spriteOffset = [ int(x) for x in to_list(spriteOffset) ]
        # 获得长宽
        width = int( textureRect[3] if isRotate else textureRect[2] )  
        height = int( textureRect[2] if isRotate else textureRect[3] ) 
        box = [ textureRect[0], textureRect[1], textureRect[0] + width, textureRect[1] + height ]
 
        rect_on_big = big_image.crop(box)
        #rect_on_big.show();
        if isRotate:
            rect_on_big = rect_on_big.rotate(90,expand = True)#expand = True避免result_image.paste报错image unmatch
            #rect_on_big.show();
 
        result_image = Image.new('RGBA', spriteSourceSize, (0,0,0,0))
        if isRotate:
            result_box=(  
                int(( spriteSourceSize[0] - height)/2 + spriteOffset[0]),  
                int(( spriteSourceSize[1] - width)/2 - spriteOffset[1]),  
                int(( spriteSourceSize[0] + height)/2 + spriteOffset[0]),  
                int(( spriteSourceSize[1] + width)/2 - spriteOffset[1])   
                )
            print(result_box)  
        else:
            result_box=(  
                int(( spriteSourceSize[0] - width)/2 + spriteOffset[0]),  
                int(( spriteSourceSize[1] - height)/2 - spriteOffset[1]),  
                int(( spriteSourceSize[0] + width)/2 + spriteOffset[0]),  
                int(( spriteSourceSize[1] + height)/2 - spriteOffset[1]) 
                )
            print(result_box)       
        result_image.paste(rect_on_big, result_box, mask = 0)
 
        outfile = filename + '/' + k
        print (outfile, "generated")
        if not os.path.exists(os.path.dirname(outfile)):
            os.makedirs(os.path.dirname(outfile))
 
        result_image.save(outfile)
 
        if isRotate:
            print("isRotate", isRotate, k)
 
 
if __name__ == '__main__':
    filename = sys.argv[1]  # 输入的文件名
    plist_filename = filename + '.plist' # 
    png_filename = filename + '.png'  #
    if (os.path.exists(plist_filename) and os.path.exists(png_filename)):  #
        gen_png_from_plist( plist_filename, png_filename )
    else:
        print ("make sure you have both plist and png files in the same directory")