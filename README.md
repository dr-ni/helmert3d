# helmparms3d

## About

The 3D Helmert transformation is a frequently used method in geodesy
to produce transformations between different Cartesian coordinate systems:
```
Xt=T+sRX
```

where
```
Xt = transformed coordinate vector

X = initial coordinate vector

T = translation vector

R = rotation matrix including rx, ry, rz

rx, ry, rz = rotations around the coordinate axes

s = scale factor
```

Seven parameters are required for this kind of transformation
and can be calculated from a set of at least three point correspondences.
helmparms3d (a free command-line tool) reads two ASCII point-files and
calculates all necessary papameters for Helmert transformation.
All parameters are written to an ASCII file for further computations.

## Development

Build:
```sh
make
```

Install:
```sh
sudo make install
```

Uninstall:
```sh
sudo make uninstall
```

## Use

Parameter computation:
```sh
helmparms3d [xyz_src_points] [xyz_dest_points] [helmert_param_filename]
```

Transformation:
```sh
helmert3d [xyz_filename] [helmert_param_filename]
```

## Files

Helmert parameter file format:
```
 r11 r12 r13
 r21 r22 r23
 r31 r32 r33
 tx ty tz
 m
```

XYZ data file format:
```
 x[1] y[1] z[1]
 ..   ..   ..
 ..   ..   ..
 x[n] y[n] z[n]
```

----

https://github.com/dr-ni/helmparms3d

