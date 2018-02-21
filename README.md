# mt.exe wrapper

     mt.exe : general error c101008d: Failed to write the updated manifest to the resource of file

Having trouble compiling your project with MSVC toolchain with this message? 

Screw it. It could be your AV program (including Windows Defender) or any other thing blocking write access to file. 
But mt.exe does not know about it and just gives up. So let's tell him to try again!

## Installation

Compile it, rename original mt.exe to mt-orig.exe and place new mt.exe to same directory 
(i.e. `C:\Program Files (x86)\Windows Kits\10\bin\x64`).

## Credits

Thanks to user `ulatekh` from `stackoverflow.com` for original idea.

https://stackoverflow.com/questions/3775406/mt-exe-general-error-c101008d-failed-to-write-the-updated-manifest-to-the-res
