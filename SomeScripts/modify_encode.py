#!/usr/bin/python

import os,sys

def convert( filename, in_enc = "GBK", out_enc="UTF-8" ):
    # read the file
    content = open( filename ).read()
    # convert the concent
    try:
        new_content = content.decode( in_enc ).encode( out_enc )
        #write to file
        open( filename, 'w' ).write( new_content )
    except:
        print " error... "

def explore( dir ):
    for root, dirs, files in os.walk( dir ):
        for file in files:
            path = os.path.join( root, file )
            #content = codecs.open(path, 'r').read()
            #source_encoding = chardet.detect(content)['encoding']
            #print source_encoding, path
            print "convert " + path,
            convert( path )
            print " done"

def main():
    if len( sys.argv ) > 1 :
        path = sys.argv[1]
        if os.path.isfile( path ):
            convert( path )
        elif os.path.isdir( path ):
            explore( path )

if __name__ == "__main__":
    main() 
	
	
##	C:\Windows\System32>E:\MyGithub\terst\encode.py "E:\MyGithub\terst\Classes"