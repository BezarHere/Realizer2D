import shutil, sys, os
from glassy import utils, atMain

this_folder = utils.parent_path(__file__)

def copy_shaders():
  args = sys.argv[1:]
  output_folder = args[0][:-1] + "\\shaders"
  shaders_folder = this_folder + '\\shaders'
  if not os.path.isdir(shaders_folder):
    os.mkdir(shaders_folder)
  print(f"{output_folder=}")
  print(f"{shaders_folder=}")
  shutil.copytree(shaders_folder, output_folder, dirs_exist_ok=True)
    

@atMain
def main():
  copy_shaders()
	

