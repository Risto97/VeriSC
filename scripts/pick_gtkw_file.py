#!/usr/bin/env python3
import subprocess
import glob
import sys, getopt
import os
from simple_term_menu import TerminalMenu


def print_usage():
    print('pick_gtkw_file.py -d <directory>')
    print('pick_gtkw_file.py -w <gtkw_file.gtkw>')
    print('pick_gtkw_file.py -h')

def main(argv):
    search_dir = ''
    in_file = ''
    try:
        opts, args = getopt.getopt(argv,"hd:f:", ["dir=", "file="])
    except getopt.GetoptError:
        print_usage()

    for opt, arg in opts:
        if opt == '-h':
            print_usage()
        elif opt in ("-d", "--dir"):
            search_dir = arg
            search_dir = os.path.abspath(search_dir)
            assert os.path.exists(search_dir), "Directory does not exist"
            assert not os.path.isfile(search_dir), "Directory path is a file"

        elif opt in ("-f", "--file"):
            in_file = arg
            assert os.path.isfile(in_file), "File doesnt exist or is not a file"
            assert in_file.endswith('.gtkw'), "File must be .gtkw"
            p = subprocess.Popen(["gtkwave", in_file])
            exit() 
                
    keyboard_list = "123qweasdzxc456rtyfghvbn789uiohjknm,0p;.-=QWEASDZXCRTYFGHVBNUIOJKL"
    keyboard_list = [char for char in keyboard_list]

    gtkw_files = glob.glob(search_dir+'/*.gtkw')
    gtkw_bn = [os.path.basename(p) for p in gtkw_files]


    gtkw_bn = [f"[{keyboard_list[i]}] " + p  for i, p in enumerate(gtkw_bn)]

    fruits = ["[1] apple", "[2] banana", "[3] orange"]
    terminal_menu = TerminalMenu(gtkw_bn, title="GTKWave save files", exit_on_shortcut=False, multi_select=True, show_multi_select_hint=True, multi_select_select_on_accept=False)
    menu_entry_index = terminal_menu.show()
    
    for i in menu_entry_index:
        p = subprocess.Popen(["gtkwave", gtkw_files[i]])


if __name__ == "__main__":
    main(sys.argv[1:])
