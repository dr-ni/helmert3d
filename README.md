![GitHub release (latest by date)](https://img.shields.io/github/v/release/Geo-Linux-Calculations/helmparms3d)
![GitHub Release Date](https://img.shields.io/github/release-date/Geo-Linux-Calculations/helmparms3d)
![GitHub repo size](https://img.shields.io/github/repo-size/Geo-Linux-Calculations/helmparms3d)
![GitHub all releases](https://img.shields.io/github/downloads/Geo-Linux-Calculations/helmparms3d/total)
![GitHub](https://img.shields.io/github/license/Geo-Linux-Calculations/helmparms3d)

# helmert3d

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
and can be calculated from a set of at least three point correspondences
[Crosilla, F. (1999). Procrustes analysis and geodetic sciences. Technical report, Quo
vadis geodesia...?, Department of Geodesy and GeoInformatics, University of Stuttgart].
The tool helmparms3d reads two point-textfiles and calculates all necessary
parameters for Helmert transformation. For further computations these parameters are stored in
an additional textfile. The Helmert transformation can
then be calculated with helmert3d. Finally the alignment can be tested with helmdiff3d.

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
helmparms3d [xyz_src_infilename] [xyz_dest_infilename] [parms_outfilename]
```

Transformation:
```sh
helmert3d [xyz_src_infilename] [param_infilename] [xyz_transformed_outfilename]
```
Test:
```sh
helmdiff3d [xyz_dest_infilename] [xyz_transformed_infilename] [xyz_diff_outfilename]
```

## Files

Helmert parameter file format:
```
 r11 r12 r13
 r21 r22 r23
 r31 r32 r33
 tx ty tz
 s
```

XYZ data file format:
```
 x[1] y[1] z[1]
 ..   ..   ..
 ..   ..   ..
 x[n] y[n] z[n]
```

----

https://github.com/dr-ni/helmert3d

