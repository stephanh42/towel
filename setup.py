#setup.py
from distutils.core import setup
from distutils.extension import Extension
from glob import glob
from sys import version_info

files = glob("src/*.cxx") + ["src/mersenne/mt19937ar.c"]
headers = glob("src/*.h*") + ["src/mersenne/mt19937ar.h"]

setup(name="towel",    
      version="0.1",
      description="Utilities for roguelike games",
      author="Stephan Houben",
      author_email="stephanh@planet.nl",
      ext_modules=[
                    Extension("towel",files,
                      libraries=["boost_python-py%d%d" % (version_info.major, version_info.minor)],
                      depends=headers
                      ),
                    ]
     )
