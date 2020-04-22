from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

LoRa_Extension = Extension(
	name="lorthon",
	sources=["lorthon.pyx"],
	libraries=["interface_lora", "loragw", "parson"],
	library_dirs=["lib"],
	include_dirs=["inc", "libloragw/inc", "parson"]
)
setup(
	name="lorthon",
	#ext_modules=cythonize([LoRa_Extension])
        ext_modules=cythonize([LoRa_Extension], language_level="3")
)
