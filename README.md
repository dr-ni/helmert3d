`Origin :`
![GitHub release (latest by date)](https://img.shields.io/github/v/release/dr-ni/helmert3d)
![GitHub Release Date](https://img.shields.io/github/release-date/dr-ni/helmert3d)
![GitHub repo size](https://img.shields.io/github/repo-size/dr-ni/helmert3d)
![GitHub all releases](https://img.shields.io/github/downloads/dr-ni/helmert3d/total)
![GitHub](https://img.shields.io/github/license/dr-ni/helmert3d)  
`GLC.ORG:`
![GitHub release (latest by date)](https://img.shields.io/github/v/release/Geo-Linux-Calculations/helmparms3d)
![GitHub Release Date](https://img.shields.io/github/release-date/Geo-Linux-Calculations/helmparms3d)
![GitHub repo size](https://img.shields.io/github/repo-size/Geo-Linux-Calculations/helmparms3d)
![GitHub all releases](https://img.shields.io/github/downloads/Geo-Linux-Calculations/helmparms3d/total)
![GitHub](https://img.shields.io/github/license/Geo-Linux-Calculations/helmparms3d)  

# helmert3d

## About

The 3D Helmert transformation is a frequently used method in geodesy
to produce transformations between different cartesian coordinate systems:
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
A simple description of the SVD-based algorithm can be found here:
http://dx.doi.org/10.18419/opus-523 on page 148 (A.3.1).

All coordinates must be translated to XYZ coordinates, first, if the coordinate system is not cartesian.
Transformations between ellipsoidal (B L H = LAT LON H) and cartesian (XYZ) coordinates can be managed with the tool helmeltrans.
The ellipsoid name, as well as both axes of the ellipsoid must be predefined in a seperate textfile (ellipsoid_infile).
See also [common_ellipsoids.txt](./common_ellipsoids.txt) for common values.

The tool helmparms3d reads two XYZ point-textfiles and calculates all necessary
parameters for Helmert transformation. For further computations these parameters are stored in
an additional textfile. The Helmert transformation can then be calculated with helmert3d. Finally the alignment can be tested with helmdiff3d.

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
helmparms3d xyz_src_infilename xyz_dest_infilename [parms_outfilename]
```

Transformation:
```sh
helmert3d xyz_src_infilename param_infilename [xyz_transformed_outfilename]
```

Coordinate system translatation:
```sh
helmeltrans {blh|xyz} src_infilename ellipsoid_infilename [outfilename]
```

Test:
```sh
helmdiff3d xyz_dest_infilename xyz_transformed_infilename [xyz_diff_outfilename]
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
 X[1] Y[1] Z[1]
 ..   ..   ..
 ..   ..   ..
 X[n] Y[n] Z[n]
```

BLH data file format:
```
 B[1] L[1] H[1]
 ..   ..   ..
 ..   ..   ..
 B[n] L[n] H[n]
```

Ellipsoid data file format:
```
 name a b
```

See ellipsoids : [Common Ellipsoids](./common_ellipsoids.txt)

----

https://github.com/dr-ni/helmert3d

