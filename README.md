helmparms3d

The 3D Helmert transformation is a frequently used method in geodesy
to produce transformations between different Cartesian coordinate systems:

Xt=T+sRX (1)

where

Xt = transformed coordinate vector

X = initial coordinate vector

T = translation vector

R = rotation matrix including rx, ry, rz

rx, ry, rz = rotations around the coordinate axes

s = scale factor

Seven parameters are required for this kind of transformation
and can be calculated from a set of at least three point correspondences.
helmparms3d (a free command-line tool) reads two ASCII point-files and
calculates all necessary papameters for Helmert transformation.
All parameters are written to an ASCII file for further computations.

Syntax:
$ helmparms3d src_points.txt dest_points.txt outfile_name.txt

Build: make

Install: sudo make install

Uninstall: sudo make uninstall

Usage: ipa2arpabet-de filename
